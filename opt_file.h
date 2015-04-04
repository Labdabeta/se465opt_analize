#ifndef OPT_FILE_H
#define OPT_FILE_H 1.0

#include <stdio.h>

/// Some buffer sizes
#define LINE_LENGTH_BUFFER_SIZE 256
#define FUNCTION_NAME_BUFFER_SIZE 100

typedef void *ParsedFileHANDLE;
typedef char FunctionName[FUNCTION_NAME_BUFFER_SIZE];

ParsedFileHANDLE parse_opt_file(FILE *f);
void free_parsed_file(ParsedFileHANDLE);

#endif 
