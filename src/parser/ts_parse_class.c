#include <tsc/parser.h>
#include <tsc/register.h>

static const TSParserToken
__attribute__((visibility("hidden")))
TS_parse_class_method_argument(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  u_long movedBy = 0;
  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  TSFunctionParseFlag parseFlag = TS_PARSE_FN_ARG_NAME;

  TSParserToken argument;

  TSLocalVariableData *argumentData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
  argumentData->name = NULL;
  argumentData->value = NULL;
  argumentData->type = NULL;

  argument.tokenType = TS_VAR;
  argument.data = argumentData;
  argument.visibility = TS_VISIBILITY_SCOPE;
  argument.children = NULL;
  argument.childrenSize = 0;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of class method argument", tsFile, &argument);
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        movedBy += wcslen(tok);
        free((void *) tok);
        tsParseData->position += movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        movedBy = 0;
        break;
      }
      case L')': {
        proceed = 0;
        TS_put_back(tsParseData->stream, tok);
        free((void *) tok);
        break;
      }
      case L',': {
        proceed = 0;
        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'=': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (argumentData->name == NULL) {
          ts_token_syntax_error((wchar_t *) L"Assigning to argument without name", tsFile, &argument);
        }
        parseFlag = TS_PARSE_FN_ARG_VALUE;
        break;
      }
      case L':': {
        movedBy += wcslen(tok);
        free((void *) tok);

        if (argumentData->name == NULL) {
          ts_token_syntax_error((wchar_t *) L"Declared argument type but argument has no name", tsFile, &argument);
        }
        if (argumentData->type != NULL) {
          ts_token_syntax_error((wchar_t *) L"Missing argument type after typing symbol", tsFile, &argument);
        }
        parseFlag = TS_PARSE_FN_ARG_TYPE;

        break;
      }
      default: {
        if (wcscmp(tok, (const wchar_t *) L"private") == 0) {
          argument.visibility = TS_VISIBILITY_PRIVATE;
        } else if (wcscmp(tok, (const wchar_t *) L"protected") == 0) {
          argument.visibility = TS_VISIBILITY_PROTECTED;
        } else if (wcscmp(tok, (const wchar_t *) L"public") == 0) {
          argument.visibility = TS_VISIBILITY_PUBLIC;
        } else if (parseFlag == TS_PARSE_FN_ARG_NAME) {
          u_long size = wcslen(tok) + 1;
          if (argumentData->name != NULL) size += wcslen(argumentData->name);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (argumentData->name != NULL) wcscpy(newPointer, argumentData->name);
          if (argumentData->name != NULL) free((void *) argumentData->name);
          wcscat(newPointer, tok);
          argumentData->name = newPointer;
        } else if (parseFlag == TS_PARSE_FN_ARG_VALUE) {
          u_long size = wcslen(tok) + 1;
          if (argumentData->value != NULL)
            size = size + wcslen(argumentData->value) + wcslen((const wchar_t *) L" ");
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (argumentData->value != NULL)
            wcscpy(newPointer, argumentData->value);
          if (argumentData->value != NULL)
            wcscat(newPointer, (const wchar_t *) L" ");
          if (argumentData->value) free((void *) argumentData->value);
          wcscat(newPointer, tok);
          argumentData->value = newPointer;
        } else /*if (parseFlag == TS_PARSE_FN_ARG_TYPE)*/ {
          argumentData->type = TS_clone_string(tok);
        }

        movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->character += movedBy;
  tsParseData->position += movedBy;
  return argument;
}

