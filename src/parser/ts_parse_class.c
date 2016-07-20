#include <tsc/parser.h>
#include <tsc/register.h>

static void
__attribute__((visibility("hidden")))
TS_parse_class_method(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy,
    TSParserToken *bodyToken
) {
  unsigned char proceed = 1;
  const char *tok;
  TSFunctionData *methodData = bodyToken->data;
  TSClassParseFlag parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS;

  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of class header", tsFile, token);
    }

    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        *movedBy += strlen(tok);
        free((void *) tok);
        tsParseData->position += *movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case '{':
      case ')': {
        *movedBy += strlen(tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_BODY;
        break;
      }
      case ':': {
        parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE;
        break;
      }
      case '}': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS) {
          //
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
        } else /*if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE)*/ {
          u_long size = strlen(tok) + 1;
          if (methodData->returnType != NULL) size += strlen(methodData->returnType);
          char *newPointer = (char *) calloc(sizeof(char), size);
          if (methodData->returnType != NULL) strcpy(newPointer, methodData->returnType);
          if (methodData->returnType != NULL) free((void *) methodData->returnType);
          strcat(newPointer, tok);
          methodData->returnType = newPointer;
        }
        break;
      }
    }
  }
}

static void
__attribute__((visibility("hidden")))
TS_parse_class_body(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData,
    TSParserToken *token,
    u_long *movedBy
) {
  unsigned char proceed = 1;
  const char *tok;
  TSClassParseFlag parseFlag = TS_PARSE_CLASS_MEMBER_NAME;

  TSParserToken bodyToken;
  bodyToken.tokenType = TS_UNKNOWN;
  bodyToken.position = 0;
  bodyToken.character = 0;
  bodyToken.line = 0;
  bodyToken.visibility = TS_VISIBILITY_PRIVATE;
  bodyToken.children = NULL;
  bodyToken.childrenSize = 0;
  bodyToken.data = NULL;
  const char *name = NULL;
  const char *type = NULL;
  const char *value = NULL;

  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of class header", tsFile, token);
    }

    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        *movedBy += strlen(tok);
        free((void *) tok);
        tsParseData->position += *movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case '}': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      case '(': {
        bodyToken.tokenType = TS_CLASS_METHOD;

        TSFunctionData *methodData = (TSFunctionData *) calloc(sizeof(TSLocalVariableData), 1);
        methodData->name = name;
        methodData->returnType = type;
        bodyToken.data = methodData;

        TS_parse_class_method(tsFile, tsParseData, token, movedBy, &bodyToken);

        TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
        if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
        if (token->children != NULL) free(token->children);
        token->children = newPointer;
        token->children[token->childrenSize] = bodyToken;
        token->childrenSize += 1;

        bodyToken.tokenType = TS_UNKNOWN;
        bodyToken.position = 0;
        bodyToken.character = 0;
        bodyToken.line = 0;
        bodyToken.visibility = TS_VISIBILITY_PRIVATE;
        bodyToken.children = NULL;
        bodyToken.childrenSize = 0;
        bodyToken.data = NULL;
        name = NULL;
        type = NULL;
        value = NULL;
        parseFlag = TS_PARSE_CLASS_MEMBER_NAME;

        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '=': {
        *movedBy += strlen(tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_VALUE;
        break;
      }
      case ':': {
        *movedBy += strlen(tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_TYPE;
        break;
      }
      case ';': {
        TSLocalVariableData *fieldData = (TSLocalVariableData *) calloc(sizeof(TSLocalVariableData), 1);
        fieldData->name = name;
        fieldData->type = type;
        fieldData->value = value;
        bodyToken.data = fieldData;
        bodyToken.tokenType = TS_CLASS_FIELD;

        TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
        if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
        if (token->children != NULL) free(token->children);
        token->children = newPointer;
        token->children[token->childrenSize] = bodyToken;
        token->childrenSize += 1;

        bodyToken.tokenType = TS_UNKNOWN;
        bodyToken.position = 0;
        bodyToken.character = 0;
        bodyToken.line = 0;
        bodyToken.visibility = TS_VISIBILITY_PRIVATE;
        bodyToken.children = NULL;
        bodyToken.childrenSize = 0;
        bodyToken.data = NULL;
        name = NULL;
        type = NULL;
        value = NULL;
        parseFlag = TS_PARSE_CLASS_MEMBER_NAME;

        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      default: {
        if (strcmp(tok, "public") == 0) {
          bodyToken.visibility = TS_VISIBILITY_PUBLIC;

        } else if (strcmp(tok, "protected") == 0) {
          bodyToken.visibility = TS_VISIBILITY_PROTECTED;

        } else if (strcmp(tok, "private") == 0) {
          bodyToken.visibility = TS_VISIBILITY_PRIVATE;

        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_NAME) {
          name = TS_clone_string(tok);
          parseFlag = TS_PARSE_CLASS_MEMBER_TYPE;

        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_TYPE) {
          u_long size = strlen(tok);
          if (type != NULL) size += strlen(type);
          char *newPointer = (char *) calloc(sizeof(char), size + 1);
          if (type != NULL) memcpy(newPointer, type, size);
          strcat(newPointer, tok);
          if (type == NULL) free((void *) type);
          type = newPointer;

        } else /*if (parseFlag == TS_PARSE_CLASS_MEMBER_VALUE)*/ {
          u_long size = strlen(tok);
          if (value != NULL) size += strlen(value);
          char *newPointer = (char *) calloc(sizeof(char), size + 1);
          if (value != NULL) memcpy(newPointer, value, size);
          strcat(newPointer, tok);
          if (value == NULL) free((void *) value);
          value = newPointer;
        }
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
    }
  }
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
  const char *tok;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Unexpected end of class header", tsFile, token);
    }
    switch (tok[0]) {
      case ' ': {
        *movedBy += strlen(tok);
        free((void *) tok);
        break;
      }
      case '\n': {
        *movedBy += strlen(tok);
        free((void *) tok);
        tsParseData->position += *movedBy;
        tsParseData->character = 0;
        tsParseData->line += 1;
        *movedBy = 0;
        break;
      }
      case '{': {
        *movedBy += strlen(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        tsParseData->position += *movedBy;
        tsParseData->character += *movedBy;
        tsParseData->line += 1;
        tsParseData->token = tok;
        *movedBy = 0;
        TSParserToken t = TS_parse_ts_token(tsFile, tsParseData);
        switch (t.tokenType) {
          case TS_EXTENDS: {
            if (data->parentClass != NULL) {
              ts_token_syntax_error("Unexpected parent name. Class can have only one parent", tsFile, token);
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
        free((void *) tok);
      }
    }
  }
}

const TSParserToken TS_parse_class(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("class");
  u_long movedBy = strlen(tsParseData->token);

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

  const char *tok;
  unsigned char proceed = 1;
  while (proceed) {
    tok = (const char *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      ts_token_syntax_error("Missing class name", tsFile, &token);
    }
    if (tok[0] != ' ') {
      if (!TS_name_is_valid(tok)) {
        ts_token_syntax_error("Invalid class name", tsFile, &token);
      }
      data->name = TS_clone_string(tok);
      proceed = 0;
    }
    free((void *) tok);
  }

  TS_register_class(tsFile, token);

  TS_parse_class_head(tsFile, tsParseData, &token, &movedBy);
  TS_parse_class_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("class");
  return token;
}