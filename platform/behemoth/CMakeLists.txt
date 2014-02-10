#
# This file is part of DronolabSTM32-embedded. 
# 


# Include the STM32 drivers folder
include_directories(
	${PROJECT_SOURCE_DIR}/drivers/cortex-m3
	${PROJECT_SOURCE_DIR}/drivers/stm32
	${PROJECT_SOURCE_DIR}/drivers/stm32f4xx)

# Add the platform library
add_library(platform STATIC
	${PROJECT_SOURCE_DIR}/platform/platform
	behemoth 
	behemoth_button
	behemoth_leds
	behemoth_drivers
	behemoth_periph
	behemoth_lib
	behemoth_net
	)

# Allow for some more cyclic deps in libraries
set_property(TARGET platform APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 3)

# Link the library to the drivers and peripherals
target_link_libraries(platform
# Driver
	drivers_stm32f4xx
	
# Lib
	freertos
	random
	printf
	event
	softtimer
	att_ctrl

# Periph 
	lis302dl
	motors
	rc
	gx3
	tlc59116
	
	)
	