static void
__attribute__((visibility("hidden")))
TS_parse_class_method(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData,
    u_long *movedBy,
    TSParserToken *bodyToken
) {
  unsigned char proceed = 1;
  const wchar_t *tok;

  TSFunctionData *methodData = bodyToken->data;

  TSClassParseFlag parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of class header", tsFile, bodyToken);
    }

    switch (tok[0]) {
      case L' ': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        tsParseData->position += *movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case L'{':
      case L')': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_BODY;
        break;
      }
      case L':': {
        parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE;
        break;
      }
      case L';': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'}': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS) {
          tsParseData->token = tok;
          tsParseData->character += *movedBy;
          tsParseData->position += *movedBy;
          *movedBy = 0;

          TSParserToken arg = TS_parse_class_method_argument(tsFile, tsParseData);

          TSParserToken *newPointer = calloc(sizeof(TSParserToken), methodData->argumentsSize + 1);

          if (methodData->arguments != NULL)
            memcpy(newPointer, methodData->arguments, sizeof(TSParserToken) * methodData->argumentsSize);
          if (methodData->arguments != NULL) free(methodData->arguments);
          methodData->arguments = newPointer;
          methodData->arguments[methodData->argumentsSize] = arg;
          methodData->argumentsSize += 1;

          *movedBy += wcslen(tok);
          free((void *) tok);
        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_BODY) {
          tsParseData->token = tok;
          tsParseData->character += *movedBy;
          tsParseData->position += *movedBy;
          *movedBy = 0;

          TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);

          TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), bodyToken->childrenSize + 1);
          if (bodyToken->children != NULL)
            memcpy(newPointer, bodyToken->children, sizeof(TSParserToken) * bodyToken->childrenSize);
          if (bodyToken->children != NULL) free(bodyToken->children);
          bodyToken->children = newPointer;
          bodyToken->children[bodyToken->childrenSize] = t;
          bodyToken->childrenSize += 1;

          *movedBy += wcslen(tok);
          free((void *) tok);
        } else /*if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE)*/ {
          u_long size = wcslen(tok) + TS_STRING_END;
          if (methodData->returnType != NULL) size += wcslen(methodData->returnType);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
          if (methodData->returnType != NULL) wcscpy(newPointer, methodData->returnType);
          if (methodData->returnType != NULL) free((void *) methodData->returnType);
          wcscat(newPointer, tok);
          methodData->returnType = newPointer;

          *movedBy += wcslen(tok);
          free((void *) tok);
        }
        break;
      }
    }
  }
}

static TSParserToken
__attribute__((visibility("hidden")))
TS_parse_class_member(
    TSFile *tsFile,
    TSParseData *tsParseData,
    u_long *movedBy
) {
  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  TSClassParseFlag parseFlag = TS_PARSE_CLASS_MEMBER_NAME;

  TSParserToken bodyToken;
  bodyToken.tokenType = TS_UNKNOWN;
  bodyToken.position = tsParseData->position;
  bodyToken.character = tsParseData->character;
  bodyToken.line = tsParseData->line;
  bodyToken.visibility = TS_VISIBILITY_PRIVATE;
  bodyToken.children = NULL;
  bodyToken.childrenSize = 0;
  bodyToken.data = NULL;

  const wchar_t *name = NULL;
  const wchar_t *type = NULL;
  const wchar_t *value = NULL;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of class member", tsFile, &bodyToken);
    }

    switch (tok[0]) {
      case L' ': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        tsParseData->position += *movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case L'(': {
        if (name == NULL) {
          ts_token_syntax_error((wchar_t *) L"Missing class method name", tsFile, &bodyToken);
        }

        bodyToken.tokenType = TS_CLASS_METHOD;

        TSFunctionData *methodData = (TSFunctionData *) calloc(sizeof(TSFunctionData), 1);
        methodData->arguments = NULL;
        methodData->argumentsSize = 0;
        methodData->name = name;
        methodData->returnType = type;

        bodyToken.data = methodData;

        TS_parse_class_method(tsFile, tsParseData, movedBy, &bodyToken);

        proceed = 0;
        break;
      }
      case L'=': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        parseFlag = TS_PARSE_CLASS_MEMBER_VALUE;
        break;
      }
      case L':': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        parseFlag = TS_PARSE_CLASS_MEMBER_TYPE;
        break;
      }
      case L';': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        TSLocalVariableData *fieldData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
        fieldData->name = name;
        fieldData->type = type;
        fieldData->value = value;
        bodyToken.data = fieldData;
        bodyToken.tokenType = TS_CLASS_FIELD;

        proceed = 0;
        break;
      }
      default: {
        if (wcscmp(tok, (const wchar_t *) L"public") == 0) {
          bodyToken.visibility = TS_VISIBILITY_PUBLIC;

        } else if (wcscmp(tok, (const wchar_t *) L"protected") == 0) {
          bodyToken.visibility = TS_VISIBILITY_PROTECTED;

        } else if (wcscmp(tok, (const wchar_t *) L"private") == 0) {
          bodyToken.visibility = TS_VISIBILITY_PRIVATE;

        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_NAME) {
          wchar_t *tmp = (wchar_t *) calloc(sizeof(wchar_t), wcslen(tok) + TS_STRING_END);
          wcscpy(tmp, tok);
          name = tmp;
          parseFlag = TS_PARSE_CLASS_MEMBER_TYPE;

        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_TYPE) {
          u_long size = wcslen(tok);
          if (type != NULL) size += wcslen(type);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size + TS_STRING_END);
          if (type != NULL) wcscpy(newPointer, type);
          wcscat(newPointer, tok);
          if (type == NULL) free((void *) type);
          type = newPointer;

        } else /*if (parseFlag == TS_PARSE_CLASS_MEMBER_VALUE)*/ {
          u_long size = wcslen(tok);
          if (value != NULL) size += wcslen(value);
          wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size + TS_STRING_END);
          if (value != NULL) wcscpy(newPointer, value);
          wcscat(newPointer, tok);
          if (value == NULL) free((void *) value);
          value = newPointer;
        }
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
    }
  }
  return bodyToken;
}

