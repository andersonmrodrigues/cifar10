CMSIS_DIR	= ../CMSIS_5
DSP_DIR	    = $(CMSIS_DIR)/CMSIS/DSP
NN_DIR	    = $(CMSIS_DIR)/CMSIS/NN
CORE_DIR	= $(CMSIS_DIR)/CMSIS/Core/Include

APPINCLUDE:=-I$(APP_ROOT) \
			-I$(APP_ROOT)/Include \
			-I$(CORE_DIR) \
			-I$(DSP_DIR)/Include \
			-I$(NN_DIR)/Include \

VERBOSE=1
WORKING_FOLDER=$(shell pwd)
TOOL_CHAIN=gcc

APP_ROOT?=$(WORKING_FOLDER)

MULT?=
DEF?=

SRC+=$(wildcard $(NN_DIR)/Source/ActivationFunctions/arm_relu_q7.c) \
	 $(wildcard $(NN_DIR)/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c) \
	 $(wildcard $(NN_DIR)/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c) \
	 $(wildcard $(NN_DIR)/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c) \
	 $(wildcard $(NN_DIR)/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c) \
	 $(wildcard $(NN_DIR)/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c) \
	 $(wildcard $(NN_DIR)/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c) \
	 $(wildcard $(NN_DIR)/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c) \
	 $(wildcard $(NN_DIR)/Source/PoolingFunctions/arm_pool_q7_HWC.c) \
	 $(wildcard $(NN_DIR)/Source/SoftmaxFunctions/arm_softmax_q7.c) \

GNG_CC=gcc
SRCINC?=
ASSDBG=
INCOBJ=$(patsubst %.c,%$(MULT).$(CROSS).o,$(SRCINC))
EXE=$(patsubst %.c,%.o, $(SRC))
EXEPP=$(patsubst %.cpp,%.o,$(SRCPP))
CFLAGS += -O2
CFLAGS += -w -g
CFLAGS += -std=gnu99

SRC+= \
	$(wildcard *.c)

SRCPP+=$(wildcard *.cpp)

SRCOBJ := $(patsubst %.c, %.o,$(SRC))

#
#---------------------------------------------------
all: all_$(TOOL_CHAIN)

#---------------------------------------------------
#gcc
ifeq ($(TOOL_CHAIN),gcc)

all_gcc: clean exe

exe::
	# Building testcase
	$(V) $(GNG_CC) $(CFLAGS) \
		$(SRC) \
		$(APPINCLUDE) -o app.exe
		
endif

# --------------------------------------------------

clean::
	- rm -f *.$(CROSS).elf *.$(CROSS).o $(NN_DIR)/Source/*/*.o $(COREA_DIR)/Source/*.o $(DEVICE_DIR)/Source/*.o *.o *.exe
