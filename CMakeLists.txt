cmake_minimum_required(VERSION 3.5)
project(tsc)

set(
  SOURCE_FILES
  src/log.c
  src/parser.c
  src/main.c
)

set(
  HEADER_FILES
  include/tsc/log.h
  include/tsc/parser.h
)

include_directories(include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

MESSAGE(INFO " Compiler: ${CMAKE_C_COMPILER}")

if (CMAKE_C_COMPILER MATCHES "clang")
  MESSAGE(INFO " Setting up memory sanitizer")
  SET(CMAKE_C_FLAGS "-g -fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer")
else()
  MESSAGE(INFO " Not clang, unable to sanitize memory")
  add_library(tsparser src/parser.c)
endif()

add_executable(tsc ${SOURCE_FILES})

if (CMAKE_C_COMPILER MATCHES "clang")
else()
  target_link_libraries(
    tsc
    tsparser
  )
endif()

FILE(COPY examples DESTINATION .)
