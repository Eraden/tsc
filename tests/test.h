#pragma once

#include <stddef.h>
#include <string.h>
#include <check_stdint.h>
#include <sys/types.h>
#include <check.h>

#include <tsc/sys.h>
#include <tsc/parser.h>
#include <tsc/output.h>

#define FAKE_FILE_BUFFER_SIZE 1024 * 1024

size_t fakeFileSize;
char *fakeFileBuffer;
FILE *fakeFile;

void initFakeFile();
void resetFakeFile();
void freeFakeFile();

TSFile build_ts_file(const char *fileName, const char *content);
void validate_ts_file(const TSFile tsFile, const int size, const TSTokenType validType);
