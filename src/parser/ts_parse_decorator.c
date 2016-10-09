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

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "decorator call");
      free((void *) tok);
      break;

    } else if (TS_is_keyword(tok)) {
      TS_UNEXPECTED_TOKEN(tsFile, token, tok, "decorator call");
      free((void *) tok);
      break;
    }

    switch (tok[0]) {
      case L'(': {
        TS_MOVE_BY(tsParseData, tok);
        if (name == NULL) {
          ts_token_syntax_error(
              (const wchar_t *) L"Missing decorator name",
              tsFile,
              token
          );
        }
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      case '\n':
      case '\t':
      case '\r':
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        if (!name) {
          ts_token_syntax_error(
              (const wchar_t *) L"Unexpected white character in decorator call function name",
              tsFile,
              token
          );
        }
        proceed = FALSE;
        break;
      }
      default: {
        wchar_t *newPointer = TS_join_strings(name, tok);
        if (name) free(name);
        name = newPointer;
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
    }
  }

  if (tsFile->sanity == TS_FILE_VALID) {
    if (name == NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Missing decorator call function name",
          tsFile,
          token
      );
    } else if (TS_name_is_valid(name) != TRUE) {
      ts_token_syntax_error(
          (const wchar_t *) L"Invalid characters in decorator call",
          tsFile,
          token
      );
    }
  }

  return name;
}

static void
__attribute__((visibility("hidden")))
__attribute__((section("parse-decorator")))
TS_parse_decorator_arguments(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "decorator call arguments");
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L')': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *argument = TS_parse_argument(tsFile, tsParseData);
        free((void *) tok);
        TS_push_child(token, argument);
      }
    }
  }
}

TSParserToken *
__attribute__((section("parse-decorator")))
TS_parse_decorator(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_DECORATOR, tsParseData)

    wchar_t *name = TS_parse_decorator_name(tsFile, tsParseData, token);

    if (name) {
      token->name = TS_clone_string(name);
      free((void *) name);
    }

    if (tsFile->sanity == TS_FILE_VALID) TS_parse_decorator_arguments(tsFile, tsParseData);

  TS_TOKEN_END(TS_DECORATOR)
}

void
TS_free_decorator(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}
