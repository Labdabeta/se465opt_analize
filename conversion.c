#include "conversion.h"

typedef struct FunctionTAG {
    int numCallees;
    char *name;
    int id;

    int *callees;
}Function;
typedef struct CallingMapTAG {
    int numFunctions;

    Function *functions;
}CallingMap;

CallingMapHANDLE calling_map_from_parsed_file(ParsedFileHANDLE pfh)
{
    ParsedFile *pf = pfh;
    CallingMap *ret;

    ret = malloc(sizeof(CallingMap));



    
