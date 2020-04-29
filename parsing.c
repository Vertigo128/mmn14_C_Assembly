#include <stdio.h>
#include <string.h>
#include "parsing.h"
#include "data.h"
#include "errors.h"
#define CUR_CHAR **ptr


/* Duplicates a string to allocated memory */
char* str_dup (char* str) {
	char* str_cpy;

	if (!str)
		return NULL;

	str_cpy = malloc(strlen(str) + 1);
	if (!str_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}

	strcpy(str_cpy, str);

	return str_cpy;
}


/* the function print input error and store error on current line struct*/
void print_error (char* type,line* line){
    line->error.failed = 1;
    line->error.err_code = type;
    fprintf (stderr, type, line->line_num, line->filename );
}


/*search for a charecter in the next word, stop search at space/EOL*/
/*return 1 if charecter found, 0 if not*/
int search_word(char c, char* ptr){
    while (!isspace (*ptr) && *ptr != '\0' && *ptr != '\n' && *ptr != c) (ptr)++;
    if (*ptr == c) return 1;
    else return 0;
}


/*skip pointer to the next whitespace, return the next non-space charecter*/
char skip_space(char** ptr){
    while (isspace (**ptr)) (*ptr)++;
    return **ptr;
}

/*skip pointer to the next whitespace, if EOL found return 0*/
int expect_non_EOL_WS (char** ptr){
    char c = skip_space(ptr);
    if (c=='\0' || c=='\n') return 0;
    return 1;
}
int read_label (char** ptr, line* line){
    char str[MAX_Label_Length+1];
    int i = 0;
    if (isdigit(CUR_CHAR)) print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line);
    while (isalpha (CUR_CHAR)) {
        str[i++] = CUR_CHAR;
        (*ptr)++;
        if (i>MAX_Label_Length) print_error(ERROR_LABEL_NAME_TOO_LONG, line);
    }
    if (CUR_CHAR != ':') print_error(ERROR_LABEL_NAME_NOT_LETTER_OR_NUM,line);
    if (line->error.failed==1) return 0;
    else {
        line->label_name = str_dup(str);
        line->label_flag = 1;
        return 1;
    }

}

/* main function to parse current line
return 0 if parsing failed, 1 if passed*/
int read_next_line (char** ptr, line* line){
    int i;
    if (strlen(*ptr)>Max_Line_Length) {print_error (ERROR_LINE_TOO_LONG,line);}
    if (CUR_CHAR== '#') return 0;
    if (!expect_non_EOL_WS(ptr)) return 0;
    if (search_word(':',*ptr)) {
        if (read_label(ptr,line)) ;
    }



}
int main (){
    char str[Max_Line_Length +1 ] = "  Mylabel:   mov #11  , LAbal1232";
    char *ptr = str;
    printf ("%s", str);
    line cur_line;
    cur_line.filename = "test1";
    cur_line.line_num = 0;
    cur_line.error.failed = 0;
    cur_line.label_flag = 0;
    printf ("Enter String:\n");
    if (read_next_line (&ptr, &cur_line)) printf ("Passed\n");


return 0;
}
