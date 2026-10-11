# Proves the layering checker catches a Qt include inside core.
file(REMOVE_RECURSE "${WORK_DIR}")
file(MAKE_DIRECTORY "${WORK_DIR}/core")
file(WRITE "${WORK_DIR}/core/bad.hpp" "#include <QString>\n")
execute_process(COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${WORK_DIR} -P ${CHECKER}
                RESULT_VARIABLE rc OUTPUT_QUIET ERROR_QUIET)
if(rc EQUAL 0)
  message(FATAL_ERROR "layering checker missed a Qt include in core")
endif()
file(WRITE "${WORK_DIR}/core/bad.hpp" "#include <string>\n")
execute_process(COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${WORK_DIR} -P ${CHECKER}
                RESULT_VARIABLE rc2 OUTPUT_QUIET ERROR_QUIET)
if(NOT rc2 EQUAL 0)
  message(FATAL_ERROR "layering checker rejected a clean core")
endif()
message(STATUS "layering self-test OK")
