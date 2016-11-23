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
    src/parser/ts_parse_while.c
    src/parser/ts_parse_do.c
    src/parser/ts_parse_of.c
    src/parser/ts_parse_in.c
    src/parser/ts_parse_json.c
    src/parser/ts_parse_scope.c
    src/parser/ts_parse_scope_or_json.c
    src/parser/ts_parse_array.c
    src/parser/ts_parse_string.c
    src/parser/ts_parse_call_arguments.c
    src/parser/ts_parse_interface.c
    src/parser/ts_borrow.c
    src/parser/ts_parse_arithmetic_operator.c
    src/parser/ts_parse_group.c
    src/parser/ts_parse_operator.c
    src/parser/ts_parse_namespace.c
    src/parser/ts_parse_spread.c
    src/parser/ts_parse_super.c
    src/parser.c
)

SET(
    OUTPUT_SOURCES
    src/output/ts_build_array_output.c
    src/output/ts_build_argument_output.c
    src/output/ts_build_arrow_output.c
    src/output/ts_build_break_output.c
    src/output/ts_build_borrow_output.c
    src/output/ts_build_call_arguments_output.c
    src/output/ts_build_caller_output.c
    src/output/ts_build_case_output.c
    src/output/ts_build_class_output.c
    src/output/ts_build_colon_output.c
    src/output/ts_build_condition_output.c
    src/output/ts_build_decorator_output.c
    src/output/ts_build_default_output.c
    src/output/ts_build_do_output.c
    src/output/ts_build_else_output.c
    src/output/ts_build_export_output.c
    src/output/ts_build_extends_output.c
    src/output/ts_build_for_output.c
    src/output/ts_build_function_output.c
    src/output/ts_build_group_output.c
    src/output/ts_build_if_output.c
    src/output/ts_build_implements_output.c
    src/output/ts_build_import_output.c
    src/output/ts_build_in_output.c
    src/output/ts_build_inline_comment_output.c
    src/output/ts_build_interface_output.c
    src/output/ts_build_json_output.c
    src/output/ts_build_local_variable_output.c
    src/output/ts_build_multiline_comment_output.c
    src/output/ts_build_namespace_output.c
    src/output/ts_build_new_output.c
    src/output/ts_build_of_output.c
    src/output/ts_build_operator_output.c
    src/output/ts_build_return_output.c
    src/output/ts_build_scope_output.c
    src/output/ts_build_string_output.c
    src/output/ts_build_semicolon_output.c
    src/output/ts_build_switch_output.c
    src/output/ts_build_while_output.c
    src/output/ts_buld_number_output.c
    src/output/ts_build_spread_output.c
    src/output/ts_build_super_output.c
    src/output/ts_output_header.c
    src/output/ts_output_footer.c
    src/output.c
)

SET(
    SOURCE_FILES
    src/file.c
    src/highlight.c
    src/os.c
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
      tests/parser/parse_extends.c
      tests/parser/parse_json.c
      tests/parser/parse_implements.c
      tests/parser/parse_while.c
      tests/parser/parse_do.c
      tests/parser/parse_super.c
      tests/parser/parse_spread.c
      tests/core/operators.c
      tests/core/sys.c
      tests/core/borrows.c
  )
ENDIF ()

SET(
    HEADER_FILES
    include/cts/log.h
    include/cts/sys.h
    include/cts/register.h
    include/cts/output.h
    include/cts/parser.h
    include/cts/os.h
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

INCLUDE_DIRECTORIES(include)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

ENABLE_TESTING()
FIND_PACKAGE(Check REQUIRED)
FIND_PACKAGE(Threads)
INCLUDE_DIRECTORIES(${CHECK_INCLUDE_DIRS})

MESSAGE(INFO " Compiler: ${CMAKE_C_COMPILER}")

IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE CACHE STRING "Debug")
ENDIF ()

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
  ADD_DEFINITIONS(-DDEBUG=1)
ELSE()
  ADD_DEFINITIONS(-DDEBUG=0)
