#


# Adding uart chain test
add_executable(setup_rc rc_control.c)
target_link_libraries(setup_rc platform)
