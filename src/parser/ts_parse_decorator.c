#include <cts/parser.h>

static wchar_t *
__attribute__((visibility("hidden")))
__attribute__((section("parse-decorator")))
TS_parse_decorator_name(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;
  wchar_t *name = NULL;
  u_long len = 0;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "decorator call");
      break;

    } else if (TS_is_keyword(tok)) {
      free((void *) tok);
      TS_UNEXPECTED_TOKEN(tsFile, token, tok, "decorator call");
      break;
    }
    len = wcslen(tok);
    switch (tok[0]) {
      case L'(': {
        if (name == NULL) {
          free((void *) tok);
          ts_token_syntax_error(
              (const wchar_t *) L"Missing decorator name",
              tsFile,
              token
          );
          proceed = FALSE;
          break;
        }
        proceed = FALSE;
        tsParseData->character += len;
        tsParseData->position += len;
        free((void *) tok);
        break;
      }
      case '\n':
      case '\t':
      case '\r':
      case L' ': {
        free((void *) tok);
        if (name) free(name);
        ts_token_syntax_error(
            (const wchar_t *) L"Unexpected white character in decorator call name",
            tsFile,
            token
        );
        proceed = FALSE;
        break;
      }
      default: {
        u_long size = len + TS_STRING_END;
        if (name != NULL) size += wcslen(name);

        wchar_t *newPointer = calloc(sizeof(wchar_t), size);
        if (name) {
          wcscpy(newPointer, name);
          free(name);
        }
        wcscat(newPointer, tok);
        name = newPointer;
        tsParseData->character += len;
        tsParseData->position += len;
        free((void *) tok);
        break;
      }
    }
  }

  if (TS_name_is_valid(name) != TRUE) {
    ts_token_syntax_error(
        (const wchar_t *) L"Invalid characters in decorator call",
        tsFile,
        token
    );
  }

  return name;
}

TSParserToken *
__attribute__((section("parse-decorator")))
TS_parse_decorator(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("decorator");
  TS_MOVE_BY(tsParseData, tsParseData->token);

  TSParserToken *token = TS_build_parser_token(TS_DECORATOR, tsParseData);
  wchar_t *name = TS_parse_decorator_name(tsFile, tsParseData, token);

  if (name) {
    token->name = TS_clone_string(name);
    free((void *) name);
  }

  // parse
  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("decorator");
  return token;
}

void
TS_free_decorator(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}
