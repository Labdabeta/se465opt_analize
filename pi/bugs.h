#ifndef BUGS_H
#define BUGS_H 1.0

#include "support.h"
#include "conversion.h"

typedef void *BugListHANDLE;

BugListHANDLE generate_bug_list(const CallingMapHANDLE, int, int);//T_SUPPORT, T_CONFIDENCE
void print_bug_list(const BugListHANDLE, const CallingMapHANDLE);

#endif //BUGS_H
