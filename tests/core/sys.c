#include "./sys.h"

START_TEST(check_resolved_directory)
  wchar_t *res = NULL;

  res = TS_resolve_directory((const wchar_t *) L"/etc/hosts");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"/etc/");
  free(res);

  res = TS_resolve_directory((const wchar_t *) L"/home/check/hosts");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"/home/check/");
  free(res);

  res = TS_resolve_directory((const wchar_t *) L"/home/check/hosts.txt");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"/home/check/");
  free(res);

  res = TS_resolve_directory((const wchar_t *) L"/home/check/hosts.plain.txt");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"/home/check/");
  free(res);
END_TEST

START_TEST(check_resolved_file)
  wchar_t *res = NULL;

  res = TS_resolve_file((const wchar_t *) L"/etc/hosts");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"hosts");
  free(res);

  res = TS_resolve_file((const wchar_t *) L"/home/check/hosts");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"hosts");
  free(res);

  res = TS_resolve_file((const wchar_t *) L"/home/check/hosts.txt");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"hosts.txt");
  free(res);

  res = TS_resolve_file((const wchar_t *) L"/home/check/hosts.plain.txt");
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"hosts.plain.txt");
  free(res);
END_TEST

START_TEST(check_resolved_path)
  wchar_t *absolute, *relative, *res, *tmp;

  tmp = (wchar_t *) L"/a/b/c/d/";
  absolute = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscat(absolute, tmp);
  tmp = (wchar_t *) L"../../e/f/file";
  relative = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscpy(relative, tmp);
  res = TS_resolve_path(absolute, relative);
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"/a/b/e/f/file");
  free(res);
  free(relative);
  free(absolute);

  tmp = (wchar_t *) L"/a/b/";
  absolute = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscat(absolute, tmp);
  tmp = (wchar_t *) L"./e/f/file";
  relative = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscpy(relative, tmp);
  res = TS_resolve_path(absolute, relative);
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"/a/b/e/f/file");
  free(res);
  free(relative);
  free(absolute);

  tmp = (wchar_t *) L"/a/b/c/";
  absolute = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscat(absolute, tmp);
  tmp = (wchar_t *) L".././e/f/file";
  relative = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscpy(relative, tmp);
  res = TS_resolve_path(absolute, relative);
  ck_assert_ptr_ne(res, NULL);
  ck_assert_wstr_eq(res, L"/a/b/e/f/file");
  free(res);
  free(relative);
  free(absolute);

  tmp = (wchar_t *) L"/";
  absolute = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscat(absolute, tmp);
  tmp = (wchar_t *) L"../../file";
  relative = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscpy(relative, tmp);
  res = TS_resolve_path(absolute, relative);
  ck_assert_ptr_eq(res, NULL);
  free(relative);
  free(absolute);

  tmp = (wchar_t *) L"/example-path-but-too-short/";
  absolute = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscat(absolute, tmp);
  tmp = (wchar_t *) L"../../../../file";
  relative = calloc(sizeof(wchar_t), wcslen(tmp) + 1);
  wcscpy(relative, tmp);
  res = TS_resolve_path(absolute, relative);
  ck_assert_ptr_eq(res, NULL);
  free(relative);
  free(absolute);
END_TEST

void parse_sys_suite(Suite *suite) {
  TCase *tc_sys = tcase_create("sys");
  tcase_add_test(tc_sys, check_resolved_directory);
  tcase_add_test(tc_sys, check_resolved_file);
  tcase_add_test(tc_sys, check_resolved_path);
  suite_add_tcase(suite, tc_sys);
}

