#include <stdio.h>
#include <stdlib.h> //for atoi
#include "opt_file.h"
#include "conversion.h"
#include "bugs.h"

int main(int argc, char *argv[])
{
    char *input_file_name = "test.txt";
    int t_support = 3;
    int t_confidence = 65;
    FILE *input_file;
    char buf[LINE_LENGTH_BUFFER_SIZE];
    ParsedFileHANDLE pf;
    CallingMapHANDLE cm;
    BugListHANDLE bl;

    if (argc > 1)
        input_file_name = argv[1];
    if (argc > 2)
        t_support = atoi(argv[2]);
    if (argc > 3)
        t_confidence = atoi(argv[3]);

    input_file = fopen(input_file_name, "r");
    fgets(buf, LINE_LENGTH_BUFFER_SIZE, input_file);//get rid of entry point

    while (buf[0]!='\n') //get rid of <null function>
        fgets(buf, LINE_LENGTH_BUFFER_SIZE, input_file);

    pf = parse_opt_file(input_file);
    cm = calling_map_from_parsed_file(pf);

    if (argc > 4)
        print_calling_map(cm);

    bl = generate_bug_list(cm,t_support,t_confidence);
    print_bug_list(bl,cm);

    //TO DO: free buglist
    free_calling_map(cm);
    free_parsed_file(pf);
    return 0;
}
