
#define LIBBLACKSQUID_IMPLEMENTATION
#include "libblacksquid.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

ltbs_cell *cee_template_compile(ltbs_cell *template, Arena *context);
typedef void (*template_byte_handler)(ltbs_cell **workpiece, char c, Arena *context);

void append_byte_literal(ltbs_cell **workpiece, char c, Arena *context);
void append_byte_cee(ltbs_cell **workpiece, char c, Arena *context);
void begin_byte_literals(ltbs_cell **workpiece, Arena *context);
void end_byte_literals(ltbs_cell **workpiece, Arena *context);

ltbs_cell *serialize_formatted_output(ltbs_cell *workpiece, Arena *context);

int main()
{
    Arena *context = malloc(sizeof(Arena));
    *context = (Arena) {0};
    
    int length = 0;
    fseek(stdin, 0L, SEEK_END);
    length = ftell(stdin);
    rewind(stdin);

    char *buffer = arena_alloc(context, length);

    for ( int index = 0; index < length + 1; index++ )
        buffer[index] = 0;

    fread(buffer, 1, length, stdin);

    ltbs_cell input_cell =
    {
	.type = LTBS_STRING,
	.data =
	{
	    .string =
	    {
		.length = length,
		.strdata = buffer
	    }
	}
    };

    ltbs_cell *result = cee_template_compile(&input_cell, context);

    fprintf(stdout, "%s\n", result->data.string.strdata);
    
    arena_free(context);
    free(context);
}

ltbs_cell *cee_template_compile(ltbs_cell *template, Arena *context)
{
    ltbs_cell *workpiece = List_Vt.nil();
    ltbs_cell *reversed;
    int length = template->data.string.length;
    char *buffer = template->data.string.strdata;
    int last_char_is_open_curly = 0;
    int last_char_is_close_curly = 0;
    int is_c_mode = 0;
    int to_escape = 0;

    begin_byte_literals(&workpiece, context);

    for ( int index = 0; index < length; index++ )
    {
	template_byte_handler handler = append_byte_literal;
	if ( is_c_mode ) handler = append_byte_cee;
	
	switch ( buffer[index] )
	{   
	    case '{':
	    {
		if ( last_char_is_open_curly && !is_c_mode )
		{
		    last_char_is_open_curly = 0;
		    is_c_mode = 1;
		    end_byte_literals(&workpiece, context);
		}

		else if ( !to_escape )
		{
		    last_char_is_open_curly = 1;
		}

	        if ( to_escape && !is_c_mode )
		{
		    append_byte_literal(&workpiece, '{', context);
		    to_escape = 0;
		    last_char_is_open_curly = 0;
		}
	    }
	    break;

	    case '}':
	    {
		if ( last_char_is_close_curly && is_c_mode )
		{
		    last_char_is_close_curly = 0;
		    is_c_mode = 0;
		    begin_byte_literals(&workpiece, context);
		}

		else if ( !to_escape )
		{
		    last_char_is_close_curly = 1;
		}

	        if ( to_escape && !is_c_mode )
		{
		    append_byte_literal(&workpiece, '}', context);
		    to_escape = 0;
		    last_char_is_open_curly = 0;
		}
	    }
	    break;

	    case '\\':
	    {
		if ( to_escape ) handler(&workpiece, '\\', context);
		
		to_escape = !to_escape;
	    }
	    break;

	    default:
	    {
		if ( last_char_is_open_curly )
		{
		    handler(&workpiece, '{', context);
		    last_char_is_open_curly = 0;
		}

		if ( last_char_is_close_curly )
		{
		    handler(&workpiece, '}', context);
		    last_char_is_close_curly = 0;
		}

	        handler(&workpiece, buffer[index], context);
		to_escape = 0;
	    }
	}
    }

    end_byte_literals(&workpiece, context);
    reversed = List_Vt.reverse(workpiece, context);

    return serialize_formatted_output(workpiece, context);
}

void begin_byte_literals(ltbs_cell **workpiece, Arena *context)
{
    ltbs_cell *to_add = String_Vt.cs("\nfprintf(fileptr, \"", context);
    *workpiece = List_Vt.cons(to_add, *workpiece, context);
}

void end_byte_literals(ltbs_cell **workpiece, Arena *context)
{
    ltbs_cell *to_add = String_Vt.cs("\");\n", context);
    *workpiece = List_Vt.cons(to_add, *workpiece, context);
}

void append_byte_literal(ltbs_cell **workpiece, char c, Arena *context)
{
    char *buffer = arena_alloc(context, 10);

    for (int index = 0; index < 10; index++)
	buffer[index] = 0;
    
    FILE *as_file = fmemopen(buffer, 10, "w");
    fprintf(as_file, "\\x%02x", c);
    fclose(as_file);
    
    ltbs_cell *to_add = String_Vt.cs(buffer, context);
    *workpiece = List_Vt.cons(to_add, *workpiece, context);
}

void append_byte_cee(ltbs_cell **workpiece, char c, Arena *context)
{
    char *buffer = arena_alloc(context, 2);
    buffer[0] = c;
    buffer[1] = 0;
    
    ltbs_cell *to_add = String_Vt.cs(buffer, context);
    *workpiece = List_Vt.cons(to_add, *workpiece, context);    
}

ltbs_cell *serialize_formatted_output(ltbs_cell *workpiece, Arena *context)
{
    Arena *workspace = malloc(sizeof(Arena));
    *workspace = (Arena) {0};
    ltbs_cell *result;
    ltbs_cell *string_builder = String_Vt.cs("", workspace);

    {
	for ( ltbs_cell *tracker = workpiece;
	      List_Vt.head(tracker);
	      tracker = List_Vt.rest(tracker) )
	{
	    ltbs_cell *head = List_Vt.head(tracker);
	    string_builder = String_Vt.append(head, string_builder, workspace);
	}
    }

    result = String_Vt.copy(string_builder, context);

    arena_free(workspace);
    free(workspace);

    return result;
}
