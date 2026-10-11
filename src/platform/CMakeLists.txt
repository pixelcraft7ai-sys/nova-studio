add_library(nova_platform STATIC
  local_file_service.cpp
  qt_settings_store.cpp
  app_paths.cpp)
add_library(nova::platform ALIAS nova_platform)
target_include_directories(nova_platform PUBLIC include)
target_link_libraries(nova_platform PUBLIC nova::core Qt6::Core)
nova_configure_target(nova_platform)
