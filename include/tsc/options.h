#pragma once

#include <tsc/sys.h>
#include <tsc/str.h>
#include <tsc/output.h>

enum TSOptionFlag {
  TSOptionFlag_Undefined,
  TSOptionFlag_Output,
  TSOptionFlag_Environment,
  TSOptionFlag_LanguageQualityRequirements,
  TSOptionFlag_Help,
};

enum TSLanguageQualityRequirements {
  TSLanguageQualityRequirements_Relax,
  TSLanguageQualityRequirements_Strict,
  TSLanguageQualityRequirements_Strong
};

char *PROGRAM;
char *ENVIRONMENT;
char *OUTPUT_FILE;
enum TSLanguageQualityRequirements LANGUAGE_QUALITY_REQUIREMENTS;

void TSC_parse_options(int argc, char **argv);
