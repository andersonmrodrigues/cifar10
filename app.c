#include <stdint.h>
#include <stdio.h>
#include "arm_math.h"
#include "arm_nnexamples_cifar10_parameter.h"
#include "arm_nnexamples_cifar10_weights.h"

#include "arm_nnfunctions.h"
#include "arm_nnexamples_cifar10_inputs.h"

#include <time.h>

// include the input and weights

static q7_t conv1_wt[CONV1_IM_CH * CONV1_KER_DIM * CONV1_KER_DIM * CONV1_OUT_CH] = CONV1_WT;
static q7_t conv1_bias[CONV1_OUT_CH] = CONV1_BIAS;

static q7_t conv2_wt[CONV2_IM_CH * CONV2_KER_DIM * CONV2_KER_DIM * CONV2_OUT_CH] = CONV2_WT;
static q7_t conv2_bias[CONV2_OUT_CH] = CONV2_BIAS;

static q7_t conv3_wt[CONV3_IM_CH * CONV3_KER_DIM * CONV3_KER_DIM * CONV3_OUT_CH] = CONV3_WT;
static q7_t conv3_bias[CONV3_OUT_CH] = CONV3_BIAS;

static q7_t ip1_wt[IP1_DIM * IP1_OUT] = IP1_WT;
static q7_t ip1_bias[IP1_OUT] = IP1_BIAS;

/* Here the image_data should be the raw uint8 type RGB image in [RGB, RGB, RGB ... RGB] format 
uint8_t   image_data[CONV1_IM_CH * CONV1_IM_DIM * CONV1_IM_DIM] = IMG_DATA;*/

uint8_t   image_data[CONV1_IM_CH * CONV1_IM_DIM * CONV1_IM_DIM];

q7_t      output_data[IP1_OUT];

//vector buffer: max(im2col buffer,average pool buffer, fully connected buffer)
q7_t      col_buffer[2 * 5 * 5 * 32 * 2];

q7_t      scratch_buffer[32 * 32 * 10 * 4];

int main()
{
  clock_t start, end;
  double time_spend;
  
  start = clock();
	
  #ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize (EventRecordAll, 1);  // initialize and start Event Recorder
  #endif

  printf("start execution\n");
  /* start the execution */
  
  for(int i=0; i < 100; i++) {
	  /*
	  char img_data_name[20];
	  sprintf(img_data_name, "IMG_DATA%i", i);
	  printf("count %s", img_data_name);
	  memcpy(image_data, img_data_name, sizeof(image_data));
	  image_data = 
	*/
	  int idx = 0;
	  for(int j = 0; j < 3072; j++) {
		image_data[idx++] = IMG_DATA[i][j];  
	  }
	
	  q7_t     *img_buffer1 = scratch_buffer;
	  q7_t     *img_buffer2 = img_buffer1 + 32 * 32 * 32;
	
	  /* input pre-processing */
	  int mean_data[3] = INPUT_MEAN_SHIFT;
	  unsigned int scale_data[3] = INPUT_RIGHT_SHIFT;
	  for (int i=0;i<32*32*3; i+=3) {
	    img_buffer2[i] =   (q7_t)__SSAT( ((((int)image_data[i]   - mean_data[0])<<7) + (0x1<<(scale_data[0]-1)))
	                             >> scale_data[0], 8);
	    img_buffer2[i+1] = (q7_t)__SSAT( ((((int)image_data[i+1] - mean_data[1])<<7) + (0x1<<(scale_data[1]-1)))
	                             >> scale_data[1], 8);
	    img_buffer2[i+2] = (q7_t)__SSAT( ((((int)image_data[i+2] - mean_data[2])<<7) + (0x1<<(scale_data[2]-1)))
	                             >> scale_data[2], 8);
	  }
	  
	  // conv1 img_buffer2 -> img_buffer1
	  arm_convolve_HWC_q7_RGB(img_buffer2, CONV1_IM_DIM, CONV1_IM_CH, conv1_wt, CONV1_OUT_CH, CONV1_KER_DIM, CONV1_PADDING,
	                          CONV1_STRIDE, conv1_bias, CONV1_BIAS_LSHIFT, CONV1_OUT_RSHIFT, img_buffer1, CONV1_OUT_DIM,
	                          (q15_t *) col_buffer, NULL);
	
	  arm_relu_q7(img_buffer1, CONV1_OUT_DIM * CONV1_OUT_DIM * CONV1_OUT_CH);
	
	  // pool1 img_buffer1 -> img_buffer2
	  arm_maxpool_q7_HWC(img_buffer1, CONV1_OUT_DIM, CONV1_OUT_CH, POOL1_KER_DIM,
	                     POOL1_PADDING, POOL1_STRIDE, POOL1_OUT_DIM, NULL, img_buffer2);
	
	  // conv2 img_buffer2 -> img_buffer1
	  arm_convolve_HWC_q7_fast(img_buffer2, CONV2_IM_DIM, CONV2_IM_CH, conv2_wt, CONV2_OUT_CH, CONV2_KER_DIM,
	                           CONV2_PADDING, CONV2_STRIDE, conv2_bias, CONV2_BIAS_LSHIFT, CONV2_OUT_RSHIFT, img_buffer1,
	                           CONV2_OUT_DIM, (q15_t *) col_buffer, NULL);
	
	  arm_relu_q7(img_buffer1, CONV2_OUT_DIM * CONV2_OUT_DIM * CONV2_OUT_CH);
	
	  // pool2 img_buffer1 -> img_buffer2
	  arm_maxpool_q7_HWC(img_buffer1, CONV2_OUT_DIM, CONV2_OUT_CH, POOL2_KER_DIM,
	                     POOL2_PADDING, POOL2_STRIDE, POOL2_OUT_DIM, col_buffer, img_buffer2);
	
	// conv3 img_buffer2 -> img_buffer1
	  arm_convolve_HWC_q7_fast(img_buffer2, CONV3_IM_DIM, CONV3_IM_CH, conv3_wt, CONV3_OUT_CH, CONV3_KER_DIM,
	                           CONV3_PADDING, CONV3_STRIDE, conv3_bias, CONV3_BIAS_LSHIFT, CONV3_OUT_RSHIFT, img_buffer1,
	                           CONV3_OUT_DIM, (q15_t *) col_buffer, NULL);
	
	  arm_relu_q7(img_buffer1, CONV3_OUT_DIM * CONV3_OUT_DIM * CONV3_OUT_CH);
	
	  // pool3 img_buffer-> img_buffer2
	  arm_maxpool_q7_HWC(img_buffer1, CONV3_OUT_DIM, CONV3_OUT_CH, POOL3_KER_DIM,
	                     POOL3_PADDING, POOL3_STRIDE, POOL3_OUT_DIM, col_buffer, img_buffer2);
	
	  arm_fully_connected_q7_opt(img_buffer2, ip1_wt, IP1_DIM, IP1_OUT, IP1_BIAS_LSHIFT, IP1_OUT_RSHIFT, ip1_bias,
	                             output_data, (q15_t *) img_buffer1);
	
	  arm_softmax_q7(output_data, 10, output_data);
	
	  for (int i = 0; i < 10; i++)
	  {
	      printf("%d: %d\n", i, output_data[i]);
	  }
  
  }
  
  end = clock();
  
  time_spend = ((double) (end - start)) / CLOCKS_PER_SEC;
  
  printf("Tempo para Executar: %f segundos", time_spend);

  return 0;
}
