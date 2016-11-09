#include <cts/parser.h>
#include <cts/register.h>

static void
TS_parse_local_variable_done(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSVariableParseFlag parseFlag
) {
  TSParserToken *token = tsParseData->parentTSToken;
  switch (parseFlag) {
    case TS_PARSE_VARIABLE_NAME: {
      ts_token_syntax_error(
          (wchar_t *) L"Missing variable name",
          tsFile,
          token
      );
      break;
    }
    case TS_PARSE_VARIABLE_TYPE: {
      ts_token_syntax_error(
          (wchar_t *) L"Expect variable type but none provided",
          tsFile,
          token
      );
      break;
    }
    case TS_PARSE_VARIABLE_VALUE: {
      ts_token_syntax_error(
          (wchar_t *) L"Expect variable default value but none provided",
          tsFile,
          token
      );
      break;
    }
    case TS_PARSE_VARIABLE_NONE: {
    }
    default: {
      break;
    }
  }
}

static void
TS_parse_local_variable_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;
  TSVariableParseFlag parseFlag = TS_PARSE_VARIABLE_NAME;
  token->name = NULL;
  token->children = calloc(sizeof(TSParserToken *), 2);
  token->childrenSize = 2;
  TSParserToken *value = NULL;
  TSParserToken *type = NULL;
  token->children[TS_VARIABLE_TYPE] = TS_find_type(tsFile->file, (const wchar_t *) L"any");
  token->children[TS_VARIABLE_VALUE] = TS_find_type(NULL, (const wchar_t *) L"undefined");

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_parse_local_variable_done(tsFile, tsParseData, parseFlag);
      break;
    }
    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        free((void *) tok);
        ts_token_syntax_error(
            (wchar_t *) L"New line in variable definition is not allowed!",
            tsFile,
            token
        );
        proceed = FALSE;
        break;
      }
      case L',': {
        TS_parse_local_variable_done(tsFile, tsParseData, parseFlag);
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        switch (token->tokenType) {
          case TS_VAR: {
            TS_put_back(tsFile->stream, (volatile const int *) (const wchar_t *) L"var ");
            break;
          }
          case TS_LET: {
            TS_put_back(tsFile->stream, (volatile const int *) (const wchar_t *) L"let ");
            break;
          }
          case TS_CONST: {
            TS_put_back(tsFile->stream, (const wchar_t *) L"const ");
            break;
          }
          default: {
            break;
          }
        }
        proceed = FALSE;
        break;
      }
      case L':': {
        parseFlag = TS_PARSE_VARIABLE_TYPE;
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'=': {
        parseFlag = TS_PARSE_VARIABLE_VALUE;
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L';': {
        proceed = FALSE;
        TS_put_back(tsFile->stream, tok);
        free((void *) tok);
        break;
      }
      default: {
        if (wcscmp(tok, (const wchar_t *) L"in") == 0 || wcscmp(tok, (const wchar_t *) L"of") == 0) {
          TS_put_back(tsFile->stream, tok);
          TS_parse_local_variable_done(tsFile, tsParseData, parseFlag);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        if (parseFlag == TS_PARSE_VARIABLE_NAME) {
          TS_log_to_file((wchar_t *) L"Setting name of local variable\n");
          if (TS_is_keyword(tok)) {
            free((void *) tok);
            ts_token_syntax_error(
                (const wchar_t *) L"Local variable name cannot use reserved word", tsFile, token
            );
            proceed = FALSE;
            break;
          }
          token->name = TS_clone_string(tok);
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          parseFlag = TS_PARSE_VARIABLE_NONE;
          TS_log_to_file((wchar_t *) L"    Local variable current name: '%ls'\n", token->name);

        } else if (parseFlag == TS_PARSE_VARIABLE_VALUE) {
          tsParseData->token = tok;
          value = TS_parse_ts_token(tsFile, tsParseData);
          if (value && value->tokenType == TS_UNKNOWN) {
            TS_type_from_string(tsFile, value);
          }
          if (value) {
            token->children[TS_VARIABLE_VALUE] = value;
          }
          parseFlag = TS_PARSE_VARIABLE_NONE;
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

        } else if (parseFlag == TS_PARSE_VARIABLE_TYPE) {
          type = TS_find_type(tsFile->file, tok);
          if (type) {
            token->children[TS_VARIABLE_TYPE] = type;
          }

          parseFlag = TS_PARSE_VARIABLE_NONE;
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

        } else {
          free((void *) tok);
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "local variable");
          proceed = FALSE;
        }
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_var(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_VAR, tsParseData)
    TS_parse_local_variable_body(tsFile, tsParseData);
  TS_TOKEN_END(TS_VAR)
}

TSParserToken *
TS_parse_let(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_LET, tsParseData)
    TS_parse_local_variable_body(tsFile, tsParseData);
  TS_TOKEN_END(TS_LET)
}

TSParserToken *
TS_parse_const(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CONST, tsParseData)
    TS_parse_local_variable_body(tsFile, tsParseData);
  TS_TOKEN_END(TS_CONST)
}

void
TS_free_var(
    const TSParserToken *token
) {
  TS_free_children_from(token, 1);

  if (token->name) free((void *) token->name);
  free((void *) token);
}

void
TS_free_let(
    const TSParserToken *token
) {
  TS_free_var(token);
}

void
TS_free_const(
    const TSParserToken *token
) {
  TS_free_var(token);
}
