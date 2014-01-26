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

#adding void test task 
add_executable(test_periph_void void.c)
target_link_libraries(test_periph_void platform )

#adding PWM dummy test
add_executable(test_periph_motors motors)
target_link_libraries(test_periph_motors platform )

#adding RC dummy test
add_executable(test_periph_rc rc)
target_link_libraries(test_periph_rc platform )

#adding LED dummy test
add_executable(test_periph_tlc59116 tlc59116)
target_link_libraries(test_periph_tlc59116 platform )

#adding GX3 dummy test
add_executable(test_periph_gx3 gx3)
target_link_libraries(test_periph_gx3 platform )

#adding LIS302DL dummy test
add_executable(test_periph_lis302dl lis302dl)
target_link_libraries(test_periph_lis302dl platform )
