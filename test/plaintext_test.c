
#include <stdio.h>

void print_formatted_text(FILE *fileptr, char *list[]);

int main()
{
    print_formatted_text(stdout, (char *[]) { "bread", "eggs", "milk", "vegetables", "spices" });
}

void print_formatted_text(FILE *fileptr, char *list[])
{
    #include "arbitrary_text.h"
}
