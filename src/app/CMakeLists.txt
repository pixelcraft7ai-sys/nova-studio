add_library(nova_app_lib STATIC
  app_services.cpp
  error_presenter.cpp
  main_window.cpp
  welcome_widget.cpp)
add_library(nova::app_lib ALIAS nova_app_lib)
target_include_directories(nova_app_lib PUBLIC .)
target_link_libraries(nova_app_lib PUBLIC
  nova::core nova::document nova::render nova::formats nova::platform
  Qt6::Core Qt6::Widgets)
nova_configure_target(nova_app_lib)

add_executable(nova-studio main.cpp)
target_link_libraries(nova-studio PRIVATE nova::app_lib)
nova_configure_target(nova-studio)
set_target_properties(nova-studio PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE ON)
