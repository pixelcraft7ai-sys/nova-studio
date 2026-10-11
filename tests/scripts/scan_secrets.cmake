# Scans text files under SCAN_DIR for likely secrets. Fails on any match.
# Usage: cmake -DSCAN_DIR=<dir> -P scan_secrets.cmake
if(NOT SCAN_DIR)
  message(FATAL_ERROR "SCAN_DIR is required")
endif()

set(PATTERNS
  "AKIA[0-9A-Z]+"
  "-----BEGIN [A-Z ]*PRIVATE KEY-----"
  "ghp_[A-Za-z0-9]+"
  "github_pat_[A-Za-z0-9_]+"
  "xox[baprs]-[A-Za-z0-9-]+"
  "sk-[A-Za-z0-9]+"
  "AIza[0-9A-Za-z_-]+"
  "(api[_-]?key|secret|passwd|password|token)[ \t]*[:=][ \t]*[\"'][^\"']+[\"']")
# Minimum lengths that make a pattern credible (CMake regex has no {n,}).
set(MINLEN_0 20)
set(MINLEN_1 30)
set(MINLEN_2 36)
set(MINLEN_3 40)
set(MINLEN_4 20)
set(MINLEN_5 24)
set(MINLEN_6 30)
set(MINLEN_7 14)

file(GLOB_RECURSE candidates RELATIVE "${SCAN_DIR}" "${SCAN_DIR}/*")
set(findings "")
foreach(rel ${candidates})
  if(rel MATCHES "^(\\.git|build|out|cmake-build[^/]*)/" OR rel MATCHES "\\.(png|jpg|jpeg|gif|zip|ico|icns|ttf|otf|woff2?)$")
    continue()
  endif()
  if(rel STREQUAL "tests/scripts/scan_secrets.cmake")
    continue()
  endif()
  file(READ "${SCAN_DIR}/${rel}" content LIMIT 2000000)
  set(i 0)
  foreach(pat ${PATTERNS})
    string(REGEX MATCH "${pat}" hit "${content}")
    if(hit)
      string(LENGTH "${hit}" len)
      if(len GREATER_EQUAL "${MINLEN_${i}}")
        string(APPEND findings "  ${rel}: matches pattern #${i}\n")
      endif()
    endif()
    math(EXPR i "${i}+1")
  endforeach()
endforeach()

if(findings)
  message(FATAL_ERROR "Possible secrets found (values not printed):\n${findings}")
endif()
message(STATUS "secret scan OK")
