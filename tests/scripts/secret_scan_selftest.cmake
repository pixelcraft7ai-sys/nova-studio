# Proves the scanner actually detects a secret (and passes a clean tree).
file(REMOVE_RECURSE "${WORK_DIR}")
file(MAKE_DIRECTORY "${WORK_DIR}/clean" "${WORK_DIR}/dirty")
file(WRITE "${WORK_DIR}/clean/a.txt" "nothing to see here\n")
# Built by concatenation so this file itself never contains a matching literal.
set(fake "AKIA" "ABCDEFGHIJKLMNOPQRST")
string(CONCAT fake ${fake})
file(WRITE "${WORK_DIR}/dirty/a.txt" "aws_key = ${fake}\n")

execute_process(COMMAND ${CMAKE_COMMAND} -DSCAN_DIR=${WORK_DIR}/clean -P ${SCANNER}
                RESULT_VARIABLE clean_rc OUTPUT_QUIET ERROR_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -DSCAN_DIR=${WORK_DIR}/dirty -P ${SCANNER}
                RESULT_VARIABLE dirty_rc OUTPUT_QUIET ERROR_QUIET)
if(NOT clean_rc EQUAL 0)
  message(FATAL_ERROR "scanner flagged a clean tree")
endif()
if(dirty_rc EQUAL 0)
  message(FATAL_ERROR "scanner missed a planted secret")
endif()
message(STATUS "secret scanner self-test OK")
