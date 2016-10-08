#pragma once

#include <stddef.h>
#include <string.h>
#include <check_stdint.h>
#include <sys/types.h>
#include <check.h>

#include <cts/sys.h>
#include <cts/parser.h>
#include <cts/output.h>

#include "support/ck_wcs_assert.h"
#include "support/ck_tsFile_sanity.h"
#include "support/ck_token_type.h"
#include "support/ck_token_eq.h"

TSParserToken *ANY;
TSParserToken *NUMBER;
TSParserToken *OBJECT;
TSParserToken *STRING_RETURN_TYPE;
TSParserToken *STRING;
TSParserToken *FUNCTION;
TSParserToken *ARRAY;
