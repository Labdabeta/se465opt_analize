#include <string.h> //for strcmp
#include <stdlib.h> //for malloc
#include "conversion.h"

#include "opt_file_def.h"

/** @brief A structure to hold a function.
  * 
  * This structure holds a list of callees for a function.
  * Internally it stores its own name as both a string and an identifier, as
  * well as an array of callees, stored by id.
  */
typedef struct FunctionTAG {
    int numCallees;
    char *name;
    int id;

    int *callees;
}Function;

/** @brief A structure to hold a calling map.
  *
  * This structure holds an array of functions, whose indices are their ids.
  */
typedef struct CallingMapTAG {
    int numFunctions;

    Function *functions;
}CallingMap;

CallingMapHANDLE calling_map_from_parsed_file(const ParsedFileHANDLE pfh)
{
    const ParsedFile *pf = pfh;
    const ParsedFile *pfb = pf; //backup
    CallingMap *ret;
    int i,ii;

    ret = malloc(sizeof(CallingMap));
    
    //Determine number of functions
    ret->numFunctions = 0;
    while (pf)
    {
        pf = pf->next;
        ++ret->numFunctions;
    }
    pf = pfb; //restore pf

    //Load functions into calling map
    ret->functions = malloc(sizeof(Function)*ret->numFunctions);
    for (i=0; i<ret->numFunctions; ++i)
    {
        //NB: this shallow copy means that the name will be clobbered if the
        //  source pfh is freed. If memory becomes an issue over speed this can
        //  be changed to a deep copy, but that will require a change to the
        //  free function.
        ret->functions[i].name = pf->fname;
        ret->functions[i].id = i;
        pf = pf->next;
    }
    pf = pfb; //restore pf

    //Load callees for each function
    i=0;
    while (pf)
    {
        ret->functions[i].numCallees = 0;
        const CalleeList *cl = pf->callees;
        while (cl)
        {
            ret->functions[i].numCallees++;
            cl = cl->next;
        }
        cl = pf->callees;

        ret->functions[i].callees = 
                            malloc(sizeof(int)*ret->functions[i].numCallees);

        for (ii=0; ii<ret->functions[i].numCallees; ++ii)
        {
            ret->functions[i].callees[ii] = calling_map_get_fid(ret,cl->name);
            cl = cl->next;
        }
        pf = pf->next;
        ++i;
    }

    return ret;
}

void free_calling_map(CallingMapHANDLE cmh)
{
    CallingMap *cm = cmh;
    free(cm->functions);//benefits of the shallow copy C:
    free(cm);
}

int calling_map_num_functions(const CallingMapHANDLE cmh)
{
    const CallingMap *cm=cmh;
    return cm->numFunctions;
}

int calling_map_num_callees(const CallingMapHANDLE cmh, int fid)
{
    const CallingMap *cm=cmh;
    return cm->functions[fid].numCallees;
}

int *calling_map_list_callees(const CallingMapHANDLE cmh, int fid)
{
    const CallingMap *cm=cmh;
    return cm->functions[fid].callees;
}
        
char *calling_map_get_fname(const CallingMapHANDLE cmh, int fid)
{
    const CallingMap *cm=cmh;
    return cm->functions[fid].name;
}

int calling_map_get_fid(const CallingMapHANDLE cmh, const char *name)
{
    const CallingMap *cm=cmh;
    int i;
    for (i=0; i<cm->numFunctions; ++i)
        if (!strcmp(cm->functions[i].name,name)) return i;
    return -1;
}

void print_calling_map(const CallingMapHANDLE cmh)
{
    CallingMap *cm = cmh;
    int i,ii;
    printf("Num functions: %d\n\n", cm->numFunctions);
    for (i=0; i<cm->numFunctions; ++i)
    {
        printf("Name: %s(%d) - %d\n",
            cm->functions[i].name,cm->functions[i].id,cm->functions[i].numCallees);
        printf("Callees: ");
        for (ii=0; ii<cm->functions[i].numCallees; ++ii)
            printf("%d ",cm->functions[i].callees[ii]);
        printf("\n");
    }
}
    

            



    
