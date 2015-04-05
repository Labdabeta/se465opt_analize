#include <stdlib.h>

#include "bugs.h"

typedef struct BugReportTAG {
    int source; //<-- The call that IS present
    int scope;  //<-- The scope of the bug
    int missing;//<-- The call that is missing 

    int support;
    float confidence;
}BugReport;

//Note: initial node's bug is invalid, it is used as a root node
typedef struct BugListTAG {
    BugReport bug;
    struct BugListTAG *next;
}BugList;

static void push_bug(BugReport br, BugList *bl)
{
    if (!bl) return;
    BugList *tmp = bl->next;
    BugList *new = malloc(sizeof(BugList));
    new->bug     = br;
    new->next    = tmp;
    bl->next     = new;
}

//Adds all instances of a without b in cmh to bl
static void add_bugs(CallingMapHANDLE cmh, BugList *bl, int a, int b)
{
    //TODO
}

BugListHANDLE generate_bug_list(const CallingMapHANDLE cmh, int t_support, int t_confidence)
{
    SupportMapHANDLE sm = generate_support_map(cmh); 
    int i,ii;
    int numFunctions = calling_map_num_functions(cmh);
    BugList *ret = malloc(sizeof(BugList));

    for (i=0; i<numFunctions; ++i)
    {
        for (ii=0; ii<numFunctions; ++ii)
        {
            if (ii==i) continue;
            if (support_map_get_support(sm,i,ii) > t_support)
            {
                if (((100*support_map_get_support(sm,i,ii))/
                     (100*support_map_get_support(sm,i,i))) > t_confidence)
                    add_bugs(cmh,ret,i,ii);
            }
        }
    }
    return ret;
}

void print_bug_list(const BugListHANDLE blh, const CallingMapHANDLE cmh)
{
    BugList *bl = blh;
    int i;

    for (i=0; i<bl->numBugs; ++i)
    {
        printf("bug: %s in %s, pair: (%s, %s), support: %d, confidence: %.2f%%\n",
            calling_map_get_fname(cmh,bl->bugs[i].source),
            calling_map_get_fname(cmh,bl->bugs[i].scope),
            calling_map_get_fname(cmh,MAX(bl->bugs[i].source,bl->bugs[i].missing)),
            calling_map_get_fname(cmh,MIN(bl->bugs[i].missing,bl->bugs[i].source)),
            bl->bugs[i].support,bl->bugs[i].confidence);
    }
}
