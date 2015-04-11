#


# Adding Manual application
add_executable(manual_setup manual.c)
target_link_libraries(manual_setup platform) 

# Adding hush application
add_executable(hush_app hush.c)
target_link_libraries(hush_app platform )
