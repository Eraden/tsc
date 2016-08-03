#pragma once

#include <stddef.h>
#include <string.h>
#include <check_stdint.h>
#include <sys/types.h>
#include <check.h>

#include <tsc/sys.h>
#include <tsc/parser.h>
#include <tsc/output.h>

#include "./ck_wcs_assert.h"

TSFile build_ts_file(const char *fileName, const char *content);
void validate_ts_file(const TSFile tsFile, const int size, const TSTokenType validType);
