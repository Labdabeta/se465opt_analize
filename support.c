#include <stdlib.h>
#include "support.h"

typedef struct SupportMapTAG {
    int numFunctions;
    int *support; //addressed as [y*numFunctions+x]
}SupportMap;

SupportMapHANDLE generate_support_map(const CallingMapHANDLE cmh)
{
    SupportMap *ret = malloc(sizeof(SupportMap));
    ret->numFunctions = calling_map_num_functions(cmh);
    ret->support = malloc(sizeof(int)*ret->numFunctions*ret->numFunctions);
    int i,ii,iii;

    //Initialize support array to {0}
    for (i=0; i<ret->numFunctions; ++i)
        for (ii=0; ii<ret->numFunctions; ++ii)
            ret->support[i*ret->numFunctions+ii] = 0;

    //Populate support array
    for (i=0; i<ret->numFunctions; ++i)
    {
        int *callees = calling_map_list_callees(cmh,i);
        for (ii=0; ii<calling_map_num_callees(cmh,i); ++ii)
        {
            for (iii=0; iii<ii; ++iii)
            {
                ret->support[callees[iii]*ret->numFunctions+callees[ii]]++;
                if (callees[iii]!=callees[ii])
                    ret->support[callees[ii]*ret->numFunctions+callees[iii]]++;
            }
        }
    }

    return ret;
}
    
void free_support_map(SupportMapHANDLE smh)
{
    SupportMap *sm = smh;
    free(sm->support);
    free(sm);
}

int support_map_get_support(const SupportMapHANDLE smh, int a, int b)
{
    SupportMap *sm = smh;
    return sm->support[a*sm->numFunctions+b];
}

