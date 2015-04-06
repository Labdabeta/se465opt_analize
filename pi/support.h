#ifndef SUPPORT_H
#define SUPPORT_H 1.0

#include "conversion.h"

typedef void *SupportMapHANDLE;

SupportMapHANDLE generate_support_map(const CallingMapHANDLE);
void free_support_map(SupportMapHANDLE);

int support_map_get_support(const SupportMapHANDLE, int, int);

#endif //SUPPORT_H
