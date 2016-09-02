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
    src/parser/ts_parse_decorator.c
    src/parser/ts_parse_import.c
    src/parser/ts_parse_export.c
    src/parser/ts_parse_scope.c
    src/parser/ts_parse_new.c
    src/parser/ts_parse_inline_comment.c
    src/parser/ts_parse_multiline_comment.c
    src/parser/ts_parse_caller.c
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
    tests/parser/variables.c
    tests/parser/functions.c
    tests/parser/if_conditions.c
    tests/parser/else_conditions.c
    tests/parser/classes.c
    tests/parser/class/multiple_instance_methods.c
    tests/parser/class/multiple_instance_fields.c
    tests/parser/return_keyword.c
    tests/parser/parse_exports.c
    tests/parser/inline_comment.c
    tests/parser/multiline_comment.c
    tests/parser/new_keyword.c
    tests/parser/parse_decorator.c
)

SET(
    HEADER_FILES
    include/tsc/log.h
    include/tsc/sys.h
    include/tsc/register.h
    include/tsc/output.h
    include/tsc/parser.h
    tests/support/ck_wcs_assert.h
)

include_directories(include)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

enable_testing()
find_package(Check REQUIRED)
find_package(Threads)
include_directories(${CHECK_INCLUDE_DIRS})

MESSAGE(INFO " Compiler: ${CMAKE_C_COMPILER}")

IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE CACHE STRING "Debug")
ENDIF ()

MESSAGE(INFO " pre memory validation: ${VALIDATE_MEMORY}")
MESSAGE(INFO " pre address validation: ${VALIDATE_ADDRESS}")

IF (NOT DEFINED VALIDATE_MEMORY AND NOT DEFINED VALIDATE_ADDRESS)
  MESSAGE(INFO " NOT DEFINED VALIDATE_MEMORY AND NOT DEFINED VALIDATE_ADDRESS")
  SET(VALIDATE_MEMORY "TRUE")
  SET(VALIDATE_ADDRESS "FALSE")
ELSEiF (DEFINED VALIDATE_MEMORY AND VALIDATE_MEMORY MATCHES "TRUE" AND NOT DEFINED VALIDATE_ADDRESS)
  MESSAGE(INFO " DEFINED VALIDATE_MEMORY AND VALIDATE_MEMORY MATCHES \"TRUE\" AND NOT DEFINED VALIDATE_ADDRESS")
  SET(VALIDATE_MEMORY "TRUE")
  SET(VALIDATE_ADDRESS "FALSE")
ELSEIF (NOT DEFINED VALIDATE_MEMORY AND DEFINED VALIDATE_ADDRESS AND VALIDATE_ADDRESS MATCHES "FALSE")
  MESSAGE(INFO " NOT DEFINED VALIDATE_MEMORY AND DEFINED VALIDATE_ADDRESS AND VALIDATE_ADDRESS MATCHES \"FALSE\"")
  SET(VALIDATE_MEMORY "TRUE")
  SET(VALIDATE_ADDRESS "FALSE")
ELSEIF (VALIDATE_MEMORY MATCHES "TRUE")
  MESSAGE(INFO " VALIDATE_MEMORY MATCHES \"TRUE\"")
  SET(VALIDATE_MEMORY "TRUE")
  SET(VALIDATE_ADDRESS "FALSE")
ELSEIF (VALIDATE_ADDRESS MATCHES "TRUE")
  MESSAGE(INFO " VALIDATE_ADDRESS MATCHES \"TRUE\"")
  SET(VALIDATE_MEMORY "FALSE")
  SET(VALIDATE_ADDRESS "TRUE")
ENDIF ()

MESSAGE(INFO " post memory validation: ${VALIDATE_MEMORY}")
MESSAGE(INFO " post address validation: ${VALIDATE_ADDRESS}")

IF (CMAKE_BUILD_TYPE MATCHES "Debug" AND CMAKE_C_COMPILER MATCHES "clang")
  IF (VALIDATE_MEMORY MATCHES "TRUE")
    MESSAGE(INFO " Setting up memory sanitizer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer")
  ENDIF ()

  IF (VALIDATE_ADDRESS MATCHES "TRUE")
    MESSAGE(INFO " Setting up address sanitizer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
  ENDIF ()
ENDIF ()

IF (Debug)
  ADD_DEFINITIONS(-DDebug=1)
ENDIF ()

ADD_DEFINITIONS(-Wstrict-prototypes -Wall -DUNICODE -D_UNICODE -D_MBCS)

add_library(
    tsparser
    ${PARSER_SOURCES}
)
add_library(
    tsoutput
    ${OUTPUT_SOURCES}
)

ADD_EXECUTABLE(tsc ${SOURCE_FILES} src/main.c)
ADD_EXECUTABLE(tsc_test ${TEST_SOURCES} tests/main.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

target_link_libraries(
    tsc
    tsparser
    tsoutput
    ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(
    tsc_test
    tsparser
    tsoutput
    ${CHECK_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

FILE(COPY examples DESTINATION .)
ADD_TEST(NAME Functions COMMAND ./tsc -f ./examples/functions/valid.ts)

ADD_TEST(NAME If_blocks COMMAND ./tsc -f ./examples/if/valid.ts)
ADD_TEST(NAME If_else_blocks COMMAND ./tsc -f ./examples/else/valid.ts)

ADD_TEST(NAME Variables COMMAND ./tsc -f ./examples/variables/valid.ts)
ADD_TEST(NAME Var_name_is_reserved_name COMMAND ./tsc -f ./examples/variables/var_name_reserved_word)
ADD_TEST(NAME Let_name_is_reserved_name COMMAND ./tsc -f ./examples/variables/let_name_reserved_word)
ADD_TEST(NAME Const_name_is_reserved_name COMMAND ./tsc -f ./examples/variables/const_name_reserved_word)

ADD_TEST(NAME Decorator_valid COMMAND ./tsc -f ./examples/decorator/valid.ts)
ADD_TEST(NAME Decorator_invalid_character COMMAND ./tsc -f ./examples/decorator/invalid_character)
ADD_TEST(NAME Decorator_missing_name COMMAND ./tsc -f ./examples/decorator/missing_name)
ADD_TEST(NAME Decorator_unexpected_end_of_stream COMMAND ./tsc -f ./examples/decorator/unexpected_end_of_stream)
ADD_TEST(NAME Decorator_unexpected_keyword COMMAND ./tsc -f ./examples/decorator/unexpected_keyword)

ADD_TEST(NAME Class_fields COMMAND ./tsc -f ./examples/class/class_with_multiple_fields.ts)
ADD_TEST(NAME Class_methods COMMAND ./tsc -f ./examples/class/class_with_multiple_methods.ts)
ADD_TEST(NAME Class_with_invalid_name COMMAND ./tsc -f ./examples/class/invalid_class_name)
ADD_TEST(NAME Class_with_missing_ending COMMAND ./tsc -f ./examples/class/missing_class_ending)

ADD_TEST(NAME CheckTsc COMMAND ./tsc_test)

SET_TESTS_PROPERTIES(
    Var_name_is_reserved_name
    Let_name_is_reserved_name
    Const_name_is_reserved_name
    Class_with_invalid_name
    Class_with_missing_ending
    Decorator_invalid_character
    Decorator_missing_name
    Decorator_unexpected_end_of_stream
    Decorator_unexpected_keyword
    PROPERTIES
    WILL_FAIL TRUE
)
