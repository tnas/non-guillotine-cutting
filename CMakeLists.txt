project(non-guillotine-cutting)

add_executable(non-guillotine-cutting main.cpp)

install(TARGETS non-guillotine-cutting RUNTIME DESTINATION bin)

list(APPEND CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_C_COMPILER "/usr/bin/gcc")
