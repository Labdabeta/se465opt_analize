#ifndef CONVERSION_H
#define CONVERSION_H 1.0

#include "opt_file.h"

typedef void *CallingMapHANDLE;

CallingMapHANDLE calling_map_from_parsed_file(ParsedFileHANDLE);
void free_calling_map(CallingMapHANDLE);

int *calling_map_list_functions(const CallingMapHANDLE);
int *calling_map_list_callees(const CallingMapHANDLE, int);//fid
char *calling_map_get_fname(const CallingMapHANDLE, int);//fid

void print_calling_map(const CallingMapHANDLE);


#endif 
