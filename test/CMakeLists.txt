include(GoogleTest)

find_package(GTest REQUIRED)

add_executable(unittests ${PROJECT_SOURCE_DIR}/src/learner/learner_test.cpp)
target_compile_options(unittests PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(unittests PRIVATE stewkk::liblearner gtest::gtest)
gtest_discover_tests(unittests)
