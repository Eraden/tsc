CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
PROJECT(cts)

SET(
    PARSER_SOURCES
    src/parser/ts_parse_argument.c
    src/parser/ts_parse_condition.c
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
    src/parser/ts_parse_break.c
    src/parser/ts_parse_return.c
    src/parser/ts_parse_local_variable.c
    src/parser/ts_parse_decorator.c
    src/parser/ts_parse_import.c
    src/parser/ts_parse_export.c
    src/parser/ts_parse_new.c
    src/parser/ts_parse_inline_comment.c
    src/parser/ts_parse_multiline_comment.c
    src/parser/ts_parse_caller.c
    src/parser/ts_parse_for.c
    src/parser/ts_parse_of.c
    src/parser/ts_parse_in.c
    src/parser/ts_parse_json.c
    src/parser/ts_parse_scope.c
    src/parser/ts_parse_scope_or_json.c
    src/parser/ts_parse_array.c
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

IF (DEFINED TSC_TEST AND TSC_TEST MATCHES "1")
  SET(
      TEST_SOURCES
      ${SOURCE_FILES}
      tests/test.c
      tests/parser/parse_variables.c
      tests/parser/parse_function.c
      tests/parser/parse_if.c
      tests/parser/parse_else.c
      tests/parser/classes.c
      tests/parser/class/multiple_instance_methods.c
      tests/parser/class/multiple_instance_fields.c
      tests/parser/parse_return.c
      tests/parser/parse_exports.c
      tests/parser/parse_inline_comment.c
      tests/parser/parse_multiline_comment.c
      tests/parser/parse_new.c
      tests/parser/parse_decorator.c
      tests/parser/parse_switch.c
      tests/parser/parse_break.c
      tests/parser/for/parse_for_let.c
      tests/parser/for/parse_for_of.c
      tests/parser/for/parse_for_in.c
  )
ENDIF ()

SET(
    HEADER_FILES
    include/cts/log.h
    include/cts/sys.h
    include/cts/register.h
    include/cts/output.h
    include/cts/parser.h
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

ADD_EXECUTABLE(cts ${SOURCE_FILES} src/main.c)

IF (DEFINED TSC_TEST AND TSC_TEST MATCHES "1")
  ADD_EXECUTABLE(cts_test ${TEST_SOURCES} tests/main.c)
ENDIF ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

target_link_libraries(
    cts
    tsparser
    tsoutput
    ${CMAKE_THREAD_LIBS_INIT}
)

IF (DEFINED TSC_TEST AND TSC_TEST MATCHES "1")
  target_link_libraries(
      cts_test
      tsparser
      tsoutput
      ${CHECK_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
  )
ENDIF ()

FILE(COPY examples DESTINATION .)

ADD_TEST(NAME cli_invalid_parameters COMMAND ./cts -)
ADD_TEST(NAME cli_invalid_file_source_parameters COMMAND ./cts -f)

ADD_TEST(NAME cli_blank_with_blank_file COMMAND ./cts -f ./examples/cli/blank.ts)
ADD_TEST(NAME cli_blank_with_blank_file_and_level_info COMMAND ./cts -f ./examples/cli/blank.ts -l info)
ADD_TEST(NAME cli_blank_with_blank_file_and_level_log COMMAND ./cts -f ./examples/cli/blank.ts -l log)
ADD_TEST(NAME cli_blank_with_blank_file_and_level_debug COMMAND ./cts -f ./examples/cli/blank.ts -l debug)
ADD_TEST(NAME cli_blank_with_blank_file_and_level_off COMMAND ./cts -f ./examples/cli/blank.ts -l off)
ADD_TEST(NAME cli_eval_source COMMAND ./cts -c 'var\ a')

ADD_TEST(NAME Functions COMMAND ./cts -f ./examples/functions/valid.ts)

ADD_TEST(NAME If_blocks COMMAND ./cts -f ./examples/if/valid.ts)
ADD_TEST(NAME If_else_blocks COMMAND ./cts -f ./examples/else/valid.ts)

ADD_TEST(NAME Variables COMMAND ./cts -f ./examples/variables/valid.ts)
ADD_TEST(NAME Var_name_is_reserved_name COMMAND ./cts -f ./examples/variables/var_name_reserved_word)
ADD_TEST(NAME Let_name_is_reserved_name COMMAND ./cts -f ./examples/variables/let_name_reserved_word)
ADD_TEST(NAME Const_name_is_reserved_name COMMAND ./cts -f ./examples/variables/const_name_reserved_word)

ADD_TEST(NAME Decorator_valid COMMAND ./cts -f ./examples/decorator/valid.ts)
ADD_TEST(NAME Decorator_invalid_character COMMAND ./cts -f ./examples/decorator/invalid_character)
ADD_TEST(NAME Decorator_missing_name COMMAND ./cts -f ./examples/decorator/missing_name)
ADD_TEST(NAME Decorator_unexpected_end_of_stream COMMAND ./cts -f ./examples/decorator/unexpected_end_of_stream)
ADD_TEST(NAME Decorator_unexpected_keyword COMMAND ./cts -f ./examples/decorator/unexpected_keyword)

ADD_TEST(NAME Class_fields COMMAND ./cts -f ./examples/class/class_with_multiple_fields.ts)
ADD_TEST(NAME Class_methods COMMAND ./cts -f ./examples/class/class_with_multiple_methods.ts)
ADD_TEST(NAME Class_with_invalid_name COMMAND ./cts -f ./examples/class/invalid_class_name)
ADD_TEST(NAME Class_with_missing_ending COMMAND ./cts -f ./examples/class/missing_class_ending)

ADD_TEST(NAME for_with_iteration COMMAND ./cts -f ./examples/for/let/valid.ts)
ADD_TEST(NAME for_iteration_with_unexpected_end_of_stream COMMAND ./cts -f ./examples/for/let/unexpected_end_of_stream)

ADD_TEST(NAME for_with_in COMMAND ./cts -f ./examples/for/in/valid.ts)
ADD_TEST(NAME for_of_with_unexpected_end_of_stream COMMAND ./cts -f ./examples/for/in/unexpected_end_of_stream)

ADD_TEST(NAME for_with_of COMMAND ./cts -f ./examples/for/of/valid.ts)
ADD_TEST(NAME for_in_with_unexpected_end_of_stream COMMAND ./cts -f ./examples/for/of/unexpected_end_of_stream)

ADD_TEST(NAME switch_head_unexpected_end_of_stream COMMAND ./cts -f ./examples/switch/head/unexpected_end_of_stream)
ADD_TEST(NAME switch_head_unexpected_token COMMAND ./cts -f ./examples/switch/head/unexpected_token)
ADD_TEST(NAME switch_body_unexpected_end_of_stream COMMAND ./cts -f ./examples/switch/body/unexpected_end_of_stream)
ADD_TEST(NAME switch_body_unexpected_token COMMAND ./cts -f ./examples/switch/body/unexpected_token)
ADD_TEST(NAME switch_conditions_unexpected_end_of_stream COMMAND ./cts -f ./examples/switch/conditions/unexpected_end_of_stream)
ADD_TEST(NAME switch_conditions_unexpected_token COMMAND ./cts -f ./examples/switch/conditions/unexpected_token)
ADD_TEST(NAME case_body_unexpected_end_of_stream COMMAND ./cts -f ./examples/case/body/unexpected_end_of_stream)
ADD_TEST(NAME case_conditions_unexpected_end_of_stream COMMAND ./cts -f ./examples/case/conditions/unexpected_end_of_stream)
ADD_TEST(NAME case_conditions_unexpected_token COMMAND ./cts -f ./examples/case/conditions/unexpected_token)

IF (DEFINED TSC_TEST AND TSC_TEST MATCHES "1")
  ADD_TEST(NAME CheckTsc COMMAND ./cts_test)
ENDIF ()

SET_TESTS_PROPERTIES(
    cli_invalid_parameters
    cli_invalid_file_source_parameters

    Var_name_is_reserved_name
    Let_name_is_reserved_name
    Const_name_is_reserved_name
    Class_with_invalid_name
    Class_with_missing_ending
    Decorator_invalid_character
    Decorator_missing_name
    Decorator_unexpected_end_of_stream
    Decorator_unexpected_keyword

    switch_head_unexpected_end_of_stream
    switch_head_unexpected_token
    switch_body_unexpected_end_of_stream
    switch_body_unexpected_token
    switch_conditions_unexpected_end_of_stream
    switch_conditions_unexpected_token

    case_body_unexpected_end_of_stream
    case_conditions_unexpected_end_of_stream
    case_conditions_unexpected_token

    for_iteration_with_unexpected_end_of_stream
    for_of_with_unexpected_end_of_stream
    for_in_with_unexpected_end_of_stream

    PROPERTIES
    WILL_FAIL TRUE
)
