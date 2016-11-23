#include <cts/file.h>
#include <cts/output.h>

void TS_write(TSFile *tsFile, const wchar_t *buffer) {
  switch (tsFile->output.type) {
    case TS_OUTPUT_STREAM: {
      fwprintf(tsFile->output.stream, (const wchar_t *) L"%ls", buffer);
      if (tsFile->output.stream != stdout && tsFile->output.stream != stderr) {
        fflush(tsFile->output.stream);
      }
      break;
    }
    case TS_OUTPUT_STRING: {
      size_t len = wcslen(tsFile->output.buffer);
      if (tsFile->output.buffer == NULL) {
        tsFile->output.buffer = malloc(len + 1);
        wcscpy(tsFile->output.buffer, buffer);
        tsFile->output.buffer[len] = 0;
      } else {
        size_t currentLen = wcslen(tsFile->output.buffer);
        tsFile->output.buffer = realloc(tsFile->output.buffer, sizeof(wchar_t) * (len + currentLen + 1));
        wcscat(tsFile->output.buffer, buffer);
        tsFile->output.buffer[len + currentLen] = 0;
      }
      break;
    }
    case TS_OUTPUT_UNSET: {
      fprintf(stderr, "No output given!\n");
      exit(6);
    }
    case TS_OUTPUT_FILE: {
      FILE *stream = fopen(tsFile->output.outputFileName, "a+");
      fwprintf(stream, (const wchar_t *) L"%ls", buffer);
      fflush(stream);
      fclose(stream);
      break;
    }
  }
}

void TS_clear_output(TSFile *tsFile) {
  switch (tsFile->output.type) {
    case TS_OUTPUT_UNSET: {
      tsFile->output.buffer = NULL;
      break;
    }
    case TS_OUTPUT_STREAM: {
      tsFile->output.stream = NULL;
      break;
    }
    case TS_OUTPUT_STRING: {
      free(tsFile->output.buffer);
      tsFile->output.buffer = NULL;
      break;
    }
    case TS_OUTPUT_FILE: {
      free(tsFile->output.outputFileName);
      tsFile->output.outputFileName = NULL;
      break;
    }
  }
}

void TS_set_output_stream(TSFile *tsFile, FILE *stream) {
  TS_clear_output(tsFile);
  tsFile->output.type = TS_OUTPUT_STREAM;
  tsFile->output.stream = stream;
}

void TS_set_output_string(TSFile *tsFile, wchar_t *str) {
  TS_clear_output(tsFile);
  tsFile->output.type = TS_OUTPUT_STRING;
  TS_write(tsFile, str);
}

void TS_set_output_file(TSFile *tsFile, char *fileName) {
  TS_clear_output(tsFile);
  tsFile->output.type = TS_OUTPUT_FILE;
  size_t len = strlen(fileName);
  tsFile->output.outputFileName = malloc(sizeof(char) * (len + 1));
  strcpy(tsFile->output.outputFileName, fileName);
  tsFile->output.outputFileName[len] = 0;
}

