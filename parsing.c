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


/* Duplicates a long integer to allocated memory */
long int* int_dup (long int* num) {
	long int* num_cpy;

	if (!num)
		return NULL;

	num_cpy = malloc(sizeof((long int) num));
	if (!num_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}
    *num_cpy = (long int) *num;

	return num_cpy;
}

/* Duplicates a string to allocated memory */
char* charecter_dup (char str) {
	char* str_cpy;

	str_cpy = malloc(sizeof(char));
	if (!str_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}

	*str_cpy =  str;

	return str_cpy;
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
    for (i=0; i<NUM_DIRECTIVE; i++){
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


/*Read next word and store at str pointer.
return 0 if failed, 1 if passed*/

int read_next_word (char** ptr,line* line, char** str_temp){
    
    char str[MAX_Label_Length+1];
    int i=0;
    
    if (isdigit(CUR_CHAR)) {print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line); return 0;}
    if (!expect_non_EOL_WS(ptr)) {print_error(ERROR_LABEL_ON_EMPTY,line); return 0;}
    if (CUR_CHAR =='\"') {print_error(ERROR_LABEL_NO_BRACKET,line); return 0;}
    while (isalnum (CUR_CHAR)) {
        str[i] = CUR_CHAR;
        i++;
        (*ptr)++;
        if (i>MAX_Label_Length) {print_error(ERROR_LABEL_NAME_TOO_LONG, line); return 0;}
    }
    str[i]='\0';
    *str_temp = str_dup(str);
    
    return 1;

}



/*Check if label identical to system words,
return 0 if failed, 1 if operation, 2 if directive, 3 if registery*/ 
int check_label (char* str){
    int i;
    for (i=0; i<NUM_OPS;i++) if (!strcmp(str,ops[i])) return 1;
    for (i=0; i<NUM_DIRECTIVE;i++) if (!strcmp(str,directive[i])) return 2;
    for (i=0; i<NUM_REGS;i++) if (!strcmp(str,regs[i])) return 3;
    return 0;
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
            long int* temp_num_ptr;
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
	        
            temp_num_ptr = int_dup(&temp_num);
            list_add_element (&line->data, temp_num_ptr);
        }while (comma_flag==1);
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
            return 1;

        }
    default:
        {
            int word_type;
            char str[MAX_Label_Length+1];
            char* str_ptr = str;
            int i=0;

            if (isdigit(CUR_CHAR)) {print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line); return 0;}
            if (!expect_non_EOL_WS(ptr)) {print_error(ERROR_LABEL_ON_EMPTY,line); return 0;}
            if (CUR_CHAR =='\"') {print_error(ERROR_LABEL_NO_BRACKET,line); return 0;}
            while (isalnum (CUR_CHAR)) {
                str[i++] = CUR_CHAR;
                (*ptr)++;
                if (i>MAX_Label_Length) {print_error(ERROR_LABEL_NAME_TOO_LONG, line); return 0;}
            }
            str[i]='\0';
            if (expect_non_EOL_WS(ptr)) {print_error(ERROR_LABEL_INVALID_FORMAT,line);return 0;}
                if ((word_type = check_label(str))) {
                    if (word_type==1) {print_error(ERROR_LABEL_NAME_IDENTICAL_OP_NAME,line);return 0;}
                    if (word_type==2) {print_error(ERROR_LABEL_NAME_IDENTICAL_DIR_MAME,line);return 0;}                        if (word_type==3) {print_error(ERROR_LABEL_NAME_IDENTICAL_REG_MAME,line);return 0;}
                }
            else {
                line->str = str_dup(str);
                return 1;
            }
        }
    }


}



/*Read current label and store results,
return 1 if label valid and 0 if failed*/ 
int read_label (char** ptr, line* line){
    char str[MAX_Label_Length+1];
    int i = 0;
    int word_type;
    if (isdigit(CUR_CHAR)) {print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line); return 0;}
    while (isalnum (CUR_CHAR)) {
        str[i++] = CUR_CHAR;
        (*ptr)++;
        if (i>MAX_Label_Length) {print_error(ERROR_LABEL_NAME_TOO_LONG, line); return 0;}
    }
    if (CUR_CHAR != ':') {print_error(ERROR_LABEL_NAME_NOT_LETTER_OR_NUM,line);return 0;}
    str[i]= '\0';
    if ((word_type = check_label(str))) {
        if (word_type==1) {print_error(ERROR_LABEL_NAME_IDENTICAL_OP_NAME,line);return 0;}
        if (word_type==2) {print_error(ERROR_LABEL_NAME_IDENTICAL_DIR_MAME,line);return 0;}
        if (word_type==3) {print_error(ERROR_LABEL_NAME_IDENTICAL_REG_MAME,line);return 0;}
    }
    else {
        line->label_name = str_dup(str);
        line->label_flag = 1;
        return 1;
    }

}


