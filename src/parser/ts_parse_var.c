#include <tsc/parser/ts_variable.h>

const TSParserToken TS_parse_var(TSFile *tsFile, TSParseData *tsParseData) {
  TS_TOKEN_BEGIN("var");
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_VAR;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.position = tsParseData->position;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  TS_parse_local_variable_body(tsFile, tsParseData, &token, &movedBy);

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("var");
  return token;
}

void TS_free_var(TSParserToken token) {
  TS_free_children(token);

  TSLocalVariableData *data = token.data;
  if (data == NULL) return;
  if (data->name != NULL) free((void *) data->name);
  if (data->type != NULL) free((void *) data->type);
  if (data->value != NULL) free((void *) data->value);
  free(data);
}
