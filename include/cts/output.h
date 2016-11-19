#pragma once

#include <cts/file.h>
#include <cts/parser.h>

#define TS_SPREAD_PREFIX_SIZE 3

void TS_write(TSFile *tsFile, const wchar_t *buffer);

void TS_clear_output(TSFile *tsFile);

void __attribute__((__used__)) TS_set_output_stream(TSFile *tsFile, FILE *stream);

void __attribute__((__used__)) TS_set_output_string(TSFile *tsFile, wchar_t *str);

void __attribute__((__used__)) TS_set_output_file(TSFile *tsFile, char *fileName);

void TS_build_output(TSFile *tsFile);

void TS_build_indent_output(TSFile *tsFile);

void TS_build_token_output(TSFile *tsFile);

void TS_build_unknown_output(TSFile *tsFile);

void TS_build_var_output(TSFile *tsFile);

void TS_build_let_output(TSFile *tsFile);

void TS_build_const_output(TSFile *tsFile);

void TS_build_semicolon_output(TSFile *tsFile);

void TS_build_colon_output(TSFile *tsFile);

void TS_build_class_output(TSFile *tsFile);

void TS_build_class_field_output(TSFile *tsFile);

void TS_build_class_method_output(TSFile *tsFile);

void TS_build_extends_output(TSFile *tsFile);

void TS_build_implements_output(TSFile *tsFile);

void TS_build_function_output(TSFile *tsFile);

void TS_build_function_return_type_output(TSFile *tsFile);

void TS_build_arrow_output(TSFile *tsFile);

void TS_build_if_output(TSFile *tsFile);

void TS_build_else_output(TSFile *tsFile);

void TS_build_return_output(TSFile *tsFile);

void TS_build_decorator_output(TSFile *tsFile);

void TS_build_default_output(TSFile *tsFile);

void TS_build_scope_output(TSFile *tsFile);

void TS_build_new_output(TSFile *tsFile);

void TS_build_inline_comment_output(TSFile *tsFile);

void TS_build_multiline_comment_output(TSFile *tsFile);

void TS_build_condition_output(TSFile *tsFile);

void TS_build_argument_output(TSFile *tsFile);

void TS_build_caller_output(TSFile *tsFile);

void TS_build_switch_output(TSFile *tsFile);

void TS_build_switch_body_output(TSFile *tsFile);

void TS_build_case_output(TSFile *tsFile);

void TS_build_break_output(TSFile *tsFile);

void TS_build_for_output(TSFile *tsFile);

void TS_build_for_with_condition_output(TSFile *tsFile);

void TS_build_for_in_output(TSFile *tsFile);

void TS_build_for_of_output(TSFile *tsFile);

void TS_build_while_output(TSFile *tsFile);

void TS_build_do_output(TSFile *tsFile);

void TS_build_loop_variables_section_output(TSFile *tsFile);

void TS_build_loop_condition_section_output(TSFile *tsFile);

void TS_build_loop_change_section_output(TSFile *tsFile);

void TS_build_of_output(TSFile *tsFile);

void TS_build_in_output(TSFile *tsFile);

void TS_build_json_output(TSFile *tsFile);

void TS_build_json_entry_output(TSFile *tsFile);

void TS_build_array_output(TSFile *tsFile);

void TS_build_string_output(TSFile *tsFile);

void TS_build_string_template_output(TSFile *tsFile);

void TS_build_call_arguments_output(TSFile *tsFile);

void TS_build_export_output(TSFile *tsFile);

void TS_build_import_output(TSFile *tsFile);

void TS_build_import_from_output(TSFile *tsFile);

void TS_build_imported_tokens_output(TSFile *tsFile);

void TS_build_interface_output(TSFile *tsFile);

void TS_build_interface_body_output(TSFile *tsFile);

void TS_build_borrow_output(TSFile *tsFile);

void TS_build_operator_output(TSFile *tsFile);

void TS_build_number_output(TSFile *tsFile);

void TS_build_group_output(TSFile *tsFile);

void TS_build_namespace_output(TSFile *tsFile);

extern const wchar_t *
__attribute__((__malloc__))
TS_output_header(void);

const wchar_t *
__attribute__((__malloc__))
TS_output_footer(void);
