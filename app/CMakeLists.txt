# Qt Configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Dependencies
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
set(QT_VERSION_MAJOR 6)

# Executable
add_executable(routineverse)
target_sources(routineverse PRIVATE
  config.h
  main.cpp
  window-cb.cpp
  window-cb.h
  window.cpp
  window.h
)
target_include_directories(routineverse PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_features(routineverse PRIVATE cxx_std_20)
target_compile_options(routineverse PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -fstack-protector>
)
set_target_properties(routineverse PROPERTIES
  VERSION ${PROJECT_VERSION}
  POSITION_INDEPENDENT_CODE ON
)
# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_target_properties(routineverse PROPERTIES
    INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX AND NOT APPLE)
  set_target_properties(routineverse PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

target_link_libraries(routineverse PRIVATE
  ${PROJECT_NAMESPACE}::libroutineverse
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Charts
)
add_executable(${PROJECT_NAMESPACE}::routineverse ALIAS routineverse)

# Install
install(TARGETS routineverse
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
