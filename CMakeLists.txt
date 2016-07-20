CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(tsc)

SET(
    SOURCE_FILES
    src/sys.c
    src/log.c
    src/register.c
    src/parser.c
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
    src/output.c
    src/output/ts_output_header.c
    src/output/ts_output_footer.c
    src/output/ts_output_var.c
    src/output/ts_output_if.c
    src/output/ts_output_else.c
    src/output/ts_output_function.c
    src/output/ts_output_return.c
    src/output/ts_output_class.c
    src/output/ts_output_export.c
    src/main.c
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

MESSAGE(INFO " Compiler: ${CMAKE_C_COMPILER}")

IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE CACHE STRING "Debug")
ENDIF()

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
  ADD_DEFINITIONS(-DDebug=1)
  IF (CMAKE_C_COMPILER MATCHES "clang")
    MESSAGE(INFO " Setting up memory sanitizer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer")
  ENDIF()
ENDIF()

IF (CMAKE_C_COMPILER MATCHES "clang")
ELSE()
  MESSAGE(INFO " Not clang, unable to sanitize memory")
  add_library(tsparser src/parser.c)
ENDIF()

ADD_EXECUTABLE(tsc ${SOURCE_FILES})

IF (CMAKE_C_COMPILER MATCHES "clang")
ELSE()
  target_link_libraries(
    tsc
    tsparser
  )
ENDIF()

FILE(COPY examples DESTINATION .)
