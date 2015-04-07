#include "opt_file.h"
#include <stdlib.h>

#include "opt_file_def.h"

/** @brief A function to fill a CalleeList with appropriate data from a file.
  *
  * This function parses a scope from the file in opt format. It should be 
  * called with the file seeked to the start of a scope block, with the initial
  * line already parsed.
  *
  * Internally it parses a single line, then recurses to parse the rest. The
  * base case of the recursion is when an empty line is encountered.
  *
  * @param f The file to be parsed.
  * @return The list of callees, or NULL if the file format is inaccurate.
  */
static CalleeList *parse_callees(FILE *f)
{
    char *line;         ///<-- Represents a line read from f
    char *save;         ///<-- Saved base of line for freeability
    CalleeList *ret;    ///<-- The returned callee list structure
    int i;              ///<-- An iterator

    if (feof(f)) return NULL;

    line = malloc(sizeof(char)*LINE_LENGTH_BUFFER_SIZE);
    save = line;
    fgets(line, LINE_LENGTH_BUFFER_SIZE, f);

    //Base case: an empty line indicates an empty list
    if (*line != ' ')
    {
        free(line);
        return NULL;
    }
    
    ret = malloc(sizeof(CalleeList));
		
    //Scan to first ', if e is found skip line
    while (*(line++) != '\'')
    {
	//looking for word "external", but other letters are not unique to the word
        if (*line == 'r') 
        {
            ret = parse_callees(f);
            free(save);
            return ret;
        }
    }
 		
    i=0;
    if(!ret) return NULL;

    ret->name = malloc(sizeof(char)*FUNCTION_NAME_BUFFER_SIZE);
    ret->name[i++] = *line;
    while (*(line++) != '\'') ret->name[i++] = *line;
    ret->name[i-1] = '\0';

    ret->next = parse_callees(f);
    free(save);
    return ret;
}

/** @brief A function to fill a ParsedFile with appropriate data from a file.
  *
  * This function parses a file in opt format. It should be called with the file
  * seeked to the start of a scope block, with the initial line ready to be 
  * read.
  *
  * Internally it parses a single scope, then recurses to parse the rest. The 
  * base case of the recursion is when an empty line is encountered.
  *
  * @param f The file to be parsed.
  * @return The list of scopes, or NULL if the file format is inaccurate.
  */
ParsedFileHANDLE parse_opt_file(FILE *f)
{
    char *line;         ///<-- Represents a line read from f
    char *save;         ///<-- Saved base of the line for freeability
    char *fname;        ///<-- Represents a function name
    ParsedFile *ret;    ///<-- The returned parsed file structure
    int i;              ///<-- An iterator

    //Base case: empty file returns NULL
    if (feof(f)) return NULL;

    line = malloc(sizeof(char)*LINE_LENGTH_BUFFER_SIZE);
    save = line;
    fname = malloc(sizeof(char)*FUNCTION_NAME_BUFFER_SIZE);
    ret = malloc(sizeof(ParsedFile));

    fgets(line, LINE_LENGTH_BUFFER_SIZE, f);
    
    if (feof(f) || *(line++) == '\n') return NULL; //if it's the end of the file
    while (*(line++) != '\''); //scan to first '

	i=0;
	while (*line != '\'') {
		  fname[i++] = *(line++);
	}
	fname[i] = 0;
	if(!ret) return NULL;
	ret->fname = fname;
	free(save);
	ret->callees = parse_callees(f);
	ret->next = parse_opt_file(f);

    return ret;
}

/** @brief A function to empty a ParsedFile data structure.
  *
  * This function frees all memory allocated by a ParsedFile structure. After a
  * call to this function you do not have to call free() on the passed data.
  *
  * @param pfh A ParsedFileHANDLE to be freed.
  */
void free_parsed_file(ParsedFileHANDLE pfh)
{
    ParsedFile *pf=pfh;
    CalleeList *cl;

    if (!pf) return;
    if (!pf->callees) return;

    while (pf->callees)
    {
        cl = pf->callees;
        pf->callees = pf->callees->next;
        free(cl->name);
        free(cl);
    }

    free_parsed_file(pf->next);
    free(pf);
}