/*Check if operation is valid and to which group it belongs
/*return 3 if not valid, 1 if one operand, 2 if two operands and 0 if no operands*/
int check_oper_group (char* str){
    int i;
    for (i=0;i<NUM_OPS;i++){
        if (!strcmp(str,ops[i])) {
            if (i>=0 && i<=4) return 2;
            if (i>=5 && i<=13) return 1;
            if (i==14 || i==15) return 0;
         }
    }
    return 3;

}


/*read next operand and store results
return 0 if failed, 1 if passed */
int read_operand (char** ptr,line* line, operand* target){
    long int temp_num;
    int label_group;
    char* str;
    if (CUR_CHAR=='#'){
        (*ptr)++;
        if (isdigit(CUR_CHAR) || CUR_CHAR != '+' || CUR_CHAR != '-'); {
            if (!read_next_num(ptr, &temp_num)){
                if (CUR_CHAR=='\0' || CUR_CHAR=='\n') {print_error (ERROR_EXPECTED_NUM_EOL,line); return 0;}
                else if (CUR_CHAR==',') {print_error (ERROR_EXPECTED_NUM_COMMA,line); return 0;}
                else {print_error (ERROR_EXPECTED_NUM,line); return 0;}
            }
        }
        skip_space(ptr);
        if (CUR_CHAR == '.') {print_error (ERROR_EXPECTED_NUM_INT,line); return 0;}
        if (temp_num >= MAX_NUM_Value || temp_num <= MIN_NUM_Value) {print_error (ERROR_NUM_RANGE,line); return 0;}
        target->value = (int) temp_num;
        target->type = 0;
        return 1;
    }
    else if (CUR_CHAR=='&'){
        target->type = 2;
        (*ptr)++;
    };
    if (!read_next_word(ptr, line, &str)) return 0;
    label_group = check_label (str);
    if (label_group==3) {
        if (target->type == 2) {print_error (ERROR_LABEL_NAME_IDENTICAL_REG_MAME,line); return 0;} 
        target->type = 3;
    }
    else if (label_group==0){
        if  (target->type != 2) target->type = 1;
    } 
    else if (label_group == 2) {print_error (ERROR_LABEL_NAME_IDENTICAL_DIR_MAME,line); return 0;}
    else {print_error (ERROR_LABEL_NAME_IDENTICAL_OP_NAME,line); return 0;}
    target->name = str_dup(str);
    return 1;
}


/*read operands and store results
return 0 if failed, 1 if passed */
int read_operands (char** ptr,line* line){
    switch (line->num_oper)
    {
    case 0:
        if (expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_INVALID_FORMAT,line); return 0;} 
        return 1;
    case 1:
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_NONE_EOL,line); return 0;}
        if (!read_operand(ptr, line, &line->op_dst)) { return 0;}
        if (expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_EOL,line); return 0;}
        return 1;
    case 2:
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_NONE_EOL,line); return 0;}
        if (!read_operand(ptr, line, &line->op_src)) { return 0;}
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_MISSING,line); return 0;}
        if (CUR_CHAR != ',') {print_error (ERROR_OPERATION_MISSING_COMMA,line); return 0;}
        (*ptr)++;
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_MISSING,line); return 0;}
        if (!read_operand(ptr, line, &line->op_dst)) { return 0;}
        if (expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_EOL,line); return 0;}
        return 1;

    }
}

/*read current operation and check if valid, store results on line struct
return 0 if failed, 1 if passed */
int read_ops (char** ptr, line* line){
    char str[MAX_OPS_Char_Length+1];
    int i=0;
    int oper_group;
    while (isalpha(CUR_CHAR)){
        str[i++]=CUR_CHAR;
        (*ptr)++;
        if (i>MAX_OPS_Char_Length) {print_error (ERROR_OPERATION_INVALID_FORMAT,line); return 0;}
    }
    str[i] = '\0';
    
    line->num_oper = check_oper_group (str);
    if (line->num_oper==3) {print_error (ERROR_OPERATION_INVALID_FORMAT,line); return 0;}
    line->op_type = 1;
    line->str = str_dup (str);
    return 1;
}
    
