configure_file(include/nova/core/version.hpp.in
               "${CMAKE_CURRENT_BINARY_DIR}/generated/nova/core/version.hpp" @ONLY)

add_library(nova_core STATIC
  logger.cpp
  result.cpp
  settings.cpp)
add_library(nova::core ALIAS nova_core)
target_include_directories(nova_core PUBLIC
  include
  "${CMAKE_CURRENT_BINARY_DIR}/generated")
nova_configure_target(nova_core)
