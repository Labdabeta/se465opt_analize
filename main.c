#include <stdio.h>
#include <stdlib.h> //for atoi
#include "opt_file.h"
#include "conversion.h"
#include "bugs.h"




int main(int argc, char *argv[])
{
    char *input_file_name = argv[1];
    
    FILE *input_file = fopen(input_file_name,"r");
    
    char buf[LINE_LENGTH_BUFFER_SIZE];
    fgets(buf,LINE_LENGTH_BUFFER_SIZE,input_file);//get rid of the entry point line

    while (buf[0]!='\n')
        fgets(buf,LINE_LENGTH_BUFFER_SIZE,input_file);//get rid of <null function>

    ParsedFileHANDLE pf = parse_opt_file(input_file);
    CallingMapHANDLE cm = calling_map_from_parsed_file(pf);

    print_calling_map(cm);
    printf("\n\n\n");

    BugListHANDLE bl = generate_bug_list(cm,3,65);
    print_bug_list(bl,cm);

    free_calling_map(cm);
    free_parsed_file(pf);
    return 0;
}
     
