#ifndef OPT_FILE_H
#define OPT_FILE_H 1.0

#include <stdio.h>

typedef void *ParsedFileHANDLE;

ParsedFileHANDLE parse_opt_file(FILE *f);

#endif 
