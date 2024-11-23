include(FetchBoost)

add_library(liblearner learner.cpp mat_language.cpp)
add_library(stewkk::liblearner ALIAS liblearner)
target_compile_features(liblearner PUBLIC cxx_std_20)
set_target_properties(liblearner PROPERTIES
  CXX_STANDART 20
  CXX_STANDART_REQUIRED YES
  CXX_EXTENSIONS YES
)
set_target_properties(liblearner PROPERTIES OUTPUT_NAME learner)
target_include_directories(
  liblearner PUBLIC $<INSTALL_INTERFACE:include>
                    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_include_directories(
  liblearner PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)
target_link_libraries(liblearner PRIVATE Boost::process)
target_compile_options(liblearner PRIVATE ${BASE_COMPILE_FLAGS})

add_executable(learner main.cpp)
target_compile_features(learner PRIVATE cxx_std_20)
set_target_properties(learner PROPERTIES
  CXX_STANDART 20
  CXX_STANDART_REQUIRED YES
  CXX_EXTENSIONS YES
)
target_compile_options(learner PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(learner PRIVATE liblearner)
