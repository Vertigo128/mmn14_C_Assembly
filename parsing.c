#include <stdio.h>
#include <string.h>
#include "parsing.h"
#include "data.h"
#include "errors.h"
#define CUR_CHAR **ptr


/* Adds an element to a linked list pointer, and if it doesn't exist (ie. NULL), creates it and assigned to pointer */
void list_add_element (list** linked_list, void* data) {
	if (data == NULL) {
		return;
	}

	if (*linked_list == NULL) {
		*linked_list = calloc(1, sizeof(list));

		/* Check if calloc failed */
		if (*linked_list == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		/* Put new values in list node */
		(*linked_list)->value = data;
	} else {
		list* ptr = *linked_list;

		/* Skip to last element */
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}

		ptr->next = calloc(1, sizeof(list));

		/* Check if calloc failed */
		if (ptr->next == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		/* Put new values in list node */
		ptr->next->value = data;
	}
}




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
    printf (type, line->line_num, line->filename );
}


/*search for a charecter in the next word, stop search at space/EOL*/
/*return 1 if charecter found, 0 if not*/
int search_word(char c, char* ptr){
    while (!isspace (*ptr) && *ptr != '\0' && *ptr != '\n' && *ptr != c) (ptr)++;
    if (*ptr == c) return 1;
    else return 0;
}


/*skip pointer to the next non-space, return the next non-space charecter*/
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

/*check if input directive is valid,
return directitve type, 0 if failed*/ 
int validate_directive (char* str){
    int i;
    for (i=0; i<MAX_FUNCTION; i++){
        if (!strcmp(str,directive[i])) return i+1;
    }
    return 0;

}

/*Read next number from ptr and skip ptr to next non-digit char,
return 1 if found and 0 if failed*/ 
int read_next_num (char** ptr, long int* temp_num){
    char* temp_ptr = *ptr;
    *temp_num = strtol (*ptr, &temp_ptr, 10);
    if (temp_ptr == *ptr ) return 0;
    *ptr = temp_ptr;
    return 1;
}

/*Read the directive string , move pointer to end of string and store results at line struct,
return 1 if found and 0 if failed*/ 
int read_string (char** ptr, char** line_str){
    char temp_str[Max_Line_Length];
    int i=0;
    while (CUR_CHAR != '\"' && isprint (CUR_CHAR)){
        temp_str[i++] = CUR_CHAR;
        (*ptr)++;
    }
    temp_str[i] = '\0';
    if ((i==0) || (CUR_CHAR != '\"')) return 0;
    *line_str = str_dup(temp_str);
    (*ptr)++;
    return 1;
}


