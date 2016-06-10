#pragma once

#include <tsc/sys.h>
#include <tsc/log.h>
#include <tsc/macro.h>

static const char  DECORATOR      = '@';
static const char *CLASS          = "class";
static const char *FUNCTION       = "function";
static const char *CONST          = "const";
static const char *LET            = "let";
static const char *VAR            = "var";
static const char *EXPORT         = "export";
static const char *DEFAULT        = "default";
static const char *EXTENDS        = "extends";
static const char *TS_PUBLIC      = "public";
static const char *TS_PROTECTED   = "protected";
static const char *TS_PRIVATE     = "private";
static const char *TS_PACKAGE     = "package";
static const char *TS_FRIEND      = "friend";
static const char *TS_GET         = "get";
static const char *TS_SET         = "set";

enum TSError {
  INVALID_DECORATOR_ANNOTATION = 10,
  MULTIPLE_DEFAULT_EXPORT_ERROR = 11,
  PARSE_CONTEXT_NO_CONTENT = 12,
  NO_PARSE_CONTEXT = 13,
  FILE_NOT_FOUND_ERROR = 14,
  INVALID_TOKEN = 15
};

enum TSMethod_Type {
  TSMethod_Type_Undefined,
  TSMethod_Type_Normal,
  TSMethod_Type_Getter,
  TSMethod_Type_Setter
};

enum TSAccessModifier {
  TSAccessModifier_Undefined,
  TSAccessModifier_Public,
  TSAccessModifier_Private,
  TSAccessModifier_Protected,
  TSAccessModifier_Local,
  TSAccessModifier_Package,
  TSAccessModifier_Friend
};

enum TSExportType {
  TSExportType_Undefined,
  TSExportType_Class,
  TSExportType_Method,
  TSExportType_Const,
  TSExportType_Let,
  TSExportType_Var
};

typedef struct sTSExport {
  unsigned short int asDefault;
  enum TSExportType type;
  void *meta;
} TSExport;

#define TSExport_SIZE sizeof(TSExport)

typedef struct sTSArgument {
  char *name;
  char *type;
  char *value;
  enum TSAccessModifier modifier;
  unsigned short int isRest;
} TSArgument;

#define TSArgument_SIZE sizeof(TSArgument)

typedef struct sTSDecorator {
  const char *name;
  TSArgument **arguments;
  unsigned long int argumentsSize;
} TSDecorator;

#define TSDecorator_SIZE sizeof(TSDecorator)

typedef struct sTSField {
  TSDecorator **decorators;
  unsigned long int decoratorsSize;
  enum TSAccessModifier modifier;
  const char *name;
  const char *type;
  const char *value;
} TSField;

#define TSField_SIZE sizeof(TSField)

typedef struct sTSFunction {
  TSArgument **arguments;
  unsigned long int argumentsSize;
  TSDecorator **decorators;
  unsigned long int decoratorsSize;
  char *body;
  const char *name;
  enum TSMethod_Type type;
} TSFunction;

#define TSFunction_SIZE sizeof(TSFunction)

typedef struct sTSMethod {
  TSArgument **arguments;
  unsigned long int argumentsSize;
  TSDecorator **decorators;
  unsigned long int decoratorsSize;
  enum TSAccessModifier modifier;
  char *body;
  char *name;
  enum TSMethod_Type type;
} TSMethod;

#define TSMethod_SIZE sizeof(TSMethod)

typedef struct sTSClass {
  TSDecorator **decorators;
  unsigned long int decoratorsSize;
  TSMethod **methods;
  unsigned long int methodsSize;
  TSField **fields;
  unsigned long int fieldsSize;
  const char *parent;
  const char *name;
} TSClass;

#define TSClass_SIZE  sizeof(TSClass)

typedef struct sTSToken {
  const char *content;
} TSToken;

#define TSToken_SIZE sizeof(TSToken)

typedef struct sTSParseContext {
  const char *content;
  const char *file;
  unsigned long long int line, character, position;
  unsigned long long int size;
  unsigned short int hasDefaultExport;
  TSToken *currentToken;
  char lastChar;
  TSFunction **globalFunctions;
  unsigned long int globalFunctionsSize;
  TSClass **globalClasses;
  unsigned long int globalClassesSize;
  TSDecorator **decoratorsStack;
  unsigned long int decoratorsStackSize;
  TSExport **exports;
  unsigned long int exportsSize;
} TSParseContext;

#define TSParseContext_SIZE sizeof(TSParseContext)

typedef struct sTSParseClassBodyData {
  enum TSAccessModifier access;
  enum TSMethod_Type mthType;
  char *name;
} TSParseClassBodyData;

#define TSParseClassBodyData_SIZE sizeof(TSParseClassBodyData)

// Types
TSToken *newTSToken();
void freeTSToken(TSToken *token);

TSParseContext *newTSParseContext(const char *file_path);
void freeTSParseContext(TSParseContext *context);

TSArgument *newTSArgument();
void freeTSArgument(TSArgument *arg);
TSArgument **pushTSArgument(TSArgument **coll, unsigned long int size, TSArgument *arg);

TSMethod *newTSMethod();
void freeTSMethod(TSMethod *mth);
TSMethod **pushTSMethod(TSMethod **coll, unsigned long int size, TSMethod *arg);

TSFunction *newTSFunction();
void freeTSFunction(TSFunction *mth);
TSFunction **pushTSFunction(TSFunction **coll, unsigned long int size, TSFunction *arg);

TSDecorator *newTSDecorator();
void freeTSDecorator(TSDecorator *dec);
TSDecorator **pushTSDecorator(TSDecorator **coll, unsigned long int size, TSDecorator *arg);

TSClass *newTSClass();
void freeTSClass(TSClass *dec);
TSClass **pushTSClass(TSClass **coll, unsigned long int size, TSClass *el);

TSExport *newTSExport();
void freeTSExport(TSExport *export);
TSExport **pushTSExport(TSExport **coll, unsigned long int size, TSExport *el);

TSField *newTSField();
void freeTSField(const TSField *field);
TSField **pushTSField(TSField **coll, unsigned long int size, TSField *el);

TSParseClassBodyData *newTSParseClassBodyData();
void freeTSParseClassBodyData(const TSParseClassBodyData *data);