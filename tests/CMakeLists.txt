ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(TEST_BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

set(TEST_NAMES
    BigIntTest
    NaiveMultTest
    KaratsubaMultTest
    FFTMultTest
)

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)

    target_include_directories(${TEST_NAME} PUBLIC
        ${Boost_INCLUDE_DIRS}
    )

    target_link_libraries(${TEST_NAME} 
        integer
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

    add_test(
        NAME ${TEST_NAME}
        WORKING_DIRECTORY ${TEST_BIN_DIR}
        COMMAND ${TEST_BIN_DIR}/${TEST_NAME}
    )

endforeach(TEST_NAME ${TEST_NAMES})