/*Read current directive and store results,
return 1 if directive is valid and 0 if failed*/ 
int read_directive (char** ptr, line* line){
    int i=0;
    int direct;
    int comma_flag;
    char str[MAX_Directive_Length+1];
    while (isalpha (CUR_CHAR))  {
        str[i++] = CUR_CHAR;
        str[i]= '\0';
        (*ptr)++;
        if (validate_directive (str)) break;
        if (i>MAX_Directive_Length){print_error (ERROR_UNDEFINED_DIRECTIVE,line); return 0;}
    }
    if (!(direct = validate_directive (str))){print_error (ERROR_UNDEFINED_DIRECTIVE,line); return 0;}
    if(!isspace(CUR_CHAR)) {print_error (ERROR_EXPECTED_SPACE_DIRECTIVE,line); return 0;}
    line->op_type = 0;
    line->directive_type = (direct);
    skip_space(ptr);

    switch (line->directive_type)
    {
    case 1: /*reading data numbers*/
        do{
            long int temp_num,garbage_num;
            comma_flag = 0;
            skip_space(ptr);
            if (isdigit(CUR_CHAR) || CUR_CHAR != '+' || CUR_CHAR != '-'); {
                if (!read_next_num(ptr, &temp_num)){
                    if (CUR_CHAR=='\0' || CUR_CHAR=='\n') {print_error (ERROR_EXPECTED_NUM_EOL,line); return 0;}
                    else if (CUR_CHAR==',') {print_error (ERROR_EXPECTED_NUM_COMMA,line); return 0;}
                    else {print_error (ERROR_EXPECTED_NUM,line); return 0;}
                }
            }
            skip_space(ptr);
            if (read_next_num(ptr, &garbage_num)) {print_error (ERROR_MISSING_COMMA,line); return 0;}
            if (CUR_CHAR == '.') {print_error (ERROR_EXPECTED_NUM_INT,line); return 0;}
            if (CUR_CHAR != '\0' && CUR_CHAR != '\n' && CUR_CHAR != ','){print_error (ERROR_EXPECTED_NUM,line); return 0;}
            else if (CUR_CHAR == ','){
                comma_flag = 1;
                (*ptr)++;
            }
            if (temp_num>=MAX_Data_Value || temp_num<=MIN_Data_Value) {print_error (ERROR_NUM_RANGE,line); return 0;}
            list_add_element (&line->data, &temp_num);
        }while (comma_flag==1);
        // printf ("First num on data is : %ld", *(long int*) line->data->value);
        return 1;
    case 2: /*reading string */
        if (CUR_CHAR != '\"') {print_error (ERROR_EXPECTED_DOUBLE_BRACKET,line); return 0;}
        (*ptr)++;
        if (!read_string(ptr,&line->str)) {
            if (CUR_CHAR== '\"') {print_error (ERROR_EMPTY_STRING,line); return 0;}
            else if (CUR_CHAR== '\0' || CUR_CHAR == '\n') {print_error (ERROR_STRING_EOL,line); return 0;}
            else {print_error (ERROR_STRING_INVALID,line); return 0;}
            }
        else {
            skip_space(ptr);
            if (CUR_CHAR != '\0' && CUR_CHAR != '\n') {print_error (ERROR_STRING_EXPECTED_EOL,line); return 0;}
            printf ("Current STring is %s :", line->str);
            return 1;

        }
    default:
        break;
    }


}


/*Read current label and store results,
return 1 if label valid and 0 if failed*/ 
int read_label (char** ptr, line* line){
    char str[MAX_Label_Length+1];
    int i = 0;
    if (isdigit(CUR_CHAR)) {print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line); return 0;}
    while (isalnum (CUR_CHAR)) {
        str[i++] = CUR_CHAR;
        (*ptr)++;
        if (i>MAX_Label_Length) {print_error(ERROR_LABEL_NAME_TOO_LONG, line); return 0;}
    }
    if (CUR_CHAR != ':') {print_error(ERROR_LABEL_NAME_NOT_LETTER_OR_NUM,line);return 0;}
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
    if (strlen(*ptr)>Max_Line_Length) {print_error (ERROR_LINE_TOO_LONG,line); return 0;}
    skip_space(ptr);
    if (CUR_CHAR== '#') return 0;
    if (!expect_non_EOL_WS(ptr)) return 0;
    if (search_word(':',*ptr)) {
        if (read_label(ptr,line)) {
            (*ptr)++;
            if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_LABEL_EMPTY,line); return 0;}
            // printf ("label is : %s", line->label_name);
            }
    }
    if (CUR_CHAR='.'){
        (*ptr)++;
        if (!read_directive (ptr,line)) return 0;
        printf ("Directive is: %d", line->directive_type);
        return 1;
    }

}
int main (){
    char str[Max_Line_Length +1 ] ;
    // = "  Mylabel:   mov #11  , LAbal1232";
    char *ptr = str;
    
    line cur_line;
    cur_line.filename = "test1";
    cur_line.line_num = 0;

    printf ("Enter String:\n");
    while (ptr = fgets (str,Max_Line_Length+1,stdin)) {
        printf ("%s", ptr);
        cur_line.error.failed = 0;
        cur_line.label_flag = 0;
        // cur_line.data->next = NULL;
        // cur_line.data->value = NULL;
        cur_line.data = NULL;
        if (read_next_line (&ptr, &cur_line)) printf ("Passed\n");
    }
        
        


return 0;
}
