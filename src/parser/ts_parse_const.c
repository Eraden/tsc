#include <tsc/parser/ts_variable.h>

const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("const");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CONST;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TS_parse_local_variable_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("const");
  return token;
}

void TS_free_const(TSParserToken token) {
  TSLocalVariableData *data = token.data;
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);

  free(data);
}
