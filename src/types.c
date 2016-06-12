#include <tsc/types.h>

static unsigned long long int readFile(char *path, char **buffer, unsigned long long int *size);

//#################################################################################
// IO
//#################################################################################

static unsigned long long int readFile(char *path, char **buffer, unsigned long long int *size) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    ERR("File '%s' not found!", path);
    exit(FILE_NOT_FOUND_ERROR);
  }
  fseek(file, 0L, SEEK_END);
  *size = (unsigned long long int) ftell(file);
  const unsigned int c = sizeof(char);
  *buffer = (char*) malloc( (*size) * c + 1 );
  if ((*buffer) == NULL) return 0;
  memset(*buffer, 0, (*size) * c + 1);
  rewind(file);
  fread(*buffer, c, (*size), file);
  return *size;
}

//#################################################################################
// TSToken
//#################################################################################

TSToken *newTSToken() {
  TSToken *token = (TSToken *) calloc(TSToken_SIZE, 1);
  CHECK_ALLOC_FAILED(token, TSToken)
  token->content = NULL;
  return token;
}

void freeTSToken(TSToken *token) {
  if (token == NULL) return;
  if (token->content != NULL) {
    free((void *) token->content);
    token->content = NULL;
  }
  free((void *) token);
}

//#################################################################################
// TSParseContext
//#################################################################################

TSParseContext *newTSParseContext(char *file_path) {
  char *b = (char *) calloc(sizeof(char), 1024);
  char *buffer = NULL;
  TSParseContext *context = (TSParseContext *) calloc(TSParseContext_SIZE, 1);
  CHECK_ALLOC_FAILED(context, TSParseContext)
  context->file = realpath(file_path, b);
  if (context->file == NULL) {
    ERR("File '%s' not found!\n", file_path);
    exit(FILE_NOT_FOUND_ERROR);
  }
  context->content = NULL;
  context->hasDefaultExport = 0;
  context->line = 0;
  context->position = 0;
  context->character = 0;
  context->currentToken = NULL;
  context->size = 0;
  context->lastChar = '\0';
  context->globalClasses = NULL;
  context->globalClassesSize = 0;
  context->globalFunctions = NULL;
  context->globalFunctionsSize = 0;
  context->decoratorsStack = NULL;
  context->decoratorsStackSize = 0;
  context->exports = NULL;
  context->exportsSize = 0;
  readFile(context->file, &buffer, &context->size);
  context->content = buffer;
  return context;
}

void freeTSParseContext(TSParseContext *context) {
  if (context == NULL) return;
  if (context->content != NULL) {
    free((void*)context->content);
    context->content = NULL;
  }
  if (context->currentToken != NULL) {
    freeTSToken(context->currentToken);
    context->currentToken = NULL;
  }
  free((void*)context);
}

//#################################################################################
// TSExport
//#################################################################################

TSExport *newTSExport() {
  TSExport *export = (TSExport *) calloc(TSExport_SIZE, 1);
  CHECK_ALLOC_FAILED(export, TSExport)
  export->type = TSExportType_Undefined;
  export->asDefault = 0;
  export->meta = NULL;
  return export;
}

void freeTSExport(TSExport *export) {
  free((void*)export);
}

PUSH_TS(TSExport)

//#################################################################################
// TSArgument
//#################################################################################

TSArgument *newTSArgument() {
  TSArgument *arg = (TSArgument *) calloc(TSArgument_SIZE, 1);
  CHECK_ALLOC_FAILED(arg, TSArgument)
  arg->name = NULL;
  arg->type = NULL;
  arg->value = NULL;
  arg->modifier = TSAccessModifier_Undefined;
  arg->isRest = 0;
  return arg;
}

void freeTSArgument(TSArgument *arg) {
  if (arg == NULL) return;
  free(arg);
}

PUSH_TS(TSArgument)

//#################################################################################
// TSField
//#################################################################################

TSField *newTSField() {
  TSField *field = (TSField *) calloc(TSField_SIZE, 1);
  CHECK_ALLOC_FAILED(field, TSField);
  field->name = NULL;
  field->type = NULL;
  field->value = NULL;
  field->decorators = NULL;
  field->decoratorsSize = 0;
  field->modifier = TSAccessModifier_Undefined;
  return field;
}

void freeTSField(const TSField *field) {
  if (field->decoratorsSize > 0) {
    for (unsigned long int i = 0; i <  field->decoratorsSize; i++) {
      freeTSDecorator(field->decorators[i]);
    }
  }
  if (field->name != NULL) free((void *) field->name);
  if (field->type != NULL) free((void *) field->type);
  free((void *) field);
}

