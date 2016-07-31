CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(tsc)

#include( CTest )

SET(
    PARSER_SOURCES
    src/parser/ts_parse_arrow.c
    src/parser/ts_parse_class.c
    src/parser/ts_parse_extends.c
    src/parser/ts_parse_implements.c
    src/parser/ts_parse_function.c
    src/parser/ts_parse_if.c
    src/parser/ts_parse_else.c
    src/parser/ts_parse_switch.c
    src/parser/ts_parse_case.c
    src/parser/ts_parse_default.c
    src/parser/ts_parse_return.c
    src/parser/ts_parse_local_variable.c
    src/parser/ts_parse_let.c
    src/parser/ts_parse_var.c
    src/parser/ts_parse_const.c
    src/parser/ts_parse_decorator.c
    src/parser/ts_parse_import.c
    src/parser/ts_parse_export.c
    src/parser/ts_parse_scope.c
    src/parser/ts_parse_new.c
    src/parser/ts_parse_inline_comment.c
    src/parser/ts_parse_multiline_comment.c
    src/parser.c
)

SET(
    OUTPUT_SOURCES
    src/output/ts_output_header.c
    src/output/ts_output_footer.c
    src/output/ts_output_var.c
    src/output/ts_output_if.c
    src/output/ts_output_else.c
    src/output/ts_output_function.c
    src/output/ts_output_return.c
    src/output/ts_output_class.c
    src/output/ts_output_export.c
    src/output.c
)

SET(
    SOURCE_FILES
    src/sys.c
    src/log.c
    src/register.c
)

SET(
    TEST_SOURCES
    ${SOURCE_FILES}
    tests/test.c
    tests/parser/classes.c
    tests/parser/variables.c
    tests/parser/functions.c
    tests/parser/if_conditions.c
    tests/parser/else_conditions.c
    tests/parser/return_keyword.c
    tests/parser/parse_exports.c
    tests/parser/inline_comment.c
    tests/parser/multiline_comment.c
    tests/parser/new_keyword.c
)

SET(
    HEADER_FILES
    include/tsc/log.h
    include/tsc/sys.h
    include/tsc/register.h
    include/tsc/output.h
    include/tsc/parser/ts_variable.h
    include/tsc/parser.h
)

include_directories(include)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

enable_testing()
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

MESSAGE(INFO " Compiler: ${CMAKE_C_COMPILER}")
MESSAGE(INFO " memory: ${MEMORY}")
MESSAGE(INFO " address: ${ADDRESS}")

IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE CACHE STRING "Debug")
ENDIF ()

IF (NOT EXISTS MEMORY AND MEMORY MATCHES "TRUE" AND NOT EXISTS ADDRESS)
  SET(MEMORY "TRUE")
ENDIF()

IF (NOT EXISTS MEMORY AND NOT EXISTS ADDRESS AND ADDRESS MATCHES "FALSE")
  SET(MEMORY "TRUE")
ENDIF()

IF (MEMORY MATCHES "TRUE")
  SET(ADDRESS "FALSE")
ENDIF()

IF (ADDRESS MATCHES "TRUE")
  SET(MEMORY "FALSE")
ENDIF()

IF (CMAKE_BUILD_TYPE MATCHES "Debug" AND CMAKE_C_COMPILER MATCHES "clang")
  IF (MEMORY MATCHES "TRUE")
    MESSAGE(INFO " Setting up memory sanitizer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer")
  ENDIF ()

  IF (ADDRESS MATCHES "TRUE")
    MESSAGE(INFO " Setting up address sanitizer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  ENDIF()
ENDIF ()

IF (Debug)
  ADD_DEFINITIONS(-DDebug=1)
ENDIF ()

add_library(tsparser ${PARSER_SOURCES})
add_library(tsoutput ${OUTPUT_SOURCES})

ADD_EXECUTABLE(tsc ${SOURCE_FILES} src/main.c)
ADD_EXECUTABLE(tsc_test ${TEST_SOURCES} tests/main.c)

target_link_libraries(
    tsc
    tsparser
    tsoutput
)

target_link_libraries(
    tsc_test
    tsparser
    tsoutput
    ${CHECK_LIBRARIES}
)

FILE(COPY examples DESTINATION .)

ADD_TEST(NAME CheckTsc COMMAND ./tsc_test)
