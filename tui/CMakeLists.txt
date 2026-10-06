set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses)

if(NOT CURSES_FOUND)
  message(STATUS "Curses not found, skipping routineverse-tui build")
  return()
endif()

add_executable(routineverse-tui)
target_sources(routineverse-tui PRIVATE
  main.cpp
  tui.cpp
  tui.hpp
)
target_include_directories(routineverse-tui PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/app>
  ${CURSES_INCLUDE_DIRS}
)
target_compile_definitions(routineverse-tui PRIVATE
  _DEFAULT_SOURCE
  _XOPEN_SOURCE=600
)
target_compile_features(routineverse-tui PRIVATE cxx_std_20)
target_compile_options(routineverse-tui PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -fstack-protector>
)
set_target_properties(routineverse-tui PROPERTIES
  VERSION ${PROJECT_VERSION}
  POSITION_INDEPENDENT_CODE ON
)
if(APPLE)
  set_target_properties(routineverse-tui PROPERTIES
    INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX AND NOT APPLE)
  set_target_properties(routineverse-tui PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

target_link_libraries(routineverse-tui PRIVATE
  ${PROJECT_NAMESPACE}::libroutineverse
  ${CURSES_LIBRARIES}
)
add_executable(${PROJECT_NAMESPACE}::routineverse-tui ALIAS routineverse-tui)

# Install
install(TARGETS routineverse-tui
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