void TS_build_token_output(TSFile *tsFile) {
  switch (tsFile->output.currentToken->tokenType) {
    case TS_UNKNOWN: {
      TS_highlighted_error("Unexpected unknown token in generating JavaScript phase\n");
      TS_highlighted_error("  consider as a bug and please report\n");
      break;
    }
    case TS_VAR: {
      TS_build_var_output(tsFile);
      break;
    }
    case TS_LET: {
      TS_build_let_output(tsFile);
      break;
    }
    case TS_CONST: {
      TS_build_const_output(tsFile);
      break;
    }
    case TS_SEMICOLON: {
      TS_build_semicolon_output(tsFile);
      break;
    }
    case TS_COLON: {
      TS_build_colon_output(tsFile);
      break;
    }
    case TS_CLASS: {
      TS_build_class_output(tsFile);
      break;
    }
    case TS_CLASS_FIELD: {
      TS_build_class_field_output(tsFile);
      break;
    }
    case TS_CLASS_METHOD: {
      TS_build_class_method_output(tsFile);
      break;
    }
    case TS_EXTENDS: {
      TS_build_extends_output(tsFile);
      break;
    }
    case TS_IMPLEMENTS: {
      TS_build_implements_output(tsFile);
      break;
    }
    case TS_FUNCTION: {
      TS_build_function_output(tsFile);
      break;
    }
    case TS_FUNCTION_RETURN_TYPE: {
      TS_build_function_return_type_output(tsFile);
      break;
    }
    case TS_ARROW: {
      TS_build_arrow_output(tsFile);
      break;
    }
    case TS_IF: {
      TS_build_if_output(tsFile);
      break;
    }
    case TS_ELSE: {
      TS_build_else_output(tsFile);
      break;
    }
    case TS_RETURN: {
      TS_build_return_output(tsFile);
      break;
    }
    case TS_DECORATOR: {
      TS_build_decorator_output(tsFile);
      break;
    }
    case TS_DEFAULT: {
      TS_build_default_output(tsFile);
      break;
    }
    case TS_SCOPE: {
      TS_build_scope_output(tsFile);
      break;
    }
    case TS_NEW: {
      TS_build_new_output(tsFile);
      break;
    }
    case TS_INLINE_COMMENT: {
      TS_build_inline_comment_output(tsFile);
      break;
    }
    case TS_MULTILINE_COMMENT: {
      TS_build_multiline_comment_output(tsFile);
      break;
    }
    case TS_CONDITION: {
      TS_build_condition_output(tsFile);
      break;
    }
    case TS_ARGUMENT: {
      TS_build_argument_output(tsFile);
      break;
    }
    case TS_CALLER: {
      TS_build_caller_output(tsFile);
      break;
    }
    case TS_SWITCH: {
      TS_build_switch_output(tsFile);
      break;
    }
    case TS_SWITCH_BODY: {
      TS_build_switch_body_output(tsFile);
      break;
    }
    case TS_CASE: {
      TS_build_case_output(tsFile);
      break;
    }
    case TS_BREAK: {
      TS_build_break_output(tsFile);
      break;
    }
    case TS_FOR: {
      TS_build_for_output(tsFile);
      break;
    }
    case TS_FOR_WITH_CONDITION: {
      TS_build_for_with_condition_output(tsFile);
      break;
    }
    case TS_FOR_IN: {
      TS_build_for_in_output(tsFile);
      break;
    }
    case TS_FOR_OF: {
      TS_build_for_of_output(tsFile);
      break;
    }
    case TS_WHILE: {
      TS_build_while_output(tsFile);
      break;
    }
    case TS_DO: {
      TS_build_do_output(tsFile);
      break;
    }
    case TS_LOOP_VARIABLES_SECTION: {
      TS_build_loop_variables_section_output(tsFile);
      break;
    }
    case TS_LOOP_CONDITION_SECTION: {
      TS_build_loop_condition_section_output(tsFile);
      break;
    }
    case TS_LOOP_CHANGE_SECTION: {
      TS_build_loop_change_section_output(tsFile);
      break;
    }
    case TS_OF: {
      TS_build_of_output(tsFile);
      break;
    }
    case TS_IN: {
      TS_build_in_output(tsFile);
      break;
    }
    case TS_JSON: {
      TS_build_json_output(tsFile);
      break;
    }
    case TS_JSON_ENTRY: {
      TS_build_json_entry_output(tsFile);
      break;
    }
    case TS_ARRAY: {
      TS_build_array_output(tsFile);
      break;
    }
    case TS_STRING: {
      TS_build_string_output(tsFile);
      break;
    }
    case TS_STRING_TEMPLATE: {
      TS_build_string_template_output(tsFile);
      break;
    }
    case TS_CALL_ARGUMENTS: {
      TS_build_call_arguments_output(tsFile);
      break;
    }
    case TS_EXPORT: {
      TS_build_export_output(tsFile);
      break;
    }
    case TS_IMPORT: {
      TS_build_import_output(tsFile);
      break;
    }
    case TS_IMPORT_FROM: {
      TS_build_import_from_output(tsFile);
      break;
    }
    case TS_IMPORTED_TOKENS: {
      TS_build_imported_tokens_output(tsFile);
      break;
    }
    case TS_INTERFACE: {
      TS_build_interface_output(tsFile);
      break;
    }
    case TS_INTERFACE_BODY: {
      TS_build_interface_body_output(tsFile);
      break;
    }
    case TS_BORROW: {
      TS_build_borrow_output(tsFile);
      break;
    }
    case TS_OPERATOR: {
      TS_build_operator_output(tsFile);
      break;
    }
    case TS_NUMBER: {
      TS_build_number_output(tsFile);
      break;
    }
    case TS_GROUP: {
      TS_build_group_output(tsFile);
      break;
    }
    case TS_NAMESPACE: {
      TS_build_namespace_output(tsFile);
      break;
    }
    case TS_UNDEFINED: {
      TS_build_indent_output(tsFile);
      TS_write(tsFile, (const wchar_t *) L"undefined");
      break;
    }
    case TS_NULL: {
      TS_build_indent_output(tsFile);
      TS_write(tsFile, (const wchar_t *) L"null");
      break;
    }
    case TS_THIS: {
      TS_build_indent_output(tsFile);
      TS_write(tsFile, (const wchar_t *) L"this");
      break;
    }
    case TS_TRUE:{
      TS_build_indent_output(tsFile);
      TS_write(tsFile, (const wchar_t *) L"true");
      break;
    }
    case TS_FALSE: {
      TS_build_indent_output(tsFile);
      TS_write(tsFile, (const wchar_t *) L"false");
      break;
    }
    case TS_SUPER: {
      TS_build_super_output(tsFile);
      break;
    }
    case TS_SPREAD: {
      TS_build_spread_output(tsFile);
      break;
    }
  }
}

