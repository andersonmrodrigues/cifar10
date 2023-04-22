# Definição de variáveis
CMSIS_DIR	= ../CMSIS_5
DSP_DIR	    = $(CMSIS_DIR)/CMSIS/DSP
NN_DIR	    = $(CMSIS_DIR)/CMSIS/NN
CORE_DIR	= $(CMSIS_DIR)/CMSIS/Core/Include

# Definição de includes para o pré-processador
APPINCLUDE:=-I$(APP_ROOT) \
			-I$(APP_ROOT)/Include \
			-I$(CORE_DIR) \
			-I$(DSP_DIR)/Include \
			-I$(NN_DIR)/Include \

# Definição de flags de compilação
VERBOSE=1
WORKING_FOLDER=$(shell pwd)
TOOL_CHAIN=gcc

APP_ROOT?=$(WORKING_FOLDER)

MULT?=
DEF?=

# Adiciona arquivos fonte do CMSIS NN na variável SRC
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

# Definição do compilador
GNG_CC=gcc
# Definição de arquivos fonte
SRCINC?=
ASSDBG=
INCOBJ=$(patsubst %.c,%$(MULT).$(CROSS).o,$(SRCINC))
EXE=$(patsubst %.c,%.o, $(SRC))
EXEPP=$(patsubst %.cpp,%.o,$(SRCPP))
CFLAGS += -O2
CFLAGS += -w -g
CFLAGS += -std=gnu99

# Adiciona os arquivos fonte do diretório atual à variável SRC
SRC+= \
	$(wildcard *.c)

# Adiciona os arquivos fonte do diretório atual à variável SRCPP
SRCPP+=$(wildcard *.cpp)

# Cria uma lista com os objetos gerados a partir dos arquivos fonte da variável SRC
SRCOBJ := $(patsubst %.c, %.o,$(SRC))

#
#---------------------------------------------------
all: all_$(TOOL_CHAIN)

#---------------------------------------------------
# Definindo alvo para compilador gcc
ifeq ($(TOOL_CHAIN),gcc)

all_gcc: clean exe

# Compilando código e criando executável
exe::
	# Building testcase
	$(V) $(GNG_CC) $(CFLAGS) \
		$(SRC) \
		$(APPINCLUDE) -o app.exe
		
endif

# --------------------------------------------------
# Definindo alvo de limpeza para remover arquivos gerados
clean::
	- rm -f *.$(CROSS).elf *.$(CROSS).o $(NN_DIR)/Source/*/*.o $(COREA_DIR)/Source/*.o $(DEVICE_DIR)/Source/*.o *.o *.exe
