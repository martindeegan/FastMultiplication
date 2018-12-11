
add_library(integer SHARED 
    BigInt.cpp
    NaiveMultiplier.cpp
    KaratsubaMultiplier.cpp
    FFTMultiplier.cpp
)

add_executable(main main.cpp)
target_link_libraries(main integer)

add_executable(fft fft.cpp)
target_link_libraries(fft integer)