PUSH_TS(TSField)

//#################################################################################
// TSFunction
//#################################################################################

TSFunction *newTSFunction() {
  TSFunction *method = (TSFunction *) calloc(TSFunction_SIZE, 1);
  CHECK_ALLOC_FAILED(method, TSFunction)
  method->name = NULL;
  method->type = TSMethod_Type_Undefined;
  method->body = NULL;
  method->decorators = NULL;
  method->decoratorsSize = 0;
  method->arguments = NULL;
  method->argumentsSize = 0;
  return method;
}

void freeTSFunction(TSFunction *mth) {
  if (mth == NULL) return;
  if (mth->decoratorsSize > 0) {
    for (unsigned long int i = 0; i < mth->decoratorsSize; i++) {
      freeTSDecorator(mth->decorators[i]);
    }
  }
  if (mth->argumentsSize > 0) {
    for (unsigned long int i = 0; i < mth->argumentsSize; i++) {
      freeTSArgument(mth->arguments[i]);
    }
  }
  free(mth);
}

PUSH_TS(TSFunction)

//#################################################################################
// TSMethod
//#################################################################################

TSMethod *newTSMethod() {
  TSMethod *method = (TSMethod *) calloc(TSMethod_SIZE, 1);
  CHECK_ALLOC_FAILED(method, TSMethod)
  method->name = NULL;
  method->type = TSMethod_Type_Undefined;
  method->body = NULL;
  method->decorators = NULL;
  method->decoratorsSize = 0;
  method->arguments = NULL;
  method->argumentsSize = 0;
  method->modifier = TSAccessModifier_Undefined;
  return method;
}

void freeTSMethod(TSMethod *mth) {
  if (mth == NULL) return;
  free(mth);
}

PUSH_TS(TSMethod)

//#################################################################################
// TSClass
//#################################################################################

TSClass *newTSClass() {
  TSClass *class = (TSClass *) calloc(TSClass_SIZE, 1);
  CHECK_ALLOC_FAILED(class, TSClass)
  class->decorators = NULL;
  class->decoratorsSize = 0;
  class->methods = NULL;
  class->methodsSize = 0;
  class->fields = NULL;
  class->fieldsSize = 0;
  class->parent = NULL;
  class->name = NULL;
  return class;
}

void freeTSClass(TSClass *class) {
  if (class == NULL) return;
  if (class->name != NULL) free((void*)class->name);
  if (class->parent != NULL) free((void*)class->parent);
  if (class->decorators != NULL) {
    unsigned long int size = class->decoratorsSize;
    for (unsigned long int i = 0; i < size; i++)
      freeTSDecorator(class->decorators[i]);
    free((void*)class->decorators);
    class->decorators = NULL;
  }
  if (class->methods != NULL) {
    unsigned long int size = class->methodsSize;
    for (unsigned long int i = 0; i < size; i++)
      freeTSMethod(class->methods[i]);
    free((void*)class->methods);
    class->methods = NULL;
  }
  free((void*)class);
}

PUSH_TS(TSClass)

//#################################################################################
// TSDecorator
//#################################################################################

TSDecorator *newTSDecorator() {
  TSDecorator *dec = (TSDecorator *) calloc(TSDecorator_SIZE, 1);
  CHECK_ALLOC_FAILED(dec, TSDecorator)
  dec->name = NULL;
  dec->arguments = NULL;
  dec->argumentsSize = 0;
  return dec;
}

void freeTSDecorator(TSDecorator *dec) {
  if (dec == NULL) return;
  if (dec->argumentsSize > 0) {
    unsigned long size = dec->argumentsSize;
    for (unsigned int i = 0; i < size; i++) {
      freeTSArgument(dec->arguments[i]);
    }
  }
  free(dec);
}

PUSH_TS(TSDecorator)

//#################################################################################
// TSParseClassBodyData
//#################################################################################

TSParseClassBodyData *newTSParseClassBodyData() {
  TSParseClassBodyData *data = (TSParseClassBodyData *) calloc(TSParseClassBodyData_SIZE, 1);
  CHECK_ALLOC_FAILED(data, TSParseClassBodyData)
  data->access = TSAccessModifier_Undefined;
  data->mthType = TSMethod_Type_Undefined;
  data->name = NULL;
  return data;
}

void freeTSParseClassBodyData(const TSParseClassBodyData *data) {
  if (data->name != NULL) {
    free(data->name);
  }
  free((void *) data);
}