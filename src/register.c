#include <tsc/register.h>

static TSRegisterEntry *
    __attribute__(( visibility("hidden") ))
    classRegister = NULL;

void TS_register_class(const TSFile *file, const TSParserToken token)
{
  {
    TSRegisterEntry *entry = classRegister;
    TSClassData *classData = token.data;
    while (entry != NULL) {
      const TSFile *tsFile = entry->tsFile;
      TSParserToken t = entry->classToken;
      TSClassData *data = t.data;
      if (file == tsFile && strcmp(data->name, classData->name) == 0) {
        return;
      }
      entry = entry->next;
    }
  }
  {
    TSRegisterEntry *entry = (TSRegisterEntry *) calloc(sizeof(TSRegisterEntry), 1);
    entry->tsFile = file;
    entry->classToken = token;
    if (classRegister == NULL) {
      classRegister = entry;
    } else {
      TSRegisterEntry *current = classRegister;
      while (current->next) current = current->next;
      current->next = entry;
    }
  }
}

const TSParserToken
__attribute__(( __unused__ ))
TS_find_class(
    const char *__attribute__(( __unused__ )) file,
    const char *__attribute__(( __unused__ )) name
)
{
  TSRegisterEntry *entry = classRegister;
  while (entry) {
    entry = entry->next;
  }
  TSParserToken t;
  t.tokenType = TS_UNKNOWN;
  return t;
}