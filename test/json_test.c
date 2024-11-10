#define LIBBLACKSQUID_IMPLEMENTATION 1
#include "../src/libblacksquid.h"
#include <stdio.h>

void render_json(FILE *fileptr, ltbs_cell *obj_list)
{
    #include "json_test.h"
}

int main()
{
    Arena context = {0};

    ltbs_cell *list = List_Vt.nil();

    char *names_list[] = {
	"storkvoyager",
	"agonizingbounce",
	"strengthmuch",
	"funfund",
	"dyogramconvince",
	"speculatebiathlon",
	"pelicanlength",
	"territorylice",
	"tomatoestamp",
	"fardagedisney",
	"quaintobviously"
    };

    for ( int index = 0; index < 11; index++ )
    {
	ltbs_cell *to_add = Hash_Vt.new(&context);

	hashmap_from_kvps(
	    to_add,
	    &context,
	    {"id", List_Vt.from_int(index, &context)},
	    {"username", String_Vt.cs(names_list[index], &context)}
	);

        list = List_Vt.cons(to_add, list, &context);
    }

    render_json(stdout, list);
    
    return 0;
}
