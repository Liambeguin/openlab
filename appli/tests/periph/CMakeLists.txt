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

# Adding void test task 
add_executable(test_periph_void void.c)
target_link_libraries(test_periph_void platform )

# Adding uart chain test
add_executable(test_periph_uart_chain full_uart.c)
target_link_libraries(test_periph_uart_chain platform)



# Adding Motors test
if(${PLATFORM_HAS_MOTORS})
	add_executable(test_periph_motors motors)
	target_link_libraries(test_periph_motors platform )
endif(${PLATFORM_HAS_MOTORS})


# Adding RC test
if(${PLATFORM_HAS_RC})
	add_executable(test_periph_rc rc)
	target_link_libraries(test_periph_rc platform )
endif(${PLATFORM_HAS_RC})


# Adding TLC59116 test
if(${PLATFORM_HAS_TLC59116})
	add_executable(test_periph_tlc59116 tlc59116)
	target_link_libraries(test_periph_tlc59116 platform )
endif(${PLATFORM_HAS_TLC59116})


# Adding GX3 test
if(${PLATFORM_HAS_GX3})
	add_executable(test_periph_gx3 gx3)
	target_link_libraries(test_periph_gx3 platform )
endif(${PLATFORM_HAS_GX3})


# Adding LIS302DL test
if(${PLATFORM_HAS_LIS302DL})
	add_executable(test_periph_lis302dl lis302dl)
	target_link_libraries(test_periph_lis302dl platform )
endif(${PLATFORM_HAS_LIS302DL})

