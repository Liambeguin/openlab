# This file is part of DronolabSTM32-embedded 
#
# DronolabSTM32-embedded is a student project designed 
# for flying drones.
#
# Go to < dronolab.etsmtl.ca > to find out more about 
# the project

# Created on: 2013-10-13
#     Author: liam
#


add_executable(test_stm32f4eval_accelero stm32f4eval_accelero)
target_link_libraries(test_stm32f4eval_accelero platform )
