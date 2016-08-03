#include "./test.h"

void validate_ts_file(const TSFile tsFile, const int size, const TSTokenType validType) {
  ck_assert_int_eq(tsFile.tokensSize, size);
  TSParserToken token = tsFile.tokens[0];

  ck_assert(token.tokenType == validType);
}

TSFile build_ts_file(const char *fileName, const char *content) {
  FILE *stream;
  stream = fmemopen((void *) content, strlen(content), "r");
  return TS_parse_stream(fileName, stream);
}
