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
static void add_bugs(CallingMapHANDLE cmh, BugList *bl, int a, int b, int s, int c)
{
    int i,ii;
    BugReport br;
    for (i=0; i<calling_map_num_functions(cmh); ++i)
    {
        //int *callees = calling_map_list_callees(cmh,i);
        int afound = 0;//no a yet
        int bfound = 0;//no b yet
        for (ii=0; ii<calling_map_num_callees(cmh,i); ++ii)
        {
            if (calling_map_list_callees(cmh,i)[ii] == a) afound = 1;
            if (calling_map_list_callees(cmh,i)[ii] == b) bfound = 1;
            if (afound && bfound) break;
        }
        if (afound && !bfound)
        {
            br.source = a;
            br.scope = i;
            br.missing = b;
            br.support = s;
            br.confidence = c;
            push_bug(br,bl);
        }
    }
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
            if (support_map_get_support(sm,i,ii) >= t_support)
            {
                int confidence = 100*support_map_get_support(sm,i,ii);
                confidence /= support_map_get_support(sm,i,i);
                if (confidence == 100) continue;
                if (confidence >= t_confidence)
                    add_bugs(cmh,ret,i,ii,support_map_get_support(sm,i,ii),
                        confidence);
            }
        }
    }
    free_support_map(sm);
    return ret;
}
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)>(b)?(b):(a))
void print_bug_list(const BugListHANDLE blh, const CallingMapHANDLE cmh)
{
    BugList *bl = blh;
    bl=bl->next;

    while (bl)
    {
        printf("bug: %s in %s, pair: (%s, %s), support: %d, confidence: %.2f%%\n",
            calling_map_get_fname(cmh,bl->bug.source),
            calling_map_get_fname(cmh,bl->bug.scope),
            calling_map_get_fname(cmh,MAX(bl->bug.source,bl->bug.missing)),
            calling_map_get_fname(cmh,MIN(bl->bug.missing,bl->bug.source)),
            bl->bug.support,(float)bl->bug.confidence);
        bl = bl->next;
    }
}
#undef MAX
#undef MIN
