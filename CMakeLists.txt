cmake_minimum_required(VERSION 3.5)
project(tsc)

set(
  SOURCE_FILES
  src/log.c
  src/str.c
  src/output.c
  src/main.c
)

include_directories(include)

set(CMAKE_C_FLAGS -g)

add_library(tstypes src/types.c)
add_library(tsparse src/parse.c)
add_library(tsdump src/dump.c)

add_executable(tsc ${SOURCE_FILES})

target_link_libraries(
  tsc
  tstypes
  tsparse
  tsdump
)

FILE(COPY examples DESTINATION .)
