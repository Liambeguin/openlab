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
	boreas
	button
	leds
	drivers
	periph
	lib
	net
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
	differential
	att_ctrl
	circular_buff
	hush

# Periph
	motors
	rc
	gx3
	tlc59116
	vn200
	datalink
	)
