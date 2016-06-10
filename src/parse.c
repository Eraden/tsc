#include <tsc/parse.h>

static void parseTSFunctionArguments(TSParseContext *context, TSFunction *fn);
static short unsigned int parseTSFunctionArgument(TSParseContext *context, TSFunction *fn);

static void validateName(TSParseContext *context, const char *name) {
  const unsigned long int nameSize = strlen(name);
  const char *invalidTokens = ":{}()|!+-*/\\@=,";
  const size_t invalidTokensSize = strlen(invalidTokens);
  for (unsigned long int i = 0; i < nameSize; i++) {
    char c = name[i];
    for (size_t j = 0; j < invalidTokensSize; j++) {
      char t = invalidTokens[j];
      if (c == t) {
        ERR("Invalid token '%c' in name: '%s'\n", c, name);
        PANIC(context, INVALID_TOKEN);
      }
    }
  }
}

//#################################################################################
// Parse
//#################################################################################

void skipTSWhite(TSParseContext *context) {
  char last = context->lastChar;
  if (last != ' ' && last != '\n') return;
  while (1) {
    getTSToken(context); // ' '
    last = context->lastChar;
    if (last != ' ' && last != '\n') {
      FLOG("---   Skipped white to: '%s'\n", context->currentToken->content);
      break;
    }
  }
}

void parseTSTokens(TSParseContext *context) {
  while (1) {
    getTSToken(context);
    if (context->currentToken->content == NULL) {
      break;
    }
    parseTSToken(context);
  }
}

void parseTSToken(TSParseContext *context) {
  const char *token = context->currentToken->content;
  if (strcmp(token, CLASS) == 0) {
    parseTSClass(context);
  } else if (strcmp(token, FUNCTION) == 0) {
    parseTSFunction(context);
  } else if (strcmp(token, CONST) == 0) {
  } else if (strcmp(token, LET) == 0) {
  } else if (strcmp(token, VAR) == 0) {
  } else if (context->lastChar == DECORATOR) {
    parseTSDecorator(context);
  } else if (strcmp(token, EXPORT) == 0) {
    parseTSExport(context);
  } else if (strcmp(token, DEFAULT) == 0) {
  }
}

void parseTSExport(TSParseContext *context) {
  POS(TSExport, context)
  TSExport *export = newTSExport();
  getTSToken(context);
  skipTSWhite(context);
  if (strcmp(context->currentToken->content, DEFAULT) == 0) {
//    FLOG("  %s\n", "recognized as default export");
    if (context->hasDefaultExport == 1) {
      ERR("%s\n", "File can has only one default export!");
      PANIC(context, MULTIPLE_DEFAULT_EXPORT_ERROR);
    }
    context->hasDefaultExport = 1;
    export->asDefault = 1;
    getTSToken(context);
    skipTSWhite(context);
  } else {
//    FLOG("  %s\n", "recognized as named export");
  }
  const char *type = cloneString(context->currentToken->content);
//  FLOG("Export type '%s'\n", type);
  parseTSToken(context);
  // TODO
  if (strcmp(type, CLASS) == 0) {
//    FLOG("  %s\n", "processing class export...");
    export->type = TSExportType_Class;
    unsigned long int size = context->globalClassesSize;
    if (size == 0) {
      ERR("No classes found! Classes size: %lu\n", 0);
      freeTSExport(export);
      return;
    }
    TSClass *last = context->globalClasses[size - 1];
    export->meta = (void*) last;
  } else if (strcmp(type, FUNCTION) == 0) {
    export->type = TSExportType_Method;
  } else if (strcmp(type, CONST) == 0) {
    export->type = TSExportType_Const;
  } else if (strcmp(type, LET) == 0) {
    export->type = TSExportType_Let;
  } else if (strcmp(type, VAR) == 0) {
    export->type = TSExportType_Var;
  }
  // FLOG("%s\n", "    replacing old exports...");
  // FLOG("+ coll addess: %p\n", context->exports);
  TSExport **new = pushTSExport(context->exports, context->exportsSize, export);
  if (context->exports != NULL) free(context->exports);
  context->exports = new;
  context->exportsSize += 1;
  // FLOG("+ coll addess: %p\n", context->exports);
  free((void *) type);
}

/**
 * TSFunction
 */
