#
# ADD proper header
#


# Add the LIS302DL library
add_library(lis302dl STATIC lis302dl/lis302dl)
target_link_libraries(lis302dl platform)