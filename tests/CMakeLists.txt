function(nova_add_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} PRIVATE Qt6::Test ${ARGN})
  nova_configure_target(${name})
  add_test(NAME ${name} COMMAND ${name})
  set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()

nova_add_test(core_test nova::core)
nova_add_test(document_test nova::document)
nova_add_test(render_test nova::render)
nova_add_test(formats_test nova::formats)
nova_add_test(platform_test nova::platform)
nova_add_test(app_test nova::app_lib)

# Integration: the real binary starts, builds its window, and exits cleanly.
add_test(NAME app_smoke_test
  COMMAND nova-studio --smoke-test --config-dir "${CMAKE_BINARY_DIR}/smoke-config")
set_tests_properties(app_smoke_test PROPERTIES
  ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
  PASS_REGULAR_EXPRESSION "smoke test passed"
  TIMEOUT 60)

# Repository policy checks (no Qt/GUI in core, no secrets committed).
add_test(NAME layering_check
  COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_SOURCE_DIR}/src -P ${CMAKE_SOURCE_DIR}/tests/scripts/check_layering.cmake)
add_test(NAME secret_scan
  COMMAND ${CMAKE_COMMAND} -DSCAN_DIR=${CMAKE_SOURCE_DIR} -P ${CMAKE_SOURCE_DIR}/tests/scripts/scan_secrets.cmake)
add_test(NAME secret_scan_selftest
  COMMAND ${CMAKE_COMMAND} -DWORK_DIR=${CMAKE_BINARY_DIR}/secret-selftest
          -DSCANNER=${CMAKE_SOURCE_DIR}/tests/scripts/scan_secrets.cmake
          -P ${CMAKE_SOURCE_DIR}/tests/scripts/secret_scan_selftest.cmake)
add_test(NAME layering_selftest
  COMMAND ${CMAKE_COMMAND} -DWORK_DIR=${CMAKE_BINARY_DIR}/layering-selftest
          -DCHECKER=${CMAKE_SOURCE_DIR}/tests/scripts/check_layering.cmake
          -P ${CMAKE_SOURCE_DIR}/tests/scripts/layering_selftest.cmake)

# --version needs no display.
add_test(NAME app_version_no_display COMMAND nova-studio --version)
set_tests_properties(app_version_no_display PROPERTIES
  ENVIRONMENT "QT_QPA_PLATFORM=" PASS_REGULAR_EXPRESSION "NOVA Studio ${PROJECT_VERSION}")
