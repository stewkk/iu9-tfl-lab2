include(FetchGTest)
include(GoogleTest)

include(FetchBoost)

add_executable(unittests ${PROJECT_SOURCE_DIR}/src/learner/learner_test.cpp)
target_compile_options(unittests PRIVATE ${BASE_COMPILE_FLAGS})
# TODO: remove
target_include_directories(
  unittests PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)
target_link_libraries(unittests PRIVATE stewkk::liblearner gtest_main Boost::process # TODO: remove
)
gtest_discover_tests(unittests)
