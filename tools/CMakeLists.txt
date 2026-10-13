add_executable(gen_docs)
target_sources(gen_docs PRIVATE
  gen_docs.cpp
)
target_include_directories(gen_docs PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_features(gen_docs PRIVATE cxx_std_20)
target_compile_options(gen_docs PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -fstack-protector>
)
set_target_properties(gen_docs PROPERTIES
  VERSION ${PROJECT_VERSION}
  POSITION_INDEPENDENT_CODE ON
)
if(APPLE)
  set_target_properties(gen_docs PROPERTIES
    INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX AND NOT APPLE)
  set_target_properties(gen_docs PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

target_link_libraries(gen_docs PRIVATE
  ${PROJECT_NAMESPACE}::libroutineverse
)
add_executable(${PROJECT_NAMESPACE}::gen_docs ALIAS gen_docs)

add_custom_target(docs
  COMMAND gen_docs "${CMAKE_SOURCE_DIR}/docs"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  DEPENDS gen_docs
  COMMENT "Regenerating docs/*.md"
)
