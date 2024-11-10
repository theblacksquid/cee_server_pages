
#include "../src/libblacksquid.h"
#include <stdio.h>

void index_page(FILE *fileptr, char *page_title)
{
    #include "index.h"
}

int main()
{
    index_page(stdout, "WELCOME TO CEE SERVER PAGES");
    index_page(stdout, "This is an example");
    index_page(stdout, "did it work?");
    
    return 0;
}
