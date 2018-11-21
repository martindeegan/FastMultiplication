
add_library(integer SHARED BigInt.cpp)

add_executable(main main.cpp)
target_link_libraries(main integer)