/*return the address type group for current operation
return 0 if failed, unique value for each type */
int check_oper_address_group (char* str) {
    int i;
    for (i=0;i<NUM_OPS;i++){
        if (!strcmp(str,ops[i])) {
            if (i==0 || i==2 || i==3) return 1;
            if (i==1) return 2;
            if (i==4) return 3;
            if ((i>=5 && i<=8) || i==12) return 4;
            if (i>=9 && i<=11) return 5;
            if (i==13) return 6;
            if (i==14 || i==15) return 7;
         }
    }
    return 0;

}



/*read if operands address type is valid for current operation
return 0 if failed, 1 if passed */
int check_valid_operands (line* line){
    int src_t = line->op_src.type;
    int dst_t = line->op_dst.type;
    int oper_group;
    oper_group = check_oper_address_group(line->str);
    switch (oper_group)
    {
    case 1:
        if ((src_t==0 || src_t == 1 || src_t ==3) && (dst_t==1 || dst_t==3)) return 1;
        break;
    case 2:
        if ((src_t==0 || src_t == 1 || src_t ==3) && (dst_t==0 || dst_t==1 || dst_t==3)) return 1;
        break;
    case 3:
        if ((src_t == 1) && (dst_t==1 || dst_t==3)) return 1;
        break;
    case 4:
        if (dst_t==1 || dst_t==3) return 1;
        break;
    case 5:
        if (dst_t==1 || dst_t==2) return 1;
        break;
    case 6:
        if (dst_t==0 || dst_t==1 || dst_t == 3) return 1;
        break;
    case 7:
        return 1;
    default:
        return 0;
    }
    return 0;
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
            }
    }
    if (CUR_CHAR=='.'){
        (*ptr)++;
        if (!read_directive (ptr,line)) return 0;
        // printf ("Directive is: %d", line->directive_type);
        return 1;
    }
    if (!read_ops (ptr,line)) {return 0;}
    if (!read_operands (ptr,line)) {return 0;}
    if (!check_valid_operands(line)) {print_error (ERROR_OPERATION_INVALID_ADDRESS,line); return 0;}
    line->op_type = 1;
    return 1;
    

}

/* Initilize current line parsing struct */
void initilize_line (line* line){
    line->op_dst.type = 0;
    line->op_src.type = 0;
    line->label_flag = 0;
    line->data = NULL;
    line->str = NULL;
    line->op_src.name = NULL;
    line->op_dst.name = NULL;
    line->op_src.value = 0;
    line->op_dst.value = 0;
}

/* Initilize current line parsing struct */
void print_line (line* line){
    
    if (line->label_flag==1)  ("Label Flag is: %d, name is %s\n",line->label_flag, line->label_name);

    if (line->op_type==1) {
        printf ("Oper name: %s\n",line->str);
        printf ("Num ops: %d\n",line->num_oper);
        printf ("Src type: %d\n",line->op_src.type);        
        printf ("Dest type: %d\n",line->op_dst.type);
        printf ("Src value: %d\n",line->op_src.value);        
        printf ("Dest value: %d\n",line->op_dst.value);
        printf ("Src name: %s\n",line->op_src.name);        
        printf ("Dest name: %s\n",line->op_dst.name);

    }
    if (line->op_type==0){
        printf ("Directive_type: %d\n",line->directive_type);
        if (line->directive_type==1) printf ("First Num is : %ld\n",*(long int*) line->data->value);
        else printf ("string/directive lable is : %s\n",line->str);
    }

}



/* Check if current label already exist on symbol table
return 0 if found, return 1 if passed without duplication */
int check_dup_label (char* str, symbol* symbol_table) {
    symbol* ptr = symbol_table;
    if (ptr==NULL) return 1;
    while (ptr->next != NULL) {
        if (!strcmp(ptr->name,str)) return 0;
        ptr = ptr->next;
    }
    if (!strcmp(ptr->name,str)) return 0;
    return 1;
}

