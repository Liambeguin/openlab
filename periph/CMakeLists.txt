#
# ADD proper header
#


# Add the LIS302DL library
add_library(lis302dl STATIC lis302dl/lis302dl)
target_link_libraries(lis302dl platform)

# Add the MOTORS library
add_library(motors STATIC motors/motors)
target_link_libraries(motors platform)

# Add the Radio Control library
add_library(rc STATIC rc/rc)
target_link_libraries(rc platform)

# Add the GX3 library
include_directories(GX3 GX3/SCP)
add_library(gx3 STATIC GX3/gx3 GX3/circular_buff)
add_library(scp STATIC GX3/SCP/scp_parser GX3/SCP/util)
target_link_libraries(gx3 scp platform)

# Add the TCL59116 library
add_library(tlc59116 STATIC tlc59116/tlc59116)
target_link_libraries(tlc59116 platform)