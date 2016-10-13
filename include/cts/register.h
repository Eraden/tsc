#pragma once

#include <cts/parser.h>
#include <cts/sys.h>

typedef struct sTSRegisterEntry TSRegisterEntry;

typedef struct sTSRegisterEntry {
  TSFile *tsFile;
  TSParserToken **classList;
  u_long listSize;
} TSRegisterEntry;

#define RegisterCollection TSRegisterEntry **

TSParserToken **TS_OPERATORS;
const unsigned short int TS_OPERATORS_COUNT;

// Predefined TS types
TSParserToken *__attribute__((__used__)) TS_ANY_TYPE;
TSParserToken *__attribute__((__used__)) TS_UNDEFINED_TYPE;
TSParserToken *__attribute__((__used__))TS_NULL_TYPE;

// Arithmetic Operators
TSParserToken *__attribute__((__used__)) TS_PLUS;
TSParserToken *__attribute__((__used__)) TS_MINUS;
TSParserToken *__attribute__((__used__)) TS_MULTIPLY;
TSParserToken *__attribute__((__used__)) TS_DIVIDE;
TSParserToken *__attribute__((__used__)) TS_MODULO;
TSParserToken *__attribute__((__used__)) TS_INCREMENT;
TSParserToken *__attribute__((__used__)) TS_DECREMENT;

// Relational Operators
TSParserToken *__attribute__((__used__)) TS_EQUAL;
TSParserToken *__attribute__((__used__)) TS_SAME;
TSParserToken *__attribute__((__used__)) TS_NOT_EQUAL;
TSParserToken *__attribute__((__used__)) TS_GREATER;
TSParserToken *__attribute__((__used__)) TS_GREATER_OR_EQUAL;
TSParserToken *__attribute__((__used__)) TS_LESS;
TSParserToken *__attribute__((__used__)) TS_LESS_OR_EQUAL;

// Logical Operators
TSParserToken *__attribute__((__used__)) TS_LOGICAL_AND;
TSParserToken *__attribute__((__used__)) TS_LOGICAL_OR;
TSParserToken *__attribute__((__used__)) TS_LOGICAL_NOT;

// Bitwise Operators
TSParserToken *__attribute__((__used__)) TS_BITWISE_AND;
TSParserToken *__attribute__((__used__)) TS_BITWISE_OR;
TSParserToken *__attribute__((__used__)) TS_BITWISE_XOR;

TSRegisterEntry *TS_register_file(TSFile *tsFile);
void TS_register_remove_file(TSFile *file);
TSFile *TS_find_file(const char *path, FILE *stream);

void TS_register_type(TSFile *tsFile, TSParserToken *token);
TSParserToken *TS_find_type(const wchar_t *file, const wchar_t *name);

void TS_destroy_register(void);

void TS_setup_predefined(void);
void TS_remove_predefined(void);

unsigned char TS_is_predefined(TSParserToken *token);

unsigned char TS_is_type(TSParserToken *token);

TSParserToken * TS_search_in(wchar_t *name, TSParserToken *scope, TSFile *tsFile);
TSParserToken * TS_search_in_scope(wchar_t *name, TSParserToken *scope);