static short unsigned int parseTSFunctionArgument(TSParseContext *context, TSFunction *fn) {
  TSArgument *arg = newTSArgument();
  getTSToken(context);
  skipTSWhite(context);

  if (strcmp(context->currentToken->content, TS_PUBLIC) == 0) {
    ERR("%s\n", "Function should not have this context!");
    PANIC(context, INVALID_TOKEN);
  } else if (strcmp(context->currentToken->content, TS_PROTECTED) == 0) {
    ERR("%s\n", "Function should not have this context!");
    PANIC(context, INVALID_TOKEN);
  } else if (strcmp(context->currentToken->content, TS_PRIVATE) == 0) {
    ERR("%s\n", "Function should not have this context!");
    PANIC(context, INVALID_TOKEN);
  }

  validateName(context, context->currentToken->content);
  const char *name = context->currentToken->content;
  if (name && strlen(name) > 4 && name[0] == '.' && name[1] == '.' && name[2] == '.') {
    size_t s = strlen(name) - 3;
    arg->name = (char *) malloc(s);
    for (size_t i = 0; i < s; i++) arg->name[i] = name[i+3];
    arg->name[s] = 0;
    arg->isRest = 1;
  } else {
    arg->name = cloneString(name);
  }

  getTSToken(context);
  skipTSWhite(context);

  if (context->lastChar == ':') {
    getTSToken(context);
    skipTSWhite(context);
    validateName(context, context->currentToken->content);
    arg->type = cloneString(context->currentToken->content);

    getTSToken(context);
    skipTSWhite(context);
    if (context->lastChar == ')')
      return 0;
  }

  if (context->lastChar == '=') {
    getTSToken(context);
    skipTSWhite(context);

    arg->value = cloneString(context->currentToken->content);
    while (1) {
      getTSToken(context);
      if (context->lastChar == ')') {
        break;
      } else {
        CONCAT(arg->value, context->currentToken->content);
      }
    }
  }

  TSArgument **new = pushTSArgument(fn->arguments, fn->argumentsSize, arg);
  if (fn->arguments != NULL) free(fn->arguments);
  fn->arguments = new;
  fn->argumentsSize += 1;

  if (context->lastChar == ')')
    return 0;

  if (context->lastChar != ',') {
    getTSToken(context);
    skipTSWhite(context);
    return 1;
  }

  return 1;
}

static void parseTSFunctionArguments(TSParseContext *context, TSFunction *fn) {
  if (context->lastChar == ')') {
    FLOG("    %s\n", "No more arguments");
    return;
  } else if (context->lastChar == ',') {
    if (fn->argumentsSize == 0) {
      ERR("%s\n", "Invalid token while parsing function arguments, unexpected ','!");
      PANIC(context, INVALID_TOKEN);
    }
  }

  while(parseTSFunctionArgument(context, fn));
}

void parseTSFunction(TSParseContext *context) {
  POS(TSFunction, context);
  FLOG("Found %s\n", "function");
  getTSToken(context); // name
  skipTSWhite(context);
  TSFunction *fn = newTSFunction();
  validateName(context, context->currentToken->content);
  fn->name = cloneString(context->currentToken->content);
  FLOG("    fn name: '%s'\n", fn->name);
  getTSToken(context); // args
  skipTSWhite(context);
  if (context->lastChar != '(') {
    ERR("%s\n", "Invalid end of function, expect (");
    PANIC(context, INVALID_TOKEN);
  }
  parseTSFunctionArguments(context, fn);
  if (context->lastChar != '{') {
    getTSToken(context);
    skipTSWhite(context);
  }
  int bracketCount = 1;
  while (bracketCount) {
    getTSToken(context);

    if (context->currentToken == NULL) {
      ERR("%s\n", "Expect function body!");
      PANIC(context, INVALID_TOKEN);
    }
    if (context->lastChar == '}') {
      bracketCount -= 1;
      if (bracketCount > 0 && context->currentToken->content != NULL && fn->body != NULL) {
        CONCAT(fn->body, context->currentToken->content);
      } else break;
    } else {
      if (context->lastChar == '{')
        bracketCount += 1;
      CONCAT(fn->body, context->currentToken->content);
    }
  }
  FLOG("  function body:\n%s\n", fn->body);
  TSFunction **new = pushTSFunction(context->globalFunctions, context->globalFunctionsSize, fn);
  free(context->globalFunctions);
  context->globalFunctionsSize += 1;
  context->globalFunctions = new;
}

void parseTSMethod(TSParseContext *context) {
}

void parseTSClass(TSParseContext *context) {
  if (context == NULL) {
    return;
  }
  POS(TSClass, context)
  TSClass *class = newTSClass();
  getTSToken(context); // move to white
  skipTSWhite(context);
  class->name = cloneString(context->currentToken->content);
  // FLOG("  class->name '%s' = '%s'\n", class->name, context->currentToken->content)
  getTSToken(context); // move to white
  skipTSWhite(context);
  if (strcmp(context->currentToken->content, EXTENDS) == 0) {
    // FLOG("  %s\n", "found class parent...");
    getTSToken(context); // move to white
    skipTSWhite(context);
    class->parent = cloneString(context->currentToken->content);
  }
  // FLOG("+ coll address: %p\n", context->globalClasses);
  TSClass **new = pushTSClass(context->globalClasses, context->globalClassesSize, class);
  if (context->globalClasses) free((void*) context->globalClasses);
  context->globalClasses = new;
  if (context->decoratorsStackSize) {
    class->decorators = context->decoratorsStack;
    class->decoratorsSize = context->decoratorsStackSize;
    context->decoratorsStackSize = 0;
    context->decoratorsStack = NULL;
  }
  context->globalClassesSize += 1;
  // FLOG("+ coll address: %p\n", context->globalClasses);
}

