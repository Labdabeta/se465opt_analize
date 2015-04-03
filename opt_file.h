#ifndef OPT_FILE_H
#define OPT_FILE_H 1.0

#include <stdio.h>

typedef void *ParsedFileHANDLE;

ParsedFileHANDLE parse_opt_file(FILE *f);
void free_parsed_file(ParsedFileHANDLE);


//TODO: add push/pop/etc to ParsedFileHANDLE, use to simplify algorithms

#endif 
