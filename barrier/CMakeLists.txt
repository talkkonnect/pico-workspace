cmake_minimum_required(VERSION 3.18)

add_executable(barrier
        barrier.c
        )

# pull in common dependencies
target_link_libraries(barrier pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(barrier)

# add url via pico_set_program_url
#barrier_auto_set_url(barrier)