void parseTSDecorator(TSParseContext *context) {
  POS(Decorator, context)
  char c = context->content[context->position];
  if (!isalnum(c)) {
    PANIC(context, INVALID_DECORATOR_ANNOTATION);
  }
  TSDecorator *dec = newTSDecorator();
  getTSToken(context);
  const char *name = cloneString(context->currentToken->content);
  dec->name = name;

  getTSToken(context);
  if (context->lastChar != '(') {
    context->position -= 1;
    return;
  }
  TSArgument *arg = NULL;
  while (context->lastChar != ')') {
    getTSToken(context);
    if (arg == NULL) arg = newTSArgument();
    if (context->lastChar == ')') break;
    else if (context->lastChar == ',') {
      arg = newTSArgument();
      TSArgument **new = pushTSArgument(dec->arguments, dec->argumentsSize, arg);
      free(dec->arguments);
      dec->arguments = new;
      dec->argumentsSize += 1;
    } else {
      if (arg->value == NULL) {
        arg->value = cloneString(context->currentToken->content);
      } else if (context->currentToken->content != NULL) {
        CONCAT(arg->value, context->currentToken->content);
      }
    }
  }
  if (arg != NULL) {
    TSArgument **new = pushTSArgument(dec->arguments, dec->argumentsSize, arg);
    free(dec->arguments);
    dec->arguments = new;
    dec->argumentsSize += 1;
  }
  TSDecorator **new = pushTSDecorator(context->decoratorsStack, context->decoratorsStackSize, dec);
  if (context->decoratorsStack) free(context->decoratorsStack);
  context->decoratorsStack = new;
  context->decoratorsStackSize += 1;
}

static void consumeString(TSParseContext *context) {
  char begin = context->lastChar;
  char prev = 0;
  context->lastChar = begin;
  while (1) {
    char current = context->content[context->position];
    char *new = appendChar(context->currentToken->content, current);
    free((void*) context->currentToken->content);
    context->currentToken->content = new;
    if (current == begin && prev != '\\') {
      context->position += 1;
      return;
    }
    context->position += 1;
    prev = current;
  }
}

void getTSToken(TSParseContext *context) {
  if (context == NULL) {
    ERR("[ERROR] %s\n", "No TSParseContext given!");
    exit(NO_PARSE_CONTEXT);
  }
  if (context->content == NULL) {
    ERR("[ERROR] %s\n", "TSParseContext content is missing!");
    exit(PARSE_CONTEXT_NO_CONTENT);
  }
  if (context->currentToken != NULL) {
    freeTSToken(context->currentToken);
  }
  context->currentToken = newTSToken();
  char c = 0, prev = 0;
  while (1) {
    if (context->position == context->size)
      break;
    c = context->content[context->position];
    prev = context->lastChar;
    context->lastChar = c;
    context->position += 1;
    context->character += 1;
    if (c == '\n') context->line += 1;
    switch (c) {
      case '\'':
      case '"':
        {
          if (context->currentToken->content != NULL) {
            if (context->position > 0) context->position -= 1;
            if (context->character > 0) context->character -= 1;
            context->lastChar = prev;
            // FLOG("-- Current token content: '%s'\n", context->currentToken->content);
            return;
          }
          char *new = appendChar(context->currentToken->content, c);
          free((void*) context->currentToken->content);
          context->currentToken->content = new;
          consumeString(context);
          // FLOG("-- Current token content: '%s'\n", context->currentToken->content);
          return;
        }
      case '\n':
        {
          if (context->currentToken->content != NULL) {
            if (context->position > 0) context->position -= 1;
            if (context->character > 0) context->character -= 1;
            context->lastChar = prev;
            return;
          }
          char *new = appendChar(context->currentToken->content, c);
          free((void*) context->currentToken->content);
          context->currentToken->content = new;
          context->character = 0;
          // FLOG("-- Current token content: '%s'\n", context->currentToken->content);
          return;
        }
      case ' ':
      case ')':
      case '(':
      case '{':
      case '}':
      case '@':
      case ':':
      case ',':
      case '=':
        {
          if (context->currentToken->content != NULL) {
            if (context->position > 0) context->position -= 1;
            if (context->character > 0) context->character -= 1;
            context->lastChar = prev;
            // FLOG("-- Current token content: '%s'\n", context->currentToken->content);
            return;
          }
          char *new = appendChar(context->currentToken->content, c);
          free((void*) context->currentToken->content);
          context->currentToken->content = new;
          // FLOG("-- Current token content: '%s'\n", context->currentToken->content);
          return;
        }
      default:
        {
          char *new = appendChar(context->currentToken->content, c);
          free((void *) context->currentToken->content);
          context->currentToken->content = new;
        }
    }
  }
}

TSParseContext *parseFile(const char *file) {
  TSParseContext *context = newTSParseContext(file);
  parseTSTokens(context);
  return context;
}
