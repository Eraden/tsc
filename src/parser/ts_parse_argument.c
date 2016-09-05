#include <tsc/parser.h>

TSParserToken *
TS_parse_argument(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  const wchar_t *tok;
  u_long movedBy = 0;

  TSLocalVariableData *data = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
  data->name = NULL;
  data->value = NULL;
  data->type = NULL;

  TSParserToken *argument = TS_build_parser_token(TS_ARGUMENT, tsParseData);
  argument->data = data;

  volatile unsigned char proceed = 1;
  TSParseArgumentFlag parseFlag = TS_PARSE_ARG_NAME;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing argument", tsFile, argument);
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        tsParseData->line += 1;
        tsParseData->character = 0;
        tsParseData->position += movedBy;
        movedBy = 0;
        free((void *) tok);
        break;
      }
      case L')': {
        if (parseFlag == TS_PARSE_ARG_VALUE && data->value == NULL)
          ts_token_syntax_error((wchar_t *) L"Value for argument is missing", tsFile, argument);

        if (parseFlag == TS_PARSE_ARG_TYPE && data->type == NULL)
          ts_token_syntax_error((wchar_t *) L"Type for argument is missing", tsFile, argument);

        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case L'=': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (data->name == NULL) {
          ts_token_syntax_error((wchar_t *) L"Assigning to argument without name", tsFile, argument);
        }
        parseFlag = TS_PARSE_ARG_VALUE;
        break;
      }
      case L',': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (parseFlag == TS_PARSE_ARG_VALUE && data->value == NULL)
          ts_token_syntax_error((wchar_t *) L"Value for argument is missing", tsFile, argument);

        if (parseFlag == TS_PARSE_ARG_TYPE && data->type == NULL)
          ts_token_syntax_error((wchar_t *) L"Value for argument is missing", tsFile, argument);

        if (data->name == NULL)
          ts_token_syntax_error((wchar_t *) L"Declared argument as next but previous has no name", tsFile, argument);

        proceed = 0;

        break;
      }
      case L':': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (data->name == NULL)
          ts_token_syntax_error((wchar_t *) L"Unexpected argument type definition. Argument has no name", tsFile, argument);

        if (data->type != NULL)
          ts_token_syntax_error((wchar_t *) L"Unexpected argument type definition. Type was already declared", tsFile, argument);
        parseFlag = TS_PARSE_ARG_TYPE;

        break;
      }
      default: {
        if (wcscmp(tok, (wchar_t *) L"private") == 0) {
          argument->visibility = TS_VISIBILITY_PRIVATE;

        } else if (wcscmp(tok, (wchar_t *) L"protected") == 0) {
          argument->visibility = TS_VISIBILITY_PROTECTED;

        } else if (wcscmp(tok, (wchar_t *) L"public") == 0) {
          argument->visibility = TS_VISIBILITY_PUBLIC;

        } else if (parseFlag == TS_PARSE_ARG_NAME) {
          u_long size = wcslen(tok) + 1;
          if (data->name != NULL) size += wcslen(data->name);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (data->name != NULL) wcscpy(newPointer, data->name);
          if (data->name != NULL) free((void *) data->name);
          wcscat(newPointer, tok);
          data->name = newPointer;

        } else if (parseFlag == TS_PARSE_ARG_VALUE) {
          u_long size = wcslen(tok) + TS_STRING_END;
          if (data->value != NULL)
            size = size + wcslen(data->value) + wcslen((const wchar_t *) L" ");
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (data->value != NULL) wcscpy(newPointer, data->value);
          if (data->value != NULL) wcscat(newPointer, (const wchar_t *) L" ");
          if (data->value) free((void *) data->value);
          wcscat(newPointer, tok);
          data->value = newPointer;

        } else /*if (parseFlag == TS_PARSE_ARG_TYPE)*/ {
          data->type = TS_clone_string(tok);
        }

        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  tsParseData->parentTSToken = argument->parent;

  return argument;
}

void TS_free_argument(const TSParserToken *token) {
  if (token->variableData) {
    if (token->variableData->name) free((void *) token->variableData->name);
    if (token->variableData->type) free((void *) token->variableData->type);
    if (token->variableData->value) free((void *) token->variableData->value);
    free(token->variableData);
  }
  free((void *) token);
}