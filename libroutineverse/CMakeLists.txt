add_library(libroutineverse SHARED)
target_sources(libroutineverse PRIVATE
  combat.cpp
  combat.hpp
  game_state.cpp
  game_state.hpp
  gears.cpp
  gears.hpp
  items.cpp
  items.hpp
  monsters.cpp
  monsters.hpp
  routineverse.cpp
  routineverse.hpp
  skills.cpp
  skills.hpp
  utils.cpp
  utils.hpp
)
target_include_directories(libroutineverse PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/routineverse>
)
target_compile_features(libroutineverse PUBLIC cxx_std_20)
target_compile_options(libroutineverse PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -fstack-protector>
)
set_target_properties(libroutineverse PROPERTIES
  PREFIX ""
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  POSITION_INDEPENDENT_CODE ON
  WINDOWS_EXPORT_ALL_SYMBOLS ON
  PUBLIC_HEADER "combat.hpp;game_state.hpp;gears.hpp;items.hpp;monsters.hpp;routineverse.hpp;skills.hpp;utils.hpp"
)
if(APPLE)
  set_target_properties(libroutineverse PROPERTIES INSTALL_RPATH "@loader_path")
elseif(UNIX AND NOT APPLE)
  set_target_properties(libroutineverse PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()
add_library(${PROJECT_NAMESPACE}::libroutineverse ALIAS libroutineverse)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

# Install
install(TARGETS libroutineverse
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/routineverse
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
