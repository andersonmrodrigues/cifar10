# parallel-rnc-cifar10

## Description

Parallelize the Convolutional Neural Network (CNN) application CIFAR-10, developed in C/C++, available within the CMSIS-NN library examples for Arm microcontrollers. CMSIS repository: https://github.com/ARM-software/CMSIS_5/tree/5.7.0/

## Details

### Folders

- /code_paral_clang: will use clang compilator and implementation will run in parallel by the threads numbers set in code
- /code_paral_gcc: will use gcc compilator and implementation will run in parallel by the threads numbers set in code
- /code_seq_clang: will use clang compilator and implementation will run in sequencial
- /code_seq_gcc: will use gcc compilator and implementation will run in sequencial

## Instructions (follow the order)

- Git clone https://github.com/ARM-software/CMSIS_5.git
- cd CMSIS_5
- git checkout 5.7.0
- cd ..
- git clone https://github.com/andersonmrodrigues/parallel-rnc-cifar10.git
- cd parallel-rnc-cifar10
- run de images.py file "python3 images.py"
- rename the result file to "arm_nnexamples_cifar10_inputs"
- copy the file "arm_nnexamples_cifar10_inputs" to the implementation you will run
- cd to the folder the implementation you choose
- run "make" to generate the .exe file
- run "./app.exe"
