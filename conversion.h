#ifndef CONVERSION_H
#define CONVERSION_H 1.0

#include "opt_file.h"

typedef void *CallingMapHANDLE;

CallingMapHANDLE calling_map_from_parsed_file(const ParsedFileHANDLE);
void free_calling_map(CallingMapHANDLE);

int calling_map_num_functions(const CallingMapHANDLE);//functions are 0..n-1
int calling_map_num_callees(const CallingMapHANDLE, int);//fid
int *calling_map_list_callees(const CallingMapHANDLE, int);//fid
char *calling_map_get_fname(const CallingMapHANDLE, int);//fid
int calling_map_get_fid(const CallingMapHANDLE, const char *);//works before callees populate

void print_calling_map(const CallingMapHANDLE);


#endif 
