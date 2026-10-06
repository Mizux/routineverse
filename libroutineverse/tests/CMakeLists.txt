add_executable(routineverse_tests test_routineverse.cpp)
target_link_libraries(routineverse_tests PRIVATE
  ${PROJECT_NAMESPACE}::libroutineverse
)
target_compile_features(routineverse_tests PRIVATE cxx_std_20)
target_compile_options(routineverse_tests PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -fstack-protector>
)
set_target_properties(routineverse_tests PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
if(APPLE)
  set_target_properties(routineverse_tests PROPERTIES
    INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX AND NOT APPLE)
  set_target_properties(routineverse_tests PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

add_test(NAME routineverse_tests COMMAND routineverse_tests)

