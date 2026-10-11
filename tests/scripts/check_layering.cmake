# Fails if a module includes something its layer may not depend on.
# Usage: cmake -DSOURCE_DIR=<repo>/src -P check_layering.cmake
if(NOT SOURCE_DIR)
  message(FATAL_ERROR "SOURCE_DIR is required")
endif()

# module -> regex of FORBIDDEN include patterns
set(RULE_core     "#include[ \t]*[<\"]Q|#include[ \t]*[<\"]nova/(document|render|formats|platform)/")
set(RULE_document "#include[ \t]*[<\"]Q|#include[ \t]*[<\"]nova/(render|formats|platform)/")
set(RULE_render   "#include[ \t]*[<\"]Q|#include[ \t]*[<\"]nova/(formats|platform)/")
set(RULE_formats  "#include[ \t]*[<\"]Q|#include[ \t]*[<\"]nova/(render|platform)/")
set(RULE_platform "#include[ \t]*[<\"]Q(Widgets|Gui)|#include[ \t]*[<\"]Q(Widget|Application|MainWindow|Dialog|MessageBox)|#include[ \t]*[<\"]nova/(document|render|formats)/")

set(violations "")
foreach(mod core document render formats platform)
  file(GLOB_RECURSE files "${SOURCE_DIR}/${mod}/*.hpp" "${SOURCE_DIR}/${mod}/*.cpp" "${SOURCE_DIR}/${mod}/*.h")
  foreach(f ${files})
    file(STRINGS "${f}" lines REGEX "#include")
    foreach(line ${lines})
      if(line MATCHES "${RULE_${mod}}")
        string(APPEND violations "  ${mod}: ${f}: ${line}\n")
      endif()
    endforeach()
  endforeach()
endforeach()

if(violations)
  message(FATAL_ERROR "Layering violations:\n${violations}")
endif()
message(STATUS "layering OK")
