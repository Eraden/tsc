#include "./parse_json.h"

START_TEST(parse_valid_json)
  TSFile *tsFile = TS_parse_file("./examples/json/valid.ts");
  ck_assert_tsFile_valid(tsFile);
  ck_assert_uint_eq(tsFile->tokensSize, 6);
  ck_assert_ptr_ne(tsFile->tokens, NULL);

  TSParserToken *variable = NULL, *json = NULL, *jsonEntry = NULL, *key = NULL, *value = NULL, *semicolon = NULL;

  variable = tsFile->tokens[0];
  ck_assert_ptr_ne(variable, NULL);
  ck_assert_eq_ts_var(variable->tokenType);
  ck_assert_ptr_ne(variable->children, NULL);
  ck_assert_uint_eq(variable->childrenSize, 2);
  TS_check_validate_borrow(variable->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);
  json = variable->children[TS_VARIABLE_VALUE_INDEX];
  ck_assert_ptr_ne(json, NULL);
  ck_assert_eq_ts_json(json->tokenType);
  ck_assert_uint_eq(json->childrenSize, 1);
  jsonEntry = json->children[0];
  ck_assert_ptr_ne(jsonEntry, NULL);
  ck_assert_eq_ts_json_entry(jsonEntry->tokenType);
  ck_assert_uint_eq(jsonEntry->childrenSize, 2);
  ck_assert_ptr_ne(jsonEntry->children, NULL);
  key = jsonEntry->children[TS_JSON_KEY];
  ck_assert_ptr_ne(key, NULL);
  ck_assert_eq_ts_string(key->tokenType);
  value = jsonEntry->children[TS_JSON_VALUE];
  ck_assert_ptr_ne(value, NULL);

  semicolon = tsFile->tokens[1];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  variable = tsFile->tokens[2];
  ck_assert_ptr_ne(variable, NULL);
  ck_assert_eq_ts_var(variable->tokenType);
  ck_assert_ptr_ne(variable->children, NULL);
  ck_assert_uint_eq(variable->childrenSize, 2);
  TS_check_validate_borrow(variable->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);
  json = variable->children[TS_VARIABLE_VALUE_INDEX];
  ck_assert_ptr_ne(json, NULL);
  ck_assert_eq_ts_json(json->tokenType);
  ck_assert_uint_eq(json->childrenSize, 2);
  jsonEntry = json->children[0];
  ck_assert_ptr_ne(jsonEntry, NULL);
  ck_assert_eq_ts_json_entry(jsonEntry->tokenType);
  ck_assert_uint_eq(jsonEntry->childrenSize, 2);
  ck_assert_ptr_ne(jsonEntry->children, NULL);
  key = jsonEntry->children[TS_JSON_KEY];
  ck_assert_ptr_ne(key, NULL);
  ck_assert_eq_ts_string(key->tokenType);
  value = jsonEntry->children[TS_JSON_VALUE];
  ck_assert_ptr_ne(value, NULL);
  jsonEntry = json->children[1];
  ck_assert_ptr_ne(jsonEntry, NULL);
  ck_assert_eq_ts_json_entry(jsonEntry->tokenType);
  ck_assert_uint_eq(jsonEntry->childrenSize, 2);
  ck_assert_ptr_ne(jsonEntry->children, NULL);
  key = jsonEntry->children[TS_JSON_KEY];
  ck_assert_ptr_ne(key, NULL);
  ck_assert_eq_ts_string(key->tokenType);
  value = jsonEntry->children[TS_JSON_VALUE];
  ck_assert_ptr_ne(value, NULL);

  semicolon = tsFile->tokens[3];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  variable = tsFile->tokens[4];
  ck_assert_ptr_ne(variable, NULL);
  ck_assert_eq_ts_var(variable->tokenType);
  ck_assert_ptr_ne(variable->children, NULL);
  ck_assert_uint_eq(variable->childrenSize, 2);
  TS_check_validate_borrow(variable->children[TS_VARIABLE_TYPE_INDEX], TS_ANY_TYPE);
  json = variable->children[TS_VARIABLE_VALUE_INDEX];
  ck_assert_ptr_ne(json, NULL);
  ck_assert_eq_ts_json(json->tokenType);
  ck_assert_uint_eq(json->childrenSize, 1);
  jsonEntry = json->children[0];
  ck_assert_ptr_ne(jsonEntry, NULL);
  ck_assert_eq_ts_json_entry(jsonEntry->tokenType);
  ck_assert_uint_eq(jsonEntry->childrenSize, 2);
  ck_assert_ptr_ne(jsonEntry->children, NULL);
  key = jsonEntry->children[TS_JSON_KEY];
  ck_assert_ptr_ne(key, NULL);
  ck_assert_eq_ts_string(key->tokenType);
  value = jsonEntry->children[TS_JSON_VALUE];
  ck_assert_ptr_ne(value, NULL);
  ck_assert_eq_ts_json(value->tokenType);

  semicolon = tsFile->tokens[5];
  ck_assert_eq_ts_semicolon(semicolon->tokenType);

  TS_free_ts_file(tsFile);
END_TEST

void parse_json(Suite *suite) {
  TCase *tc_json = tcase_create("Parse JSON");
  tcase_add_test(tc_json, parse_valid_json);
  suite_add_tcase(suite, tc_json);
}