static void
__attribute__((visibility("hidden")))
TS_parse_class_head(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
  TSClassData *data = (TSClassData *) token->data;
  unsigned char proceed = 1;
  const wchar_t *tok;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of class header", tsFile, token);
    }
    switch (tok[0]) {
      case L' ': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        tsParseData->position += *movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case L'{': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        tsParseData->position += *movedBy;
        tsParseData->character += *movedBy;
        tsParseData->token = tok;
        *movedBy = 0;

        TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
        free((void *) tok);

        switch (t.tokenType) {
          case TS_EXTENDS: {
            if (data->parentClass != NULL) {
              ts_token_syntax_error((wchar_t *) L"Unexpected parent name. Class can have only one parent", tsFile, token);
            }
            data->parentClass = t.data;
            break;
          }
          case TS_IMPLEMENTS: {
            break;
          }
          case TS_UNKNOWN: {
            free(t.data);
            break;
          }
          default: {
            break;
          }
        }
      }
    }
  }
}

static void
__attribute__((visibility("hidden")))
TS_parse_class_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {

  volatile unsigned char proceed = 1;
  const wchar_t *tok;

  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Unexpected end of class member", tsFile, token);
    }

    switch (tok[0]) {
      case L' ': {
        *movedBy += wcslen(tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        tsParseData->position += *movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case L'}': {
        *movedBy += wcslen(tok);
        free((void *) tok);

        proceed = 0;
        break;
      }
      default: {
        tsParseData->token = tok;
        tsParseData->character += *movedBy;
        tsParseData->position += *movedBy;
        *movedBy = 0;

        TSParserToken child = TS_parse_ts_token(tsFile, tsParseData);
        free((void *) tok);

        if (child.tokenType == TS_UNKNOWN) {
          TS_put_back(tsParseData->stream, child.data);
          free(child.data);

          child = TS_parse_class_member(tsFile, tsParseData, movedBy);
        }

        TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
        if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
        if (token->children != NULL) free(token->children);
        token->children = newPointer;
        token->children[token->childrenSize] = child;
        token->childrenSize += 1;

        break;
      }
    }
  }
}

const TSParserToken TS_parse_class(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("class");
  u_long movedBy = wcslen(tsParseData->token);

  TSClassData *data = (TSClassData *) calloc(sizeof(TSClassData), 1);
  TSParserToken token;
  token.tokenType = TS_CLASS;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = data;

  data->name = NULL;
  data->parentClass = NULL;
  data->implementsInterfaces = NULL;
  data->implementsInterfacesSize = 0;

  const wchar_t *tok;
  unsigned char proceed = 1;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error((wchar_t *) L"Missing class name", tsFile, &token);
    }
    if (tok[0] != ' ') {
      if (!TS_name_is_valid(tok)) {
        ts_token_syntax_error((wchar_t *) L"Invalid class name", tsFile, &token);
      }
      data->name = TS_clone_string(tok);
      proceed = 0;
    }
    free((void *) tok);
  }

//  TS_register_class(tsFile, token);

  TS_parse_class_head(tsFile, tsParseData, &token, &movedBy);
  TS_parse_class_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("class");
  return token;
}

void TS_free_class(const TSParserToken token) {
  TS_free_children(token);
  TSClassData *data = token.data;
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->parentClass != NULL) free((void *) data->parentClass);
  if (data->implementsInterfaces != NULL) free((void *) data->implementsInterfaces);
  free(data);
}

void TS_free_class_field(const TSParserToken token) {
  TS_free_var(token);
}

void TS_free_class_method(const TSParserToken token) {
  TS_free_function(token);
}
