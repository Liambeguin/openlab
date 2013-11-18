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

#adding SPI_2 dummy test 
add_executable(test_stm32f4eval_spi2 stm32f4eval_spi2)
target_link_libraries(test_stm32f4eval_spi2 platform )

#adding PWM dummy test
add_executable(test_stm32f4eval_motors stm32f4eval_motors)
target_link_libraries(test_stm32f4eval_motors platform )

#adding RC dummy test
add_executable(test_stm32f4eval_rc stm32f4eval_rc)
target_link_libraries(test_stm32f4eval_rc platform )