/* Adds an element to a linked list pointer, and if it doesn't exist (ie. NULL), creates it and assigned to pointer */
void update_symbol (symbol** symbol_table, line* line,int DC) {
	
    if (*symbol_table == NULL) {
		*symbol_table = calloc(1, sizeof(symbol));

		/* Check if calloc failed */
		if (*symbol_table == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		/* Put new values in list node */
		(*symbol_table)->value = DC;
        (*symbol_table)->type = line->directive_type;
        (*symbol_table)->name = str_dup (line->label_name);
        (*symbol_table)->next = NULL;

	} else {
		symbol* ptr = *symbol_table;

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
		ptr->next->value = DC;
        ptr->next->type = line->directive_type;
        ptr->next->name = str_dup (line->label_name);
        ptr->next->next = NULL;
	}
}

/*Add current line data numbers to data image table
return the new DC counter*/
int add_data_image (list* cur_line_data, data_image** data_img, int DC){


    if (*data_img == NULL) {
		*data_img = calloc(1, sizeof(data_image));

		/* Check if calloc failed */
		if (*data_img == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		// /* Put new values in list node */
        //     (*data_img)->counter = DC++;
        //     (*data_img)->type = 1;
        //     (*data_img)->value = int_dup( cur_line_data->value);
        //     (*data_img)->next = NULL;
        //     if (cur_line_data->next != NULL) cur_line_data = cur_line_data->next;

    }
    data_image* ptr = *data_img;

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    while (cur_line_data->next != NULL) {
            ptr->next = calloc(1, sizeof(symbol));

            /* Check if calloc failed */
            if (ptr->next == NULL) {
                fprintf(stderr, ERROR_OUT_OF_MEMORY);
                exit(1);
            }
            ptr->next->counter = DC++;
            ptr->next->type = 1;
            ptr->next->value = int_dup( cur_line_data->value);
            ptr->next = NULL;
            cur_line_data = cur_line_data->next;
    }

    ptr->next = calloc(1, sizeof(symbol));
    if (ptr->next == NULL) { /* Check if calloc failed */
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(1);
    }
    ptr->next->counter = DC++;
    ptr->next->type = 1;
    ptr->next->value = int_dup( cur_line_data->value);
    ptr->next = NULL;
    return DC;

    
}
    
    
    // data_image* ptr_img = data_img;
    // list* ptr_data_line = cur_line_data;
    // while (ptr_img->next != NULL) {
    //     ptr_img = ptr_img->next;
    // }
    // do{
    //         ptr_img->next = calloc(1, sizeof(symbol));

    //         /* Check if calloc failed */
    //         if (ptr_img->next == NULL) {
    //             fprintf(stderr, ERROR_OUT_OF_MEMORY);
    //             exit(1);
    //         }
    //         ptr_img->counter = DC++;
    //         ptr_img->type = 1;
    //         ptr_img->value = int_dup( ptr_data_line->value);
    //         ptr_img = ptr_img->next;
    //         ptr_data_line = ptr_data_line->next;
    // }while (ptr_data_line->next != NULL);
    // ptr_img->counter = DC++;
    // ptr_img->type = 1;
    // ptr_img->value = int_dup( ptr_data_line->value);
    // return DC;

    // }

/*Add current line string to data image table
return the new DC counter*/
int add_string_image (char* cur_line_string, data_image* data_img, int DC){
    data_image* ptr_img = data_img;

    while (ptr_img->next != NULL) {
        ptr_img = ptr_img->next;
    }
    do{
            ptr_img->next = calloc(1, sizeof(symbol));

            /* Check if calloc failed */
            if (ptr_img->next == NULL) {
                fprintf(stderr, ERROR_OUT_OF_MEMORY);
                exit(1);
            }
            ptr_img->next->counter = DC++;
            ptr_img->next->type = 2;
            ptr_img->next->value = charecter_dup(*cur_line_string);
            ptr_img = ptr_img->next;
            cur_line_string++;
    }while (*cur_line_string != '\0');
    ptr_img->counter = DC++;
    ptr_img->type = 2;
    ptr_img->value = charecter_dup('0');
    return DC;
    
}


int main (){
    char str[Max_Line_Length +1 ] ;
    char *ptr = str;
    line cur_line;
    int DC = 0;
    int IC = CODE_START;
    data_image* data_ptr = NULL;
    symbol* symbol_table_ptr =NULL;

    printf ("Enter String:\n");
    
    while (ptr = fgets (str,Max_Line_Length+1,stdin)) {
        printf ("%s", ptr);
        initilize_line (&cur_line);

        if (read_next_line (&ptr, &cur_line)) {
            print_line (&cur_line);
            if (cur_line.directive_type==1 || cur_line.directive_type==2) {
                if (cur_line.label_flag==1) {
                    if (!check_dup_label(cur_line.label_name, symbol_table_ptr)) {print_error (ERROR_LABEL_EXIST,&cur_line); continue;}
                    update_symbol(&symbol_table_ptr,&cur_line,DC);
                }
                if (cur_line.directive_type==1){
                    DC = add_data_image (cur_line.data, &data_ptr, DC);
                }
                // if (cur_line.directive_type==2){
                //     DC = add_string_image (cur_line.str, &data, DC);
                // }
            }
        }

    }
        
    return 0;
}

