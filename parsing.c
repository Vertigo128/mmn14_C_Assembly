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
    if (direct == 3 || direct == 4) line->num_words = 0;
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
        target->flag = 1;
        return 1;
    }
    else if (CUR_CHAR=='&'){
        target->type = 2;
        target->flag = 1;
        (*ptr)++;
    };
    if (!read_next_word(ptr, line, &str)) return 0;
    label_group = check_label (str);
    if (label_group==3) {
        if (target->type == 2) {print_error (ERROR_LABEL_NAME_IDENTICAL_REG_MAME,line); return 0;} 
        target->type = 3;
        target->flag = 1;
    }
    else if (label_group==0){
        if  (target->type != 2) {target->type = 1, target->flag = 1;}
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
        if (line->op_dst.type!=3) line->num_words++;
        if (expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_EOL,line); return 0;}
        return 1;
    case 2:
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_NONE_EOL,line); return 0;}
        if (!read_operand(ptr, line, &line->op_src)) { return 0;}
        if (line->op_src.type!=3) line->num_words++;
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_MISSING,line); return 0;}
        if (CUR_CHAR != ',') {print_error (ERROR_OPERATION_MISSING_COMMA,line); return 0;}
        (*ptr)++;
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_MISSING,line); return 0;}
        if (!read_operand(ptr, line, &line->op_dst)) { return 0;}
        if (line->op_dst.type!=3) line->num_words++;
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
    line->op_dst.flag = 0;
    line->op_src.flag = 0;
    line->op_src.value = 0;
    line->op_dst.value = 0;
    line->op_src.name = NULL;
    line->op_dst.name = NULL;
    line->label_flag = 0;
    line->data = NULL;
    line->str = NULL;
    line->num_words = 1;
    line->error.failed =0;
    line->directive_type=0;
    line->op_type=0;
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
void update_symbol (symbol** symbol_table, line* line,int DC, char* target, int type) {
	
    if (*symbol_table == NULL) {
		*symbol_table = calloc(1, sizeof(symbol));

		/* Check if calloc failed */
		if (*symbol_table == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		/* Put new values in list node */
		(*symbol_table)->value = DC;
        (*symbol_table)->type = type;
        (*symbol_table)->name = str_dup (target);
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
        ptr->next->type = type;
        ptr->next->name = str_dup (target);
        ptr->next->next = NULL;
	}
}

/*Add current line data numbers to data image table
return the new DC counter*/
int add_data_image (list* cur_line_data, data_image** data_img, int DC){
    int end_flag = 0;
    if (*data_img == NULL) {
		*data_img = calloc(1, sizeof(data_image));

		/* Check if calloc failed */
		if (*data_img == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
        
        (*data_img)->counter = DC++;
        (*data_img)->type = 1;
        (*data_img)->value = int_dup( cur_line_data->value);
        (*data_img)->next = NULL;
        if (cur_line_data->next != NULL)  cur_line_data = cur_line_data->next;
        else end_flag = 1;
    

    }
    data_image* ptr = *data_img;
    

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    while (end_flag == 0) {
            ptr->next = calloc(1, sizeof(symbol));

            /* Check if calloc failed */
            if (ptr->next == NULL) {
                fprintf(stderr, ERROR_OUT_OF_MEMORY);
                exit(1);
            }
            ptr->next->counter = DC++;
            ptr->next->type = 1;
            ptr->next->value = int_dup( cur_line_data->value);
            // ptr->next->next = NULL;
            ptr = ptr->next;
            
            if (cur_line_data->next != NULL)  cur_line_data = cur_line_data->next;
            else break;
    }
return DC;
  
}
    

/*Add current line string to data image table
return the new DC counter*/
int add_string_image (char* cur_line_string, data_image** data_img, int DC){
    int end_flag = 0;
    if (*data_img == NULL) {
		*data_img = calloc(1, sizeof(data_image));

		/* Check if calloc failed */
		if (*data_img == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
        
        (*data_img)->counter = DC++;
        (*data_img)->type = 2;
        (*data_img)->value = charecter_dup(*cur_line_string);
        cur_line_string++;

    }

    data_image* ptr_img = *data_img;
    while (ptr_img->next != NULL) {
        ptr_img = ptr_img->next;
    }
    while (*cur_line_string != '\0'){
            ptr_img->next = calloc(1, sizeof(data_image));

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
    }
    ptr_img->next = calloc(1, sizeof(data_image));
    if (ptr_img->next == NULL) { /* Check if calloc failed */
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(1);
    }
    ptr_img->next->counter = DC++;
    ptr_img->next->type = 2;
    ptr_img->next->value = charecter_dup('0');
    ptr_img->next->next = NULL;
    return DC;
    
}




/* Function to free the symbol table memory*/
void freelist_symbol (symbol* *ptr){
    symbol* temp_ptr;
    while (*ptr){
        temp_ptr = *ptr;
        *ptr = temp_ptr->next;
        free (temp_ptr);

    }

}

/* Function to free the data table memory*/
void freelist_data (data_image* *ptr){
    data_image* temp_ptr;
    while (*ptr){
        temp_ptr = *ptr;
        *ptr = temp_ptr->next;
        free (temp_ptr);

    }

}

/* Function to free the code table memory*/
void freelist_code (generated_code* *ptr){
    generated_code* temp_ptr;
    while (*ptr){
        temp_ptr = *ptr;
        *ptr = temp_ptr->next;
        free (temp_ptr);

    }

}

/* Find the funct and op_code of current command */
void find_funct_opcode (char* str, int* funct, int* op_code) {
    int i;
    for (i=0;i<NUM_OPS;i++){
        if (!strcmp(str,ops[i])) {
            if (i==0) {*funct = 0; *op_code = 0; return;}
            else if (i==1) {*funct = 0; *op_code = 1; return;}
            else if (i==2) {*funct = 1; *op_code = 2; return;}
            else if (i==3) {*funct = 2; *op_code = 2; return;}
            else if (i==4) {*funct = 0; *op_code = 4; return;}
            else if (i==5) {*funct = 1; *op_code = 5; return;}
            else if (i==6) {*funct = 2; *op_code = 5; return;}
            else if (i==7) {*funct = 3; *op_code = 5; return;}
            else if (i==8) {*funct = 4; *op_code = 5; return;}
            else if (i==9) {*funct = 1; *op_code = 9; return;}
            else if (i==10) {*funct = 2; *op_code = 9; return;}
            else if (i==11) {*funct = 3; *op_code = 9; return;}
            else if (i==12) {*funct = 0; *op_code = 12; return;}
            else if (i==13) {*funct = 0; *op_code = 13; return;}
            else if (i==14) {*funct = 0; *op_code = 14; return;}
            else  {*funct = 0; *op_code = 15; return;}
        
         }
    }

}


/* Write the first word code to the code table struct */
int generate_first_word_code (line* line){
    unsigned int ARE = 1<<2;
    unsigned int funct;
    unsigned int reg_src = 0;
    unsigned int address_src =0 ;
    unsigned int reg_dst = 0 ;
    unsigned int address_dst = 0;
    unsigned int opcode;
    unsigned int word;

    find_funct_opcode (line->str, &funct, &opcode);
    // if (line->op_src.type == 3) reg_src = find_reg(line->op_src.name);
    if (line->op_src.type == 3) reg_src = (int) (line->op_src.name[1]) - 48;
    if (line->op_dst.type == 3) reg_dst = (int) (line->op_dst.name[1]) - 48;
    address_src = line->op_src.type;
    address_dst = line->op_dst.type;
    reg_src <<= 13;
    reg_dst <<= 8;
    funct <<= 3;
    address_src <<= 16;
    address_dst <<= 11;
    opcode <<=18;
    word = ARE | funct | reg_dst | address_dst | reg_src | address_src | opcode; 
    return word;
}

/* Helper Function to print to screen current word*/
void print_word (int word){
    int j=23;
    int i;

    for (i =1 << 23; i ; i>>=1, j--){
        printf ("%c", word&i? '1' : '0');
        // if (j==3 || j== 8 || j==11 || j== 13 || j==16 || j==18) printf ("|"); 
    }
    // printf ("\n");
}


/* write the generated code in the next available code list , advance IC by 1 */
void write_first_word_code (unsigned int first_word,generated_code** code_ptr,int* IC){
    if (*code_ptr == NULL) {
		*code_ptr = calloc(1, sizeof(symbol));

		/* Check if calloc failed */
		if (*code_ptr == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		/* Put new values in list node */
		(*code_ptr)->word = first_word;
        (*code_ptr)->counter = *IC;
        (*code_ptr)->next = NULL;
        (*IC)++;

	} else {
        generated_code* ptr = *code_ptr;

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

		ptr->next->word = first_word;
        ptr->next->counter = *IC;
        ptr->next->next = NULL;
        (*IC)++;
    }

}


/* write the immidite number code in the next available code list, advance IC by 1 */
void write_code_number (unsigned int number,generated_code** code_ptr,int* IC){

    int ARE = 1<<2;
    int calculated_num = (number << 3 | ARE);
    

    generated_code* ptr = *code_ptr;

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

    ptr->next->word = calculated_num;
    ptr->next->counter = *IC;
    ptr->next->next = NULL;
    (*IC)++;
   
}

/* add empty code line for second pass, advance IC by 1 */
void write_code_skip (generated_code** code_ptr,int* IC){

    generated_code* ptr = *code_ptr;

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

    ptr->next->counter = *IC;
    ptr->next->next = NULL;
    (*IC)++;
   
}


/* Debug helper function to print generated tables */
void print_tables (symbol* symbol_table_ptr, data_image* data_ptr, generated_code* code_ptr, external* external_ptr){
    
    while (symbol_table_ptr!=NULL) {
        printf ("\n Symbols: val : %d, name : %s, type : %d, type_e : %d", symbol_table_ptr->value, symbol_table_ptr->name, symbol_table_ptr->type, symbol_table_ptr->type_e);
        if (symbol_table_ptr->next !=NULL) symbol_table_ptr= symbol_table_ptr ->next;
        else break;
    }
    while (data_ptr!=NULL) {
        printf ("\n Data: type : %d, counter : %d", data_ptr->type, data_ptr->counter);
        if (data_ptr->type==1) printf (" values is : %ld", *(long int*) data_ptr->value);
        else if (data_ptr->type==2) printf (" values is : %c ", *(char*) data_ptr->value);
        if (data_ptr->next !=NULL) data_ptr = data_ptr->next;
        else break;
    }

    while (code_ptr!=NULL) {
        printf ("\n Code: counter : %d, code :  " , code_ptr->counter);
        print_word (code_ptr->word);
        if (code_ptr->next !=NULL) code_ptr = code_ptr->next;
        else break;
    }

    while (external_ptr!=NULL) {
        printf ("\n External: counter : %d, code : %s " , external_ptr->value, external_ptr->name);
        if (external_ptr->next !=NULL) external_ptr = external_ptr->next;
        else break;
    }
    printf ("\n");

}


/* update symbol data table to add ICF to every data label value */
void update_symbol_first_pass (symbol** symbol_table, int ICF) {
	
		symbol* ptr = *symbol_table;
		while (ptr->next != NULL) {
            if (ptr->type == 1 || ptr->type == 2) ptr->value += ICF;
			ptr = ptr->next;
		}
        if (ptr->type == 1) ptr->value += ICF;
	
}

/* update symbol data entry flag 
return 0 if no symbol found, 1 if pass */
int update_symbol_entry (symbol** symbol_table, char* str,int line, char* file){
    symbol* ptr = *symbol_table;
    int found_flag = 0;
    while (ptr->next != NULL) {
            if (!strcmp(str,ptr->name)) {
                ptr->type_e = 1;
                found_flag = 1;
            }
			ptr = ptr->next;
		}
    if (found_flag==0) {fprintf (stderr, ERROR_ENTRY_LABEL_NOT_FOUND, line, file ); return 0;}
    return 1;

}

/* find current operand label in symbol table  
return 0 if no symbol found, -1 if label is external and adress counter if found */
int find_symbol_address (symbol** symbol_table, char* str){
    
    symbol* ptr = *symbol_table;

    while (ptr->next != NULL) {
        
        if (!strcmp(str,ptr->name)) {
            if (ptr->type != 4) return (ptr->value);
            else return -1;
        }
        ptr = ptr->next;
	}
    if (!strcmp(str,ptr->name)) {
            if (ptr->type != 4) return (ptr->value);
            else return -1;
        }
    return 0;
}


/* Create and update external table */
void update_external_table (external** external_ptr, int IC, char* str) {
    if (*external_ptr == NULL) {
		*external_ptr = calloc(1, sizeof(external));

		/* Check if calloc failed */
		if (*external_ptr == NULL) {
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		/* Put new values in list node */
		(*external_ptr)->name = str_dup (str);
        (*external_ptr)->value = IC;

	} else {
        external* ptr = *external_ptr;

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

		ptr->next->name = str_dup (str);
        ptr->next->value = IC;
        ptr->next->next = NULL;
    }

}


/* write the second pass missing labels code in the next relevant code list , advance IC by 1 */

void write_second_word (int symbol_address,int IC, int address_type, generated_code** code_ptr){
    
    int ARE;
    int calculated_num;
    if (symbol_address == -1) calculated_num = 1;
    else {
        ARE = 1<<address_type;
        calculated_num =  (symbol_address<<3 | ARE);
    }
    
    generated_code* ptr = *code_ptr;

    while (ptr->counter != IC) {
        ptr = ptr->next;
    }

    ptr->word = calculated_num;

}


void print_code (generated_code* ptr_code, data_image* data_ptr, int ICF){

    while (data_ptr!=NULL) {
        printf ("\n %7d ", (data_ptr->counter + ICF));
        printf ("0x%6lx", *(unsigned long int*) data_ptr->value);
        if (data_ptr->next !=NULL) data_ptr = data_ptr->next;
        else break;
    }
    return;
}

int main (){
    char* filename = "test_second_pass1";
    FILE *fd1;


    char str[Max_Line_Length +1 ] ;
    char *ptr = str;
    line cur_line;
    int DC = 0;
    int IC = CODE_START;
    unsigned int first_word; 
    int i,j;
    int ICF,DCF;
    int error_flag = 0;
    generated_code* code_ptr = NULL;
    data_image* data_ptr = NULL;
    symbol* symbol_table_ptr =NULL;
    external* external_table_ptr = NULL;

    printf ("Enter String:\n");
    freelist_symbol (&symbol_table_ptr);
    freelist_data (&data_ptr);
    freelist_code (&code_ptr);

    if (!(fd1 = fopen (filename,"r"))){
        printf ("error");
        exit (0);
    }
    

    while (ptr = fgets (str,Max_Line_Length+1,fd1)) {
        printf ("%s", ptr);
        initilize_line (&cur_line); /*TO DO ***** UPDATE LINE NUMBER*/

        if (read_next_line (&ptr, &cur_line)) {
            if (cur_line.directive_type==1 || cur_line.directive_type==2) {
                if (cur_line.label_flag==1) {
                    if (!check_dup_label(cur_line.label_name, symbol_table_ptr)) {print_error (ERROR_LABEL_EXIST,&cur_line); continue;}
                    update_symbol(&symbol_table_ptr,&cur_line,DC, cur_line.label_name,cur_line.directive_type);
                }
                if (cur_line.directive_type==1){
                    DC = add_data_image (cur_line.data, &data_ptr, DC);
                }
                if (cur_line.directive_type==2){
                    DC = add_string_image (cur_line.str, &data_ptr, DC);
                }
            }
            else if (cur_line.directive_type==3) continue;
            else if (cur_line.directive_type==4) update_symbol(&symbol_table_ptr,&cur_line,FIRST_PASS_EXTERN_VAL, cur_line.str,cur_line.directive_type);
            else { /*Code command */
                if (cur_line.label_flag==1) {
                    if (!check_dup_label(cur_line.label_name, symbol_table_ptr)) {print_error (ERROR_LABEL_EXIST,&cur_line); continue;}
                    update_symbol(&symbol_table_ptr,&cur_line,IC, cur_line.label_name,SYMBOL_DIRECTIVE_CODE);
                }
                first_word =  generate_first_word_code (&cur_line);
                // print_word (first_word);
                write_first_word_code (first_word,&code_ptr, &IC); 
                if (cur_line.num_words==1);
                else if (cur_line.op_src.flag==0){
                    if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) write_code_skip(&code_ptr, &IC);
                    if (cur_line.op_dst.type==0) write_code_number(cur_line.op_dst.value, &code_ptr, &IC);
                }
                else if (cur_line.op_src.flag==1){
                    if (cur_line.op_src.type==1 || cur_line.op_src.type==2) write_code_skip(&code_ptr, &IC);
                    if (cur_line.op_src.type==0) write_code_number(cur_line.op_src.value, &code_ptr, &IC);
                    
                    if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) write_code_skip(&code_ptr, &IC);
                    if (cur_line.op_dst.type==0) write_code_number(cur_line.op_dst.value, &code_ptr, &IC);
                }
                 

            }

        }
        if (cur_line.error.failed == 1) error_flag = 1; 
    }
    if (error_flag==1) return 1;
    ICF = IC;
    DCF = DC;
    update_symbol_first_pass (&symbol_table_ptr, ICF);
    print_tables(symbol_table_ptr,data_ptr, code_ptr,external_table_ptr);

    /*Second Pass */
    rewind (fd1);
    IC = CODE_START;

    while (ptr = fgets (str,Max_Line_Length+1,fd1)) {
        initilize_line (&cur_line); /*TO DO ***** UPDATE LINE NUMBER*/
        read_next_line (&ptr, &cur_line);
        // if (cur_line.label_flag==1) continue;
        if (cur_line.directive_type==1 || cur_line.directive_type==2 || cur_line.directive_type == 4) continue;
        else if (cur_line.directive_type==3) {
            if (!update_symbol_entry(&symbol_table_ptr, cur_line.str,cur_line.line_num, cur_line.filename));
        }
        else { /*Code only */
            if (cur_line.num_words == 1) {IC++; continue;}
            if (cur_line.op_src.flag==0 || cur_line.op_src.type==3 ){ 
                if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) {
                    int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_dst.name);
                    if (!symbol_address) {
                        fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename);
                        printf (ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename);
                        break;
                    }
                    else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+1, cur_line.op_dst.name);};
                    if (cur_line.op_dst.type==2) symbol_address = symbol_address - IC;
                    write_second_word(symbol_address,IC+1,cur_line.op_dst.type, &code_ptr);
                    
                }
            }
            else { /*Two additional words */
                if (cur_line.op_src.type==1 || cur_line.op_src.type==2) {
                    int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_src.name);
                    if (!symbol_address) {
                        fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename); 
                        printf (ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename);
                        break;}
                    else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+1, cur_line.op_src.name);};
                    if (cur_line.op_src.type==2) symbol_address = symbol_address - IC;
                    write_second_word(symbol_address,IC+1,cur_line.op_src.type, &code_ptr);
                    
                }
                if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) {
                    int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_dst.name);
                    if (!symbol_address) {
                        fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename); 
                        printf ( ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename); 

                        break;}
                    else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+2, cur_line.op_dst.name);};
                    if (cur_line.op_dst.type==2) symbol_address = symbol_address - IC;
                    write_second_word(symbol_address,IC+2,cur_line.op_dst.type, &code_ptr);
                }
            }
            
        }
        IC += cur_line.num_words;

    
    }
    print_tables(symbol_table_ptr,data_ptr, code_ptr, external_table_ptr);
    print_code (code_ptr, data_ptr, ICF);
    return 0;
}