ENDIF()


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
    SET("${CMAKE_C_FLAGS} -g -fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer")
  ENDIF ()

  IF (VALIDATE_ADDRESS MATCHES "TRUE")
    MESSAGE(INFO " Setting up address sanitizer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
  ENDIF ()
ENDIF ()

IF (Debug)
  ADD_DEFINITIONS(-DDebug=1)
ENDIF ()

ADD_DEFINITIONS(-Wstrict-prototypes -DUNICODE -D_UNICODE -D_MBCS)

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

ADD_TEST(NAME break_unexpected_token COMMAND ./cts -f examples/break/unexpected_token )
ADD_TEST(NAME break_valid_ts COMMAND ./cts -f examples/break/valid.ts )
ADD_TEST(NAME case_body_unexpected_end_of_stream COMMAND ./cts -f examples/case/body/unexpected_end_of_stream )
ADD_TEST(NAME case_conditions_unexpected_end_of_stream COMMAND ./cts -f examples/case/conditions/unexpected_end_of_stream )
ADD_TEST(NAME case_conditions_unexpected_token COMMAND ./cts -f examples/case/conditions/unexpected_token )
ADD_TEST(NAME case_valid_ts COMMAND ./cts -f examples/case/valid.ts )
ADD_TEST(NAME class_class_with_multiple_fields_ts COMMAND ./cts -f examples/class/class_with_multiple_fields.ts )
ADD_TEST(NAME class_class_with_multiple_methods_ts COMMAND ./cts -f examples/class/class_with_multiple_methods.ts )
ADD_TEST(NAME class_invalid_class_name COMMAND ./cts -f examples/class/invalid_class_name )
ADD_TEST(NAME class_missing_class_ending COMMAND ./cts -f examples/class/missing_class_ending )
ADD_TEST(NAME class_missing_class_name COMMAND ./cts -f examples/class/missing_class_name )
ADD_TEST(NAME class_multiple_super_class COMMAND ./cts -f examples/class/multiple_super_class )
ADD_TEST(NAME class_valid_ts COMMAND ./cts -f examples/class/valid.ts )
ADD_TEST(NAME cli_blank_ts COMMAND ./cts -f examples/cli/blank.ts )
ADD_TEST(NAME decorator_invalid_character COMMAND ./cts -f examples/decorator/invalid_character )
ADD_TEST(NAME decorator_missing_name COMMAND ./cts -f examples/decorator/missing_name )
ADD_TEST(NAME decorator_unexpected_end_of_stream COMMAND ./cts -f examples/decorator/unexpected_end_of_stream )
ADD_TEST(NAME decorator_unexpected_keyword COMMAND ./cts -f examples/decorator/unexpected_keyword )
ADD_TEST(NAME decorator_valid_ts COMMAND ./cts -f examples/decorator/valid.ts )
ADD_TEST(NAME do_valid_ts COMMAND ./cts -f examples/do/valid.ts )
ADD_TEST(NAME else_no_ending_bracket COMMAND ./cts -f examples/else/no_ending_bracket )
ADD_TEST(NAME else_no_if COMMAND ./cts -f examples/else/no_if )
ADD_TEST(NAME else_valid_ts COMMAND ./cts -f examples/else/valid.ts )
ADD_TEST(NAME export_unexpected_end_of_stream COMMAND ./cts -f examples/export/input/unexpected_end_of_stream)
ADD_TEST(NAME export_unexpected_new_line COMMAND ./cts -f examples/export/input/unexpected_new_line)
ADD_TEST(NAME export_valid_ts COMMAND ./cts -f examples/export/input/valid.ts)
ADD_TEST(NAME extends_valid_ts COMMAND ./cts -f examples/extends/valid.ts )
ADD_TEST(NAME for_in_unexpected_end_of_stream COMMAND ./cts -f examples/for/in/unexpected_end_of_stream )
ADD_TEST(NAME for_in_valid_ts COMMAND ./cts -f examples/for/in/valid.ts )
ADD_TEST(NAME for_let_unexpected_end_of_stream COMMAND ./cts -f examples/for/let/unexpected_end_of_stream )
ADD_TEST(NAME for_let_valid_ts COMMAND ./cts -f examples/for/let/valid.ts )
ADD_TEST(NAME for_of_unexpected_end_of_stream COMMAND ./cts -f examples/for/of/unexpected_end_of_stream )
ADD_TEST(NAME for_of_valid_ts COMMAND ./cts -f examples/for/of/valid.ts )
ADD_TEST(NAME functions_invalid_return_type COMMAND ./cts -f examples/functions/invalid_return_type )
ADD_TEST(NAME functions_invalid_return_type_name COMMAND ./cts -f examples/functions/invalid_return_type_name )
ADD_TEST(NAME functions_missing_arg_name COMMAND ./cts -f examples/functions/missing_arg_name )
ADD_TEST(NAME functions_missing_arg_type COMMAND ./cts -f examples/functions/missing_arg_type )
ADD_TEST(NAME functions_missing_arg_value COMMAND ./cts -f examples/functions/missing_arg_value )
ADD_TEST(NAME functions_missing_first_arg_name COMMAND ./cts -f examples/functions/missing_first_arg_name )
ADD_TEST(NAME functions_missing_return_type COMMAND ./cts -f examples/functions/missing_return_type )
ADD_TEST(NAME functions_valid_ts COMMAND ./cts -f examples/functions/valid.ts )
ADD_TEST(NAME if_no_args COMMAND ./cts -f examples/if/input/no_args)
ADD_TEST(NAME if_no_ending_bracket COMMAND ./cts -f examples/if/input/no_ending_bracket)
ADD_TEST(NAME if_valid_ts COMMAND ./cts -f examples/if/input/valid.ts)
ADD_TEST(NAME implements_valid_ts COMMAND ./cts -f examples/implements/valid.ts )
ADD_TEST(NAME import_go_up_valid_ts COMMAND ./cts -f examples/import/go_up/valid.ts )
ADD_TEST(NAME import_source_ts COMMAND ./cts -f examples/import/source.ts )
ADD_TEST(NAME import_valid_ts COMMAND ./cts -f examples/import/valid.ts )
ADD_TEST(NAME inline_comment_valid_ts COMMAND ./cts -f examples/inline_comment/valid.ts )
ADD_TEST(NAME interface_valid_ts COMMAND ./cts -f examples/interface/valid.ts )
ADD_TEST(NAME json_valid_ts COMMAND ./cts -f examples/json/valid.ts )
ADD_TEST(NAME multiline_comment_missing_ending COMMAND ./cts -f examples/multiline_comment/missing_ending )
ADD_TEST(NAME multiline_comment_valid_ts COMMAND ./cts -f examples/multiline_comment/valid.ts )
ADD_TEST(NAME namespace_valid_ts COMMAND ./cts -f examples/namespace/valid.ts )
ADD_TEST(NAME new_unexpected_colon COMMAND ./cts -f examples/new/unexpected_colon )
ADD_TEST(NAME new_unexpected_end_of_stream COMMAND ./cts -f examples/new/unexpected_end_of_stream )
ADD_TEST(NAME new_unexpected_new_line COMMAND ./cts -f examples/new/unexpected_new_line )
ADD_TEST(NAME new_valid_ts COMMAND ./cts -f examples/new/valid.ts )
ADD_TEST(NAME operators_valid_ts COMMAND ./cts -f examples/operators/valid.ts )
ADD_TEST(NAME return_valid_ts COMMAND ./cts -f examples/return/valid.ts )
ADD_TEST(NAME return_without_function COMMAND ./cts -f examples/return/without_function )
ADD_TEST(NAME scope_valid_ts COMMAND ./cts -f examples/scope/valid.ts )
ADD_TEST(NAME spread_valid_ts COMMAND ./cts -f examples/spread/input/valid.ts )
ADD_TEST(NAME super_valid_ts COMMAND ./cts -f examples/super/input/valid.ts )
ADD_TEST(NAME switch_body_unexpected_end_of_stream COMMAND ./cts -f examples/switch/body/unexpected_end_of_stream )
ADD_TEST(NAME switch_body_unexpected_token COMMAND ./cts -f examples/switch/body/unexpected_token )
ADD_TEST(NAME switch_conditions_unexpected_end_of_stream COMMAND ./cts -f examples/switch/conditions/unexpected_end_of_stream )
ADD_TEST(NAME switch_conditions_unexpected_token COMMAND ./cts -f examples/switch/conditions/unexpected_token )
ADD_TEST(NAME switch_head_unexpected_end_of_stream COMMAND ./cts -f examples/switch/head/unexpected_end_of_stream )
ADD_TEST(NAME switch_head_unexpected_token COMMAND ./cts -f examples/switch/head/unexpected_token )
ADD_TEST(NAME switch_valid_ts COMMAND ./cts -f examples/switch/valid.ts )
ADD_TEST(NAME test_ts COMMAND ./cts -f examples/test.ts )
ADD_TEST(NAME type_validation_ts COMMAND ./cts -f examples/type_validation.ts )
ADD_TEST(NAME unicode_ts COMMAND ./cts -f examples/unicode.ts )
ADD_TEST(NAME variables_const_name_reserved_word COMMAND ./cts -f examples/variables/const_name_reserved_word )
ADD_TEST(NAME variables_let_name_reserved_word COMMAND ./cts -f examples/variables/let_name_reserved_word )
ADD_TEST(NAME variables_missing_type COMMAND ./cts -f examples/variables/missing_type )
ADD_TEST(NAME variables_missing_value COMMAND ./cts -f examples/variables/missing_value )
ADD_TEST(NAME variables_no_name COMMAND ./cts -f examples/variables/no_name )
ADD_TEST(NAME variables_valid_ts COMMAND ./cts -f examples/variables/valid.ts )
ADD_TEST(NAME variables_var_name_reserved_word COMMAND ./cts -f examples/variables/var_name_reserved_word )
ADD_TEST(NAME while_valid_ts COMMAND ./cts -f examples/while/valid.ts )

IF (DEFINED TSC_TEST AND TSC_TEST MATCHES "1")
  ADD_TEST(NAME CheckTsc COMMAND ./cts_test)
ENDIF ()

SET_TESTS_PROPERTIES(
    break_unexpected_token

    case_body_unexpected_end_of_stream
    case_conditions_unexpected_end_of_stream
    case_conditions_unexpected_token

    class_invalid_class_name
    class_missing_class_ending
    class_missing_class_name
    class_multiple_super_class

    decorator_invalid_character
    decorator_missing_name
    decorator_unexpected_end_of_stream
    decorator_unexpected_keyword

    else_no_ending_bracket
    else_no_if

    export_unexpected_end_of_stream
    export_unexpected_new_line

    for_in_unexpected_end_of_stream

    for_let_unexpected_end_of_stream

    for_of_unexpected_end_of_stream

    functions_invalid_return_type
    functions_invalid_return_type_name
    functions_missing_arg_name
    functions_missing_arg_type
    functions_missing_arg_value
    functions_missing_first_arg_name
    functions_missing_return_type

    if_no_args
    if_no_ending_bracket

    multiline_comment_missing_ending

    new_unexpected_colon
    new_unexpected_end_of_stream
    new_unexpected_new_line

    return_without_function

    switch_body_unexpected_end_of_stream
    switch_body_unexpected_token
    switch_conditions_unexpected_end_of_stream
    switch_conditions_unexpected_token
    switch_head_unexpected_end_of_stream
    switch_head_unexpected_token

    variables_const_name_reserved_word
    variables_let_name_reserved_word
    variables_missing_type
    variables_missing_value
    variables_no_name

    variables_var_name_reserved_word

    PROPERTIES
    WILL_FAIL TRUE
)

INSTALL(
    TARGETS cts tsparser tsoutput
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    PUBLIC_HEADER DESTINATION include
)

INSTALL(DIRECTORY lib DESTINATION lib)


IF (CMAKE_BUILD_TYPE MATCHES "Debug")
  SET(LIBRARY_OUTPUT_PATH ${cts_SOURCE_DIR}/lib)
#ELSE()
#  SET(LIBRARY_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()
ADD_DEFINITIONS(-DLIBRARY_OUTPUT_PATH="${LIBRARY_OUTPUT_PATH}")