void TS_build_indent_output(TSFile *tsFile) {
  volatile unsigned int indentSize = tsFile->output.indent * tsFile->settings.indentSize;

  if (indentSize) {
    wchar_t *indent = calloc(sizeof(wchar_t *), indentSize + 1);

    while (indentSize) {
      indentSize -= 1;
      wcscat(indent, (const wchar_t *) L" ");
    }

    TS_write(tsFile, indent);
    free(indent);
  }
}

void TS_build_output(TSFile *tsFile) {
  if (tsFile->sanity != TS_FILE_VALID)
    return;

  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"(function (global) {\n");

  // TODO: Sort instead multiple loops :/
  unsigned long index = 0;
  TSParserToken **tokens = NULL;

  TSParserToken *token = NULL;

  volatile unsigned int initialIndent = tsFile->output.indent;

  // imports
  index = tsFile->tokensSize;
  tokens = tsFile->tokens;
  while (index) {
    index -= 1;
    token = tokens[0];
    if (token->tokenType == TS_IMPORT) {
      tsFile->output.indent = initialIndent;
      tsFile->output.currentToken = token;
      TS_build_token_output(tsFile);
    }
    tokens += 1;
  }

  tsFile->output.indent = initialIndent + 1;
  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"var module = {};\n");
  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"(function (module) {\n");
  tsFile->output.indent += 1;

  // definitions
  index = tsFile->tokensSize;
  tokens = tsFile->tokens;
  while (index) {
    index -= 1;
    token = tokens[0];
    if (token->tokenType != TS_EXPORT && token->tokenType != TS_IMPORT) {
      tsFile->output.currentToken = token;
      TS_build_token_output(tsFile);
    }
    tokens += 1;
  }

  // exports
  index = tsFile->tokensSize;
  tokens = tsFile->tokens;
  while (index) {
    index -= 1;
    token = tokens[0];
    if (token->tokenType == TS_EXPORT) {
      tsFile->output.currentToken = token;
      TS_build_token_output(tsFile);
    }
    tokens += 1;
  }

  if (tsFile->output.indent > 0) {
    tsFile->output.indent -= 1;
  } else {
    TS_highlighted_error("Invalid indent move!\n");
    exit(TS_PARSER_INTERNAL_ERROR);
  }

  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"}(module))\n");

  if (tsFile->output.indent > 0) {
    tsFile->output.indent -= 1;
  } else {
    TS_highlighted_error("Invalid indent move!\n");
    exit(TS_PARSER_INTERNAL_ERROR);
  }

  TS_build_indent_output(tsFile);
  TS_write(tsFile, (const wchar_t *) L"}(window));\n");
}