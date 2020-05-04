#include <stdio.h>
#include <string.h>
#include "parsing.h"
#include "data.h"
#include "errors.h"
#define CUR_CHAR **ptr /*Helper macro to input pointer*/


/* Adds an element to a linked list pointer, and if it doesn't exist (ie. NULL), creates it and assigned to pointer 
inputs - linked_list-> pointer to list struct, data -> pointer of data to add*/
void list_add_element (list** linked_list, void* data) {
	if (data == NULL) { /*check if data exist, exit if not*/
		return;
	}

	if (*linked_list == NULL) { /*check if linked list alread exist */
		*linked_list = calloc(1, sizeof(list));
		
		if (*linked_list == NULL) { /* Check if calloc failed */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		(*linked_list)->value = data; /* Put new value in the new list node */
	} else {
		list* ptr = *linked_list;

		
		while (ptr->next != NULL) { /* Skip to last element */
			ptr = ptr->next;
		}

		ptr->next = calloc(1, sizeof(list)); /* Create new last node */

		
		if (ptr->next == NULL) { /* Check if calloc failed */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		ptr->next->value = data; /* Put new values in last list node */
	}
}


/* Duplicates a long integer to allocated memory 
inputs - num-> number to copy
return the new pointer */
long int* int_dup (long int* num) {
	
    long int* num_cpy; /*temp variable to store the data*/

	if (!num) /*check if data exist, exit if not*/
		return NULL;

	num_cpy = malloc(sizeof((long int) num)); /* Create new pointer  */
	if (!num_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}
    *num_cpy = (long int) *num; /* Copy values to new pointer */

	return num_cpy;
}

/* Duplicates a string to allocated memory 
inputs - str-> char to copy
return the new pointer */
char* charecter_dup (char str) {
	char* str_cpy; /*temp variable to store the data*/

	str_cpy = malloc(sizeof(char)); /* Create new pointer */
	if (!str_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}

	*str_cpy =  str; /* Copy values to new pointer */

	return str_cpy;
}


/* Duplicates a string to allocated memory 
inputs - str-> string to cpy
return the new pointer */
char* str_dup (char* str) {
	char* str_cpy; /*temp variable to store the data*/

	if (!str)
		return NULL;

	str_cpy = malloc(strlen(str) + 1); /* Create new pointer */
	if (!str_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}

	strcpy(str_cpy, str); /* Copy values to new pointer */
	return str_cpy;
}


/* the function print input error and store error on current line struct
inputs - type-> type of error, line -> pointer to current line struct*/
void print_error (char* type,line* line){
    line->error.failed = 1; /* set error flag to 1 */
    line->error.err_code = type; /*store the error flag */
    fprintf (stderr, type, line->line_num, line->filename ); /*print error to stderr*/

}


/*search for a charecter in the next word, stop search at space/EOL
inputs - c-> charecter to find, ptr -> string to search
return 1 if charecter found, 0 if not*/
int search_word(char c, char* ptr){
    while (!isspace (*ptr) && *ptr != '\0' && *ptr != '\n' && *ptr != c) (ptr)++; /*search until EOL or charecter found*/
    if (*ptr == c) return 1; /*return 1 if found*/
    else return 0; /*return 0 if not found*/
}


/*skip pointer to the next non-space, return the next non-space charecter
inputs - ptr-> pointer to string*/
char skip_space(char** ptr){
    while (isspace (**ptr)) (*ptr)++; /*keep skiping until non-space char is found*/
    return **ptr; /*return the non-space charecter*/
}


/*skip pointer to the next whitespace, if EOL found return 0
inputs - ptr-> pointer to string*/
int expect_non_EOL_WS (char** ptr){
    char c = skip_space(ptr); /*get the next non-space charercter*/
    if (c=='\0' || c=='\n') return 0; /*return 0 if it is EOL*/
    return 1; /*return 1 if non-EOL*/
}


/*check if input directive is valid,
inputs - str-> string containing input directive
return directitve type, 0 if failed*/ 
int validate_directive (char* str){
    int i;
    for (i=0; i<NUM_DIRECTIVE; i++){
        if (!strcmp(str,directive[i])) return i+1;
    }
    return 0;

}


/*Read next number from ptr and skip ptr to next non-digit char,
inputs - ptr-> pointer to string, temp_num pointer to number to store results
return 1 if found and 0 if failed*/ 
int read_next_num (char** ptr, long int* temp_num){
    char* temp_ptr = *ptr; /*declare a temp pointer*/
    *temp_num = strtol (*ptr, &temp_ptr, 10); /*convert string to number*/
    if (temp_ptr == *ptr ) return 0; /*if pointer haven't moved, no number was converted - return 0*/
    *ptr = temp_ptr; /*move original pointer after the number*/
    return 1;
}


/*Read the directive string , move pointer to end of input string and store results at line struct,
inputs - ptr-> pointer to input string, line_str ->pointer to string location at line struct.
return 1 if found and 0 if failed*/ 
int read_string (char** ptr, char** line_str){
    char temp_str[Max_Line_Length]; /*create temp array to store string*/
    int i=0; /*counter to array*/
    while (CUR_CHAR != '\"' && isprint (CUR_CHAR)){ /*check that charectes are valid and no " end of string is reached*/
        temp_str[i++] = CUR_CHAR; /*store char*/
        (*ptr)++; /*advance pointer*/
    }
    temp_str[i] = '\0'; /*add EOL char*/
    if ((i==0) || (CUR_CHAR != '\"')) return 0; /*if no valid char found return 0*/
    *line_str = str_dup(temp_str); /*store results at line struct*/
    (*ptr)++; /*advance input pointer by 1*/
    return 1;
}


/*Read next word and store at str_temp pointer.
inputs - ptr-> pointer to input string, line ->pointer line struct, str_temp -> pointer to store next word
return 0 if failed, 1 if passed*/
int read_next_word (char** ptr,line* line, char** str_temp){
    
    char str[MAX_Label_Length+1]; /*create temp array to store string*/
    int i=0; /*counter to string length*/
    
    if (isdigit(CUR_CHAR)) {print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line); return 0;} /*Raise error if string start with digit*/
    if (!expect_non_EOL_WS(ptr)) {print_error(ERROR_LABEL_ON_EMPTY,line); return 0;} /*Raise error if line ended*/
    if (CUR_CHAR =='\"') {print_error(ERROR_LABEL_NO_BRACKET,line); return 0;} /*Raise error if " found */
    while (isalnum (CUR_CHAR)) { /*keep reading while alphanumeric chars found*/
        str[i] = CUR_CHAR; /*store results*/
        i++; /*advance counter*/
        (*ptr)++; /*advance input pointer*/
        if (i>MAX_Label_Length) {print_error(ERROR_LABEL_NAME_TOO_LONG, line); return 0;} /*raise error if label too long*/
    }
    str[i]='\0'; /*add EOF to end of string*/
    *str_temp = str_dup(str); /*store results at allocated memory pointer*/
    return 1;
}


/*Check if label identical to system words,
inputs - str-> string containing current label
return 0 if failed, 1 if operation, 2 if directive, 3 if registery*/ 
int check_label (char* str){
    int i;
    for (i=0; i<NUM_OPS;i++) if (!strcmp(str,ops[i])) return 1; /*check if identical to operation*/
    for (i=0; i<NUM_DIRECTIVE;i++) if (!strcmp(str,directive[i])) return 2; /*check if identical to directive*/
    for (i=0; i<NUM_REGS;i++) if (!strcmp(str,regs[i])) return 3; /*check if identical to registery*/
    return 0; /*return 0 if current label is not using system word*/
}


/*Read current directive and store results,
inputs - ptr-> pointer to input string, line ->pointer line struct
return 1 if directive is valid and 0 if failed*/ 
int read_directive (char** ptr, line* line){
    int i=0; /*temp counter*/
    int direct; /*store the directive type*/
    int comma_flag; /*flag if comma used on input*/
    char str[MAX_Directive_Length+1]; /*temp char array to store string*/

    while (isalpha (CUR_CHAR))  { /*keep reading while letters are used*/
        str[i++] = CUR_CHAR; /*store char and advance counter*/
        str[i]= '\0'; /*add EOF char to string */
        (*ptr)++; /*advance input pointer*/
        if (validate_directive (str)) break; /*check if string is valid directive, exit while if it is*/
        if (i>MAX_Directive_Length){print_error (ERROR_UNDEFINED_DIRECTIVE,line); return 0;} /*raise error if string length is too big*/
    }
    if (!(direct = validate_directive (str))){print_error (ERROR_UNDEFINED_DIRECTIVE,line); return 0;} /*raise error if string is undefind directive*/
    if(!isspace(CUR_CHAR)) {print_error (ERROR_EXPECTED_SPACE_DIRECTIVE,line); return 0;} /*raise error if directive is valid but no space is following*/
    line->op_type = 0; /*store operation type to data */
    line->directive_type = (direct); /*store directive type to relevant directive*/
    if (direct == 3 || direct == 4) line->num_words = 0; /*if entry/external number of code words is 0*/
    skip_space(ptr); /*skip input pointer to next non-space word*/

    switch (line->directive_type) /*read the directive base on directive type*/
    {
    case 1: /*reading data numbers*/
        do{
            long int temp_num,garbage_num; /*temp vars to store the numbers*/
            long int* temp_num_ptr; /*temp pointer to number*/
            comma_flag = 0; /*initilize flag to 0*/
            skip_space(ptr);  /*skip input pointer to next non-space word*/
            if (isdigit(CUR_CHAR) || CUR_CHAR != '+' || CUR_CHAR != '-'); { /*if valid number char, start reading number*/
                if (!read_next_num(ptr, &temp_num)){ /*call function to read the next number, and store at temp_num*/
                    if (CUR_CHAR=='\0' || CUR_CHAR=='\n') {print_error (ERROR_EXPECTED_NUM_EOL,line); return 0;} /*Raise error if EOL*/
                    else if (CUR_CHAR==',') {print_error (ERROR_EXPECTED_NUM_COMMA,line); return 0;} /*Raise error if comma before digit*/
                    else {print_error (ERROR_EXPECTED_NUM,line); return 0;} /*Raise error if invalid number*/
                }
            }
            skip_space(ptr);  /*skip input pointer to next non-space word*/
            if (read_next_num(ptr, &garbage_num)) {print_error (ERROR_MISSING_COMMA,line); return 0;} /*check if additional number appear after space without comma*/
            if (CUR_CHAR == '.') {print_error (ERROR_EXPECTED_NUM_INT,line); return 0;} /*Raise error if real number*/
            if (CUR_CHAR != '\0' && CUR_CHAR != '\n' && CUR_CHAR != ','){print_error (ERROR_EXPECTED_NUM,line); return 0;} /*Raise error wrong number format*/
            else if (CUR_CHAR == ','){ /*if comma found, raise flag and advance input pointer*/
                comma_flag = 1;
                (*ptr)++;
            }
            if (temp_num>=MAX_Data_Value || temp_num<=MIN_Data_Value) {print_error (ERROR_NUM_RANGE,line); return 0;} /*Raise error if number is too large*/
            temp_num_ptr = int_dup(&temp_num); /*store results in allocated memory pointer*/
            list_add_element (&line->data, temp_num_ptr); /*add results at data list*/
        }while (comma_flag==1); /*keep reading numbers until no comma found*/
        return 1; /*return Pass*/
 
    case 2: /*reading string */
        if (CUR_CHAR != '\"') {print_error (ERROR_EXPECTED_DOUBLE_BRACKET,line); return 0;} /*Raise error if string is not starting with " */
        (*ptr)++; /*advance input pointer */
        
        if (!read_string(ptr,&line->str)) { /*call funtion to read the string and store results at line struct */
            if (CUR_CHAR== '\"') {print_error (ERROR_EMPTY_STRING,line); return 0;} /*Raise error if string is empty*/
            else if (CUR_CHAR== '\0' || CUR_CHAR == '\n') {print_error (ERROR_STRING_EOL,line); return 0;} /*Raise error if line ended*/
            else {print_error (ERROR_STRING_INVALID,line); return 0;} /*Raise error invalid string format*/
            }

        else {
            skip_space(ptr); /*advance input pointer until non-space char */
            if (CUR_CHAR != '\0' && CUR_CHAR != '\n') {print_error (ERROR_STRING_EXPECTED_EOL,line); return 0;} /*Raise error no EOL found after string*/
            return 1; /*return Pass*/

        }
    default: /*Entry/External directive*/
        {
            int word_type; /*store word type*/
            char str[MAX_Label_Length+1]; /*char array to store label*/
            char* str_ptr = str; 
            int i=0; /*array counter */

            if (isdigit(CUR_CHAR)) {print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line); return 0;} /*Raise error if label start with digit*/
            if (!expect_non_EOL_WS(ptr)) {print_error(ERROR_LABEL_ON_EMPTY,line); return 0;} /*Raise error if no label*/
            if (CUR_CHAR =='\"') {print_error(ERROR_LABEL_NO_BRACKET,line); return 0;} /*Raise error if label encapsulated with "*/
            while (isalnum (CUR_CHAR)) { /*keep reading label while alphanumeric chars*/
                str[i++] = CUR_CHAR; /*store results and advance counter*/
                (*ptr)++; /*advance input pointer*/
                if (i>MAX_Label_Length) {print_error(ERROR_LABEL_NAME_TOO_LONG, line); return 0;} /*Raise error if label too long*/
            }
            str[i]='\0'; /*add EOF to temp string*/
            if (expect_non_EOL_WS(ptr)) {print_error(ERROR_LABEL_INVALID_FORMAT,line);return 0;} /*Raise error if input ended with no EOL/space*/
                if ((word_type = check_label(str))) { /*call function to check if label using system words and raise error if it is*/
                    if (word_type==1) {print_error(ERROR_LABEL_NAME_IDENTICAL_OP_NAME,line);return 0;} 
                    if (word_type==2) {print_error(ERROR_LABEL_NAME_IDENTICAL_DIR_MAME,line);return 0;}                        
                    if (word_type==3) {print_error(ERROR_LABEL_NAME_IDENTICAL_REG_MAME,line);return 0;}
                }
            else {
                line->str = str_dup(str); /*store results of allocated memory pointer to line struct*/
                return 1;
            }
        }
    }


}



/*Read current label and store results,
inputs - ptr-> pointer to input string, line ->pointer line struct
return 1 if label valid and 0 if failed*/ 
int read_label (char** ptr, line* line){
    char str[MAX_Label_Length+1]; /*temp char array*/
    int i = 0; /*array counter*/
    int word_type; /*store word_type*/

    if (isdigit(CUR_CHAR)) {print_error(ERROR_LABEL_CANNOT_START_WITH_NUM,line); return 0;} /*Raise error if start with number */
    while (isalnum (CUR_CHAR)) { /*keep reading while is alphnumeric*/
        str[i++] = CUR_CHAR; /*store and advance counter*/ 
        (*ptr)++; /*Advance pointer*/
        if (i>MAX_Label_Length) {print_error(ERROR_LABEL_NAME_TOO_LONG, line); return 0;} /*Raise error label too long*/
    }
    if (CUR_CHAR != ':') {print_error(ERROR_LABEL_NAME_NOT_LETTER_OR_NUM,line);return 0;} /*raise error wrong label format*/
    str[i]= '\0';
    if ((word_type = check_label(str))) { /*Check if label is identical to system word and raise error*/
        if (word_type==1) {print_error(ERROR_LABEL_NAME_IDENTICAL_OP_NAME,line);return 0;}
        if (word_type==2) {print_error(ERROR_LABEL_NAME_IDENTICAL_DIR_MAME,line);return 0;}
        if (word_type==3) {print_error(ERROR_LABEL_NAME_IDENTICAL_REG_MAME,line);return 0;}
    }
    else {
        line->label_name = str_dup(str); /*Store allocated memory pointer to label at line struct*/
        line->label_flag = 1; /*Set label flag to 1*/
        return 1;
    }

}


/*Check if operation is valid and to which group it belongs
inputs - str -> string of operation name
return 3 if not valid, 1 if one operand, 2 if two operands and 0 if no operands*/
int check_oper_group (char* str){
    int i;
    for (i=0;i<NUM_OPS;i++){ /*loop through operation array and check group*/
        if (!strcmp(str,ops[i])) { /*check if str match the current operation on array*/
            if (i>=0 && i<=4) return 2; /*ops 0-4 have 2 opertand*/
            if (i>=5 && i<=13) return 1; /*ops 5-13 have 1 opertand*/
            if (i==14 || i==15) return 0; /*ops 14-15 have no opertand*/
         }
    }
    return 3; /*if str is no operation name, return 3; */

}


/*read next operand and store results
inputs - ptr-> pointer to input string, line ->pointer line struct, target operand (src,dst) to store results.
return 0 if failed, 1 if passed */
int read_operand (char** ptr,line* line, operand* target){
    long int temp_num; /*temp num variable */
    int label_group; /*store the label group */
    char* str; /*the string to store results*/

    if (CUR_CHAR=='#'){ /*operand is immidiete, reading number - type 0*/
        (*ptr)++; /*advance input pointer */
        if (isdigit(CUR_CHAR) || CUR_CHAR != '+' || CUR_CHAR != '-'); { /*check next char is digit*/
            if (!read_next_num(ptr, &temp_num)){ /*Read the next number and store at temp_num*/
                if (CUR_CHAR=='\0' || CUR_CHAR=='\n') {print_error (ERROR_EXPECTED_NUM_EOL,line); return 0;} /*Raise error if reached EOL without number*/
                else if (CUR_CHAR==',') {print_error (ERROR_EXPECTED_NUM_COMMA,line); return 0;} /*Raise error if comma appear before number*/
                else {print_error (ERROR_EXPECTED_NUM,line); return 0;} /*Raise error wrong number format */
            }
        }
        skip_space(ptr); /*advance input pointer to next non-space character*/
        if (CUR_CHAR == '.') {print_error (ERROR_EXPECTED_NUM_INT,line); return 0;} /*raise error if number is real*/
        if (temp_num >= MAX_NUM_Value || temp_num <= MIN_NUM_Value) {print_error (ERROR_NUM_RANGE,line); return 0;} /*raise error if number out of range*/
        target->value = (int) temp_num; /*store numer at target operand*/
        target->type = 0; /*set target operand to type 0*/
        target->flag = 1; /*set target flag to type 1*/
        return 1; /*return pass*/
    }
    else if (CUR_CHAR=='&'){ /*operand is relative address type 2 */
        target->type = 2; /*set target operand to type 2*/
        target->flag = 1; /*set target flag to type 1*/
        (*ptr)++; /*advance input pointer */
    };
    if (!read_next_word(ptr, line, &str)) return 0; /*read next word, if failed return 0*/
    label_group = check_label (str); /*check label is valid*/
    if (label_group==3) { /*if operand is registery*/
        if (target->type == 2) {print_error (ERROR_LABEL_NAME_IDENTICAL_REG_MAME,line); return 0;} /*if relative address and reg, raise error*/
        target->type = 3; /*set target operand to type 3*/
        target->flag = 1; /*set target flag to type 1**/
    }
    else if (label_group==0){ /*if label is not using saved word*/
        if  (target->type != 2) {target->type = 1, target->flag = 1;} /*if not relative address, set type to 1 (direct address) and update flag*/
    } 
    else if (label_group == 2) {print_error (ERROR_LABEL_NAME_IDENTICAL_DIR_MAME,line); return 0;} /*Raise error operand identical to directive name*/
    else {print_error (ERROR_LABEL_NAME_IDENTICAL_OP_NAME,line); return 0;} /*Raise error operand identical to operation name */
    target->name = str_dup(str); /*save string to target operand (src/dst) using allocated memory pointer*/
    return 1; /*pass*/
}


/*read operands and store results
inputs - ptr-> pointer to input string, line ->pointer line struct
return 0 if failed, 1 if passed */
int read_operands (char** ptr,line* line){
    switch (line->num_oper) /*read operands based on number of expected operands*/
    {
    case 0: /*no operands are expected*/
        if (expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_INVALID_FORMAT,line); return 0;} /*Raise error expect to find EOL*/
        return 1; /*pass*/
    case 1: /*Expect one operand*/
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_NONE_EOL,line); return 0;}  /*Raise error found EOL instead of operand*/
        if (!read_operand(ptr, line, &line->op_dst)) { return 0;} /*Read operand, if failed return 0*/
        if (line->op_dst.type!=3) line->num_words++; /*if operand is not registery, update number of code words in line struct*/
        if (expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_EOL,line); return 0;} /*Raise error if line not ended*/
        return 1; /*pass*/
    case 2:
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_NONE_EOL,line); return 0;} /*Raise error found EOL instead of operand*/
        if (!read_operand(ptr, line, &line->op_src)) { return 0;} /*Read operand, if failed return 0*/
        if (line->op_src.type!=3) line->num_words++; /*if operand is not registery, update number of code words in line struct*/
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_MISSING,line); return 0;} /*Raise error found EOL instead of comma between operands*/
        if (CUR_CHAR != ',') {print_error (ERROR_OPERATION_MISSING_COMMA,line); return 0;} /*Raise error comma not found between operands*/
        (*ptr)++; /*advance input pointer*/
        if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_MISSING,line); return 0;} /*Raise error found EOL instead of operand*/
        if (!read_operand(ptr, line, &line->op_dst)) { return 0;} /*Read operand, if failed return 0*/
        if (line->op_dst.type!=3) line->num_words++; /*if operand is not registery, update number of code words in line struct*/
        if (expect_non_EOL_WS(ptr)) {print_error (ERROR_OPERATION_EXPECTED_EOL,line); return 0;}/*Raise error if line not ended*/
        return 1;

    }
}


/*read current operation and check if valid, store results on line struct
inputs - ptr-> pointer to input string, line ->pointer line struct
return 0 if failed, 1 if passed */
int read_ops (char** ptr, line* line){
    char str[MAX_OPS_Char_Length+1]; /*temp char array*/
    int i=0; /*array counter */
    int oper_group; /*store operation group */
    while (isalpha(CUR_CHAR)){ /*keep reading while reading letters*/
        str[i++]=CUR_CHAR; /*store results and update counter*/
        (*ptr)++; /*advance input pointer*/
        if (i>MAX_OPS_Char_Length) {print_error (ERROR_OPERATION_INVALID_FORMAT,line); return 0;} /*raise error if input is too long for operation*/
    }
    str[i] = '\0'; /*add EOL character*/
    
    line->num_oper = check_oper_group (str); /*check which operation group str belongs and store results at line struct*/
    if (line->num_oper==3) {print_error (ERROR_OPERATION_INVALID_FORMAT,line); return 0;} /*if operation is not valid raise error and return 0*/
    line->op_type = 1; /*update operation type to code*/
    line->str = str_dup (str); /*store string allocated memoty pointer at line struct*/
    return 1; /*pass*/
}


/*return the address type group for current operation
inputs - str-> string containing operation name
return 0 if failed, unique value for each type */
int check_oper_address_group (char* str) { 
    int i; /*counter*/
    for (i=0;i<NUM_OPS;i++){ /*check which address type current operation is related*/
        if (!strcmp(str,ops[i])) {
            if (i==0 || i==2 || i==3) return 1; /*src->0,1,3 dst->1,3*/
            if (i==1) return 2; /*src->0,1,3 dst->0,1,3*/
            if (i==4) return 3; /*src->1 dst->1,3*/
            if ((i>=5 && i<=8) || i==12) return 4; /*no src, dst->1,3*/
            if (i>=9 && i<=11) return 5; /*no src, dst->1,2*/
            if (i==13) return 6; /*no src, dst->0,1,3*/
            if (i==14 || i==15) return 7; /*no operands*/
         }
    } 
    return 0; /*failed - str doesn't match any known operation*/

}


/*read if operands address type is valid for current operation
inputs - line ->pointer line struct
return 0 if failed, 1 if passed */
int check_valid_operands (line* line){
    int src_t = line->op_src.type; /*temp var with source operand type*/
    int dst_t = line->op_dst.type;  /*temp var with destination operand type*/
    int oper_group; /* store operation group*/
    oper_group = check_oper_address_group(line->str); /*call function to check operation group*/
    switch (oper_group)
    {
    case 1: /*mov,add,sub*/
        if ((src_t==0 || src_t == 1 || src_t ==3) && (dst_t==1 || dst_t==3)) return 1; /*pass*/
        break;
    case 2: /*cmp*/
        if ((src_t==0 || src_t == 1 || src_t ==3) && (dst_t==0 || dst_t==1 || dst_t==3)) return 1;/*pass*/
        break;
    case 3: /*lea*/
        if ((src_t == 1) && (dst_t==1 || dst_t==3)) return 1;/*pass*/
        break;
    case 4: /*clr,not,inc,dec,red*/
        if (dst_t==1 || dst_t==3) return 1;/*pass*/
        break;
    case 5:/*jmp,bne*/
        if (dst_t==1 || dst_t==2) return 1;/*pass*/
        break;
    case 6:/*prn*/
        if (dst_t==0 || dst_t==1 || dst_t == 3) return 1;/*pass*/
        break;
    case 7:/*rts,stop*/
        return 1;/*pass*/
    default:
        return 0;/*failed - current operands doesn't match the operation requirements*/
    }
    return 0; /*failed - current operands doesn't match the operation requirements*/
}

/* main function to parse current line
inputs - ptr-> pointer to input string, line ->pointer line struct
return 0 if parsing failed, 1 if passed*/
int read_next_line (char** ptr, line* line){
    // int i; /*counter*/
    if (strlen(*ptr)>Max_Line_Length) {print_error (ERROR_LINE_TOO_LONG,line); return 0;} /*raise error if current line is too long*/
    skip_space(ptr); /*advance input pointer until non-space char*/
    if (CUR_CHAR== '#') return 0; /*if # ignore current line*/
    if (!expect_non_EOL_WS(ptr)) return 0; /*if line ended, ignore current line*/
    if (search_word(':',*ptr)) { /*call function to search to check if label is defined*/
        if (read_label(ptr,line)) { /*call function to read label and store results at line struct*/
            (*ptr)++; /*advance input pointer*/
            if (!expect_non_EOL_WS(ptr)) {print_error (ERROR_LABEL_EMPTY,line); return 0;} /*raise error if line ended after label declaration*/
            }
    }
    if (CUR_CHAR=='.'){ /*check if char is . expect directive*/
        (*ptr)++; /*advance input pointer*/ 
        if (!read_directive (ptr,line)) return 0; /*read directive and store results at line struct, return 0 if failed*/
        return 1; /*pass*/
    }
    if (!read_ops (ptr,line)) {return 0;} /*expect code command, read operation, store results at line struct, return 0 if failed*/
    if (!read_operands (ptr,line)) {return 0;} /* read operands*/
    if (!check_valid_operands(line)) {print_error (ERROR_OPERATION_INVALID_ADDRESS,line); return 0;} /*check operand are valid, raise error if not*/
    line->op_type = 1; /*update operation type to code*/
    return 1; /*pass*/
}


/* Initilize current line parsing struct
inputs - line ->pointer line struct */
void initilize_line (line* line){ /*set struct main values to 0*/
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

/* helper function for debugging to print the struct line 
inputs - line ->pointer line struct */
void print_line (line* line){
    
    if (line->label_flag==1)  ("Label Flag is: %d, name is %s\n",line->label_flag, line->label_name); /*print label if exist*/

    if (line->op_type==1) { /*print operation and operands if operation is code*/
        printf ("Oper name: %s\n",line->str);
        printf ("Num ops: %d\n",line->num_oper);
        printf ("Src type: %d\n",line->op_src.type);        
        printf ("Dest type: %d\n",line->op_dst.type);
        printf ("Src value: %d\n",line->op_src.value);        
        printf ("Dest value: %d\n",line->op_dst.value);
        printf ("Src name: %s\n",line->op_src.name);        
        printf ("Dest name: %s\n",line->op_dst.name);

    }
    if (line->op_type==0){ /*print directive name and values if operation is data*/
        printf ("Directive_type: %d\n",line->directive_type);
        if (line->directive_type==1) printf ("First Num is : %ld\n",*(long int*) line->data->value);
        else printf ("string/directive lable is : %s\n",line->str);
    }

}


/* Check if current label already exist on symbol table
inputs - str -> string with current label, symbol_table -> current symbol tabel 
return 0 if found, return 1 if passed without duplication */
int check_dup_label (char* str, symbol* symbol_table) {
    symbol* ptr = symbol_table; /*set pointer*/
    if (ptr==NULL) return 1; /*return 1 if table still empty*/
    while (ptr->next != NULL) { /*keep check all nodes of symbol table and check if match to current label*/
        if (!strcmp(ptr->name,str)) return 0; /*if label match to table node, return 0 (failed) */
        ptr = ptr->next; /*advance tabel pointer*/
    }
    if (!strcmp(ptr->name,str)) return 0; /*if label match to table node, return 0 (failed) */
    return 1; /*pass, current label doesn't exist on symbol table*/
}

/* Adds an element to a linked list pointer, and if it doesn't exist (ie. NULL), creates it and assigned to pointer 
inputs - symbol_table -> pointer to symbol table, line -> current line struct, DC - data counter, 
          target ->operand to update, type -> new symbol name*/
void update_symbol (symbol** symbol_table, line* line,int DC, char* target, int type) {
	
    if (*symbol_table == NULL) { /*if symbol table note exist yet, create first node*/
		*symbol_table = calloc(1, sizeof(symbol));

		if (*symbol_table == NULL) { /* Check if calloc failed and exit*/
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		/* update new symbol node with relevant values */
		(*symbol_table)->value = DC; /*update counter*/
        (*symbol_table)->type = type; /*update type*/
        (*symbol_table)->name = str_dup (target); /*copy name to allocated memory pointer in symbol table*/
        (*symbol_table)->next = NULL; /*set next node to null*/

	} else { /*table already exist*/
		symbol* ptr = *symbol_table; /*create ptr as pointer to symbol table*/

		while (ptr->next != NULL) { /* Skip to last element */
			ptr = ptr->next;
		}
		ptr->next = calloc(1, sizeof(list)); /*create new node after last node*/
		if (ptr->next == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		/* Put new values in list node */
		ptr->next->value = DC; /*update counter*/
        ptr->next->type = type; /*update type*/
        ptr->next->name = str_dup (target); /*copy name to allocated memory pointer in symbol table*/
        ptr->next->next = NULL; /*set next node to null*/
	}
}

/*Add current line data numbers to data image table
inputs - cur_line_data -> pointer list node containing data from current line, data_img -> pointer to data image table pointer, DC - data counter, 
return the new DC counter*/
int add_data_image (list* cur_line_data, data_image** data_img, int DC){
    int end_flag = 0; /*flag as end of table*/
    if (*data_img == NULL) { /*data image table is not exist, create new*/
		*data_img = calloc(1, sizeof(data_image));

		
		if (*data_img == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
        
        /*update the new node with first data node*/
        (*data_img)->counter = DC++; /*store counter and advance*/
        (*data_img)->type = 1; /*set type to 1 - data*/
        (*data_img)->value = int_dup( cur_line_data->value); /*store results as allocated memory pointer*/
        (*data_img)->next = NULL; /*set next node to null*/
        if (cur_line_data->next != NULL)  cur_line_data = cur_line_data->next; /*advance to next data node of current line if available*/
        else end_flag = 1; /*no further data node exist*/
    }
    /* data image table already exist*/
    data_image* ptr = *data_img; /*set ptr to data image table*/
    

    while (ptr->next != NULL) { /*get to last node of data_image*/
        ptr = ptr->next;
    }
    while (end_flag == 0) { /*while not reach last data node of current line*/
            ptr->next = calloc(1, sizeof(symbol)); /*create new node at data_image table*/
            if (ptr->next == NULL) { /* Check if calloc failed and exit*/
                fprintf(stderr, ERROR_OUT_OF_MEMORY);
                exit(1);
            }
            ptr->next->counter = DC++; /*store and update counter*/
            ptr->next->type = 1; /*set type to 1 - data*/
            ptr->next->value = int_dup( cur_line_data->value); /*store results as allocated memory pointer*/
            ptr = ptr->next; /*advance data image table pointer*/
            if (cur_line_data->next != NULL)  cur_line_data = cur_line_data->next; /*advance to next data node of current line if available*/
            else break; /*no further data node exist*/
    }
return DC; /*return the new DC counter*/
}
    

/*Add current line string to data image table
inputs - cur_line_data -> string  from current line, data_img -> pointer to data image table pointer, DC - data counter, 
return the new DC counter*/
int add_string_image (char* cur_line_string, data_image** data_img, int DC){
    int end_flag = 0; /*flag as end of table*/
    if (*data_img == NULL) { /*data image table is not exist, create new*/
		*data_img = calloc(1, sizeof(data_image));

		if (*data_img == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}        
        /*update the new node with first data node*/
        (*data_img)->counter = DC++; /*store and update counter*/
        (*data_img)->type = 2; /*set type to 2 - string*/
        (*data_img)->value = charecter_dup(*cur_line_string); /*copy charecter pointer to data image table*/
        cur_line_string++;  /*advance string pointer*/

    }
    /* data image table already exist*/
    data_image* ptr_img = *data_img; /*set ptr to data image table*/
    while (ptr_img->next != NULL) { /*get to last node of data_image*/
        ptr_img = ptr_img->next;
    }
    while (*cur_line_string != '\0'){  /*keep store data until end of string*/
            ptr_img->next = calloc(1, sizeof(data_image));  /*create new node at data_image table*/
            if (ptr_img->next == NULL) { /* Check if calloc failed and exit*/
                fprintf(stderr, ERROR_OUT_OF_MEMORY);
                exit(1);
            }
            ptr_img->next->counter = DC++; /*store and update counter*/
            ptr_img->next->type = 2; /*set type to 2 - string*/
            ptr_img->next->value = charecter_dup(*cur_line_string); /*copy charecter pointer to data image table*/
            ptr_img = ptr_img->next; /*advance to next data image node*/
            cur_line_string++; /*advance string pointer*/
    }
    /*add last charected /0 node at data image table*/
    ptr_img->next = calloc(1, sizeof(data_image)); /*create new nore*/
    if (ptr_img->next == NULL) { /* Check if calloc failed and exit */
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(1);
    }
    ptr_img->next->counter = DC++;  /*store and update counter*/
    ptr_img->next->type = 2; /*set type to 2 - string*/
    ptr_img->next->value = charecter_dup('\0'); /*add pointer to \0 character*/
    ptr_img->next->next = NULL; /*set next node to NULL*/
    return DC; /*return the updated DC*/
    
}




/* Function to free the symbol table memory
inputs - ptr -> pointer to symbol table*/
void freelist_symbol (symbol* *ptr){
    symbol* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr; 
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}


/* Function to free the data table memory
inputs - ptr -> pointer to data table*/
void freelist_data (data_image* *ptr){
    data_image* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr;
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}

/* Function to free the code table memory
inputs - ptr -> pointer to code table*/
void freelist_code (generated_code* *ptr){
    generated_code* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr;
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}

/* Function to free the external table memory
inputs - ptr -> pointer to external table*/
void freelist_external (external* *ptr){
    external* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr;
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}


/* Find the funct and op_code of current command 
inputs - str -> current operation, funct -> pointer to funct int, op_code ->pointer to op_code int*/
void find_funct_opcode (char* str, int* funct, int* op_code) {
    int i; /*counter*/
    for (i=0;i<NUM_OPS;i++){ /*loop through operations array*/
        if (!strcmp(str,ops[i])) { /*check if str match array item, update funct and op_code based on operation found*/
            if (i==0) {*funct = 0; *op_code = 0; return;}  /*mov*/
            else if (i==1) {*funct = 0; *op_code = 1; return;} /*cmp*/
            else if (i==2) {*funct = 1; *op_code = 2; return;} /*add*/
            else if (i==3) {*funct = 2; *op_code = 2; return;} /*sub*/
            else if (i==4) {*funct = 0; *op_code = 4; return;} /*lea*/
            else if (i==5) {*funct = 1; *op_code = 5; return;} /*clr*/
            else if (i==6) {*funct = 2; *op_code = 5; return;} /*not*/
            else if (i==7) {*funct = 3; *op_code = 5; return;} /*inc*/
            else if (i==8) {*funct = 4; *op_code = 5; return;} /*dec*/
            else if (i==9) {*funct = 1; *op_code = 9; return;} /*jmp*/
            else if (i==10) {*funct = 2; *op_code = 9; return;} /*bne*/
            else if (i==11) {*funct = 3; *op_code = 9; return;} /*jsr*/
            else if (i==12) {*funct = 0; *op_code = 12; return;} /*red*/
            else if (i==13) {*funct = 0; *op_code = 13; return;} /*prn*/
            else if (i==14) {*funct = 0; *op_code = 14; return;} /*rts*/
            else  {*funct = 0; *op_code = 15; return;} /*stop*/
         }
    }
}


/* Write the first word code to the code table struct 
inputs - line -> pointer to line struct*/
int generate_first_word_code (line* line){
    unsigned int ARE = 1<<2; /*A bit is always 1 on first word*/
    unsigned int funct; /*bits 3-7*/
    unsigned int reg_src = 0; /*bits 13-15*/
    unsigned int address_src =0; /*bits 16-17*/
    unsigned int reg_dst = 0 ; /*bits 18-10*/
    unsigned int address_dst = 0; /*bits 11-12*/
    unsigned int opcode; /*bits 18-23*/
    unsigned int word; /*final word*/

    find_funct_opcode (line->str, &funct, &opcode);  /*call function to get funct and op_code values*/
    if (line->op_src.type == 3) reg_src = (int) (line->op_src.name[1]) - 48; /*store source registery number*/
    if (line->op_dst.type == 3) reg_dst = (int) (line->op_dst.name[1]) - 48; /*store destination registery number*/
    address_src = line->op_src.type; /*store source address*/
    address_dst = line->op_dst.type; /*store destination  address*/
     /*bitwise shift values according to definitions*/
    reg_src <<= REG_SRC_FIRST_BIT;
    reg_dst <<= REG_DST_FIRST_BIT;
    funct <<= FUNCT_FIRST_BIT;
    address_src <<= ADDRESS_SRC_FIRST_BIT;
    address_dst <<= ADDRESS_DST_FIRST_BIT;
    opcode <<=OP_CODE_FIRST_BIT;
    word = ARE | funct | reg_dst | address_dst | reg_src | address_src | opcode;  /*combine all variables with bitwise or*/
    return word; /*return calculated number*/
}

/* Helper Function to print to screen current word
inputs - word -> current word*/
void print_word (int word){
    int j=23;
    int i;

    for (i =1 << 23; i ; i>>=1, j--){ /*loop through bits and print value*/
        printf ("%c", word&i? '1' : '0');
        /* if (j==3 || j== 8 || j==11 || j== 13 || j==16 || j==18) printf ("|");  option  to add delimiters per fields*/
    }
    printf ("\n");
}


/* write the generated code in the next available code node , advance IC by 1 
inputs - first_word -> word to write to code table, code_ptr -> pointer to code table, IC - pointer to current IC counter*/
void write_first_word_code (unsigned int first_word,generated_code** code_ptr,int* IC){
    if (*code_ptr == NULL) { /*if code table not exixt, create one*/
		*code_ptr = calloc(1, sizeof(symbol));

		
		if (*code_ptr == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		/* update values at first node */
		(*code_ptr)->word = first_word; /*store word*/
        (*code_ptr)->counter = *IC; /*store IC counter*/
        (*code_ptr)->next = NULL; /*set next node to NULL*/
        (*IC)++; /*updat counter*/

	} else { /*Code table already defined*/
        generated_code* ptr = *code_ptr; /*set ptr to code table*/

		/* Skip to last element */
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = calloc(1, sizeof(list)); /*create new node*/
		if (ptr->next == NULL) { /* Check if calloc failed and exit*/
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		ptr->next->word = first_word; /*store word*/
        ptr->next->counter = *IC; /*store IC counter*/
        ptr->next->next = NULL;/*set next node to NULL*/
        (*IC)++; /*updat counter*/
    }
}


/* write the immidite number code in the next available code node, advance IC by 1 
inputs - first_word -> word to write to code table, code_ptr -> pointer to code table, IC - pointer to current IC counter*/
void write_code_number (unsigned int number,generated_code** code_ptr,int* IC){

    int ARE = 1<<2; /*A bit is on for immidiate address*/
    int calculated_num = (number << 3 | ARE); /*shift stored number by 3 and combine with ARE*/
    generated_code* ptr = *code_ptr; /*set ptr to code table*/

    /* Skip to last element */
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = calloc(1, sizeof(list)); /*create new node*/
    if (ptr->next == NULL) { /* Check if calloc failed  and exit */
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(1);
    }
    ptr->next->word = calculated_num;  /*store calculated number and ARE as word*/
    ptr->next->counter = *IC; /*store IC counter*/
    ptr->next->next = NULL; /*set next node to NULL*/
    (*IC)++; /*updat counter*/
   
}

/* add empty code line for second pass, advance IC by 1 
inputs - code_ptr -> pointer to code table, IC - pointer to current IC counter*/
void write_code_skip (generated_code** code_ptr,int* IC){

    generated_code* ptr = *code_ptr; /*set ptr to code table*/
    /* Skip to last element */
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = calloc(1, sizeof(list)); /*create new node*/
    if (ptr->next == NULL) {  /* Check if calloc failed and exit*/
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(1);
    }
    ptr->next->counter = *IC; /*store IC counter*/
    ptr->next->next = NULL; /*set next node to NULL*/
    (*IC)++; /*updat counter*/
}


/* Debug helper function to print generated tables 
inputs - symbol_table_ptr -> pointer to symbol table, data_ptr -> pointer to data image table, 
        code_ptr -> pointer to code table, external_par -> pointer to external table*/
void print_tables (symbol* symbol_table_ptr, data_image* data_ptr, generated_code* code_ptr, external* external_ptr){
    
    while (symbol_table_ptr!=NULL) { /*loop through symbol table and pring values*/
        printf ("\n Symbols: val : %d, name : %s, type : %d, type_e : %d", symbol_table_ptr->value, symbol_table_ptr->name, symbol_table_ptr->type, symbol_table_ptr->type_e);
        if (symbol_table_ptr->next !=NULL) symbol_table_ptr= symbol_table_ptr ->next;
        else break;
    }
    while (data_ptr!=NULL) { /*loop through data image table and pring values*/
        printf ("\n Data: type : %d, counter : %d", data_ptr->type, data_ptr->counter);
        if (data_ptr->type==1) printf (" values is : %ld", *(long int*) data_ptr->value);
        else if (data_ptr->type==2) printf (" values is : %c ", *(char*) data_ptr->value);
        if (data_ptr->next !=NULL) data_ptr = data_ptr->next;
        else break;
    }

    while (code_ptr!=NULL) { /*loop through code table and pring values*/
        printf ("\n Code: counter : %d, code :  " , code_ptr->counter);
        print_word (code_ptr->word);
        if (code_ptr->next !=NULL) code_ptr = code_ptr->next;
        else break;
    }

    while (external_ptr!=NULL) {/*loop through external table and pring values*/
        printf ("\n External: counter : %d, code : %s " , external_ptr->value, external_ptr->name);
        if (external_ptr->next !=NULL) external_ptr = external_ptr->next;
        else break;
    }
    printf ("\n");

}


/* update symbol data table to add ICF to every data label value 
inputs - symbol_table -> pointer to symbol table pointer, ICF - final code counter*/
void update_symbol_first_pass (symbol** symbol_table, int ICF) {
	
		symbol* ptr = *symbol_table; /*set pointer*/
		while (ptr->next != NULL) { /*keep looping while not last node*/
            if (ptr->type == 1 || ptr->type == 2) ptr->value += ICF; /*if symbol type is data/string then update counter by ICF*/
			ptr = ptr->next;
		}
        if (ptr->type == 1) ptr->value += ICF; /*update last node - if symbol type is data/string then update counter by ICF*/
	
}

/* update symbol data entry flag 
inputs - symbol_table -> pointer to symbol table pointer, str - current entry label, line - current input line, file - current filename
return 0 if no symbol found, 1 if pass */
int update_symbol_entry (symbol** symbol_table, char* str,int line, char* file){
    symbol* ptr = *symbol_table; /*set pointer*/
    int found_flag = 0; /*flag if symbol found*/
    while (ptr->next != NULL) { /*keep looping while not last node*/
            if (!strcmp(str,ptr->name)){ /*keep search symbol table, if match to current, set entry flag to 1*/
                ptr->type_e = 1;
                found_flag = 1;
            }
			ptr = ptr->next;
		}
    if (found_flag==0) {fprintf (stderr, ERROR_ENTRY_LABEL_NOT_FOUND, line, file ); return 0;} /*raise error if entry label note found on symbol table*/
    return 1; /*pass */
}


/* find current operand label in symbol table  
inputs - symbol_table -> pointer to symbol table pointer, str - current entry label.
return 0 if no symbol found, -1 if label is external and adress counter if found */
int find_symbol_address (symbol** symbol_table, char* str){
  
    symbol* ptr = *symbol_table; /*set pointer*/
    while (ptr->next != NULL) { /*keep looping while not last node*/
        if (!strcmp(str,ptr->name)) { /*check if str match current node*/
            if (ptr->type != 4) return (ptr->value); /*if match to current and not external return adress*/
            else return -1; /*return -1 if external */
        }
        ptr = ptr->next; 
	}
    if (!strcmp(str,ptr->name)) { /*check also last node*/
            if (ptr->type != 4) return (ptr->value); /*if match to current and not external return adress*/
            else return -1; /*return -1 if external */
        }
    return 0; /*no symbol found */
}


/* Create and update external table 
inputs - external_table -> pointer to external table pointer, IC -> code counter, str ->current label.*/
void update_external_table (external** external_ptr, int IC, char* str) {
    if (*external_ptr == NULL) { /*if table not exist, create new*/
		*external_ptr = calloc(1, sizeof(external));

		
		if (*external_ptr == NULL) { /* Check if calloc failed  and exit */ 
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		(*external_ptr)->name = str_dup (str); /*store current label at new node*/
        (*external_ptr)->value = IC; /*store current counter at new node*/

	} else { /*External table exist*/
        external* ptr = *external_ptr; /*set ptr to external table*/
		while (ptr->next != NULL) { /* Skip to last element */
			ptr = ptr->next;
		}
		ptr->next = calloc(1, sizeof(list)); /*create new node*/
		if (ptr->next == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		ptr->next->name = str_dup (str); /*store current label at new node*/
        ptr->next->value = IC; /*store current counter at new node*/
        ptr->next->next = NULL; /*set next node ot NULL */
    }

}


/* write the second pass, add missing labels code in the next relevant code list , advance IC by 1 
inputs - symbol_address - adress of current symbol, IC -> code counter, 
          address+type -> type of address, code_ptr-> pointer to code table pointer */
void write_second_word (int symbol_address,int IC, int address_type, generated_code** code_ptr){
    
    int ARE; /*define ARE bits*/
    int calculated_num; /*temp number for calculation*/
    if (symbol_address == -1) calculated_num = 1; /*if external, calculated number is 1*/
    else {
        ARE = 1<<address_type; /* shift ARE per address type, if direct (1) R bit is on, if relative (2) L bit is on*/
        calculated_num =  (symbol_address<<3 | ARE); /*calculate the required word by combine the ARE and the shifted number to bits 3-24*/
    }
    
    generated_code* ptr = *code_ptr; /*set pointer*/

    while (ptr->counter != IC) { /*skip to last node*/
        ptr = ptr->next;
    }

    ptr->word = calculated_num; /*store the calculated word*/

}


/* write the generated code to the ob file 
inputs - code_ptr -> pointer to code table, data_ptr -> pointer to data image table, 
        ICF - final code counter, DCF - final data counter, file_ob - file name to save results. */
void print_code (generated_code* code_ptr, data_image* data_ptr, int ICF, int DCF, char* file_ob){
    FILE *current_file = NULL; /*create file pointer */
    char str[10]; /*set temp char array*/
    char str_cpy[6]; /*set temp char array at final size*/
    int num; /*temp num */
    int str_len; /*store the converted to hex str length*/

    if (!(current_file = fopen (file_ob,"wt"))){ /*open at write mode the ob file*/
        fprintf (stderr, ERROR_WRITE_FILE);
        exit (0); 
    }
    fprintf (current_file,"\t%d %d", ICF-100, DCF); /*print header to file */
    while (code_ptr!=NULL) { /*Printing code section*/
        num = (unsigned int) code_ptr->word; /*cast word to number */
        sprintf(str, "%06x", num); /*convert word to hex as string */
        str_len = strlen(str); /*get string length*/
        if (str_len>6) strcpy (str,str+(str_len-6) ); /*trim the string back to 6 digits*/
        fprintf (current_file,"\n%07d ", (code_ptr->counter)); /*print to file current counter*/
        fprintf (current_file,"%s", str); /*print to file converted hex string*/
        if (code_ptr->next !=NULL) code_ptr = code_ptr->next; /*skip to next node*/
        else break;    
    }
    while (data_ptr!=NULL) { /*Printing data section*/
        num = *(unsigned int*) data_ptr->value; /*cast word to number */
        sprintf(str, "%06x", num); /*convert word to hex as string */
        str_len = strlen(str); /*get string length*/
        if (str_len>6) strcpy (str,str+(str_len-6) ); /*trim the string back to 6 digits*/
        fprintf (current_file,"\n%07d ", (data_ptr->counter + ICF)); /*print to file current counter + ICF*/
        fprintf (current_file,"%s", str); /*print to file converted hex string*/
        if (data_ptr->next !=NULL) data_ptr = data_ptr->next; /*skip to next node*/
        else break;
    }
    fclose(current_file); /*close file*/
    return;
}

/* write the external table to the ext file 
inputs - external_ptr -> pointer to external table, file_ext -> string of ext file name */
void print_external (external* external_ptr, char* file_ext){
    FILE *current_file = NULL; /*create file pointer */
    if (!(current_file = fopen (file_ext,"wt"))) {  /*open at write mode the ob file*/
        fprintf (stderr, ERROR_WRITE_FILE);
        exit (0); 
    }
    while (external_ptr!=NULL) { /* loop until last node */
        fprintf (current_file,"%s %07d\n" ,external_ptr->name, external_ptr->value ); /*print to file external label name and code addres*/
        if (external_ptr->next !=NULL) external_ptr = external_ptr->next; /*move to next node*/
        else break;
    }

    fclose(current_file);
    return;
}

/* write the entry table to the ent file 
inputs - symbol_ptr -> pointer to symbol table, file_ent -> string of ent file name */
void print_entry (symbol* symbol_ptr, char* file_ent){
    
    FILE *current_file = NULL;  /*create file pointer */

    if (!(current_file = fopen (file_ent,"wt"))){ /*open at write mode the ob file*/
        fprintf (stderr, ERROR_WRITE_FILE);
        exit (0); 
    }

    while (symbol_ptr!=NULL) { /* loop until last node of symbol table */
        if (symbol_ptr->type_e ==1 ){ /*check if current symbol has entry flag on */
            fprintf (current_file,"%s %07d\n" ,symbol_ptr->name, symbol_ptr->value ); /*print to file entry label name and addres*/
        }
        
        if (symbol_ptr->next !=NULL) symbol_ptr = symbol_ptr->next; /*move to next node*/
        else break;
    }

    fclose(current_file); /*close file */
    return;
}


/*The function open the next input file and save results to file pointer
inputs - argv -> pointer to argv array of input file names, files -> pointer to struct with current file names
         file_counter -> counter of number of files, file -> pointer to new file
return 0 if failed, 1 if passed*/
int open_ps_name (char** argv,filenames* files, int file_counter, FILE** file){
    
    char str_temp[MAX_FILENAME_LENGTH]; /*create temp file name string*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ps,strcat(str_temp, ".ps")); /*concatenate .ps extention and store result at files struct*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ent,strcat(str_temp, ".ent")); /*concatenate .ent extention and store result at files struct*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ext,strcat(str_temp, ".ext")); /*concatenate .ext extention and store result at files struct*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ob,strcat(str_temp, ".ob")); /*concatenate .ob extention and store result at files struct*/
        
    if (!(*file = fopen (files->name_ps,"r"))){ /*open ps file in read mode before parsing*/
        fprintf (stderr, ERROR_OPEN_FILE); /*send error and exit if failed open file*/
        return (0); 
    }
    return 1; /*pass*/
}


int main (int argc, char *argv[]){
    
    FILE *file_ps; /*pointer to current ps file*/
    char str[Max_Line_Length +1 ] ; /*string to store current line*/
    char *ptr = str; /*create pointer to current line string*/
    line cur_line; /*create current line parsing struct*/
    filenames files; /*create filenames struct */
    int file_counter = 1; /*count number of files*/
    unsigned int first_word; /*store current line first code word after parsing */
    int i,j; /*counters*/
    int ICF; /*ICF define final code counter after first pass*/
    int DCF; /*DCF define final data counter after first pass*/
    
    
    if (argc==1) {fprintf (stderr, ERROR_MISSING_FILE); return (1);}; /*raise error and exit if no ps file passed*/

    while (file_counter<=(argc-1)) { /*keep while there is more ps files to compile*/
        int DC = 0; /*initilize data counter*/
        int IC = CODE_START; /*initilize code counter*/
        int error_flag = 0; /*set error flag to 0 */

        if (!open_ps_name (argv, &files, file_counter, &file_ps)) {return 0;}; /*call function to open next file and save to file_ps pointer, exit if failed*/
        file_counter++; /*increase file counter by 1*/
        generated_code* code_ptr = NULL; /*initilize code table pointer*/
        data_image* data_ptr = NULL; /*initilize data image table pointer*/
        symbol* symbol_table_ptr =NULL; /*initilize symbol table pointer*/
        external* external_table_ptr = NULL; /*initilize external table pointer*/
        cur_line.filename = files.name_ps; /*set and store file name*/
        cur_line.line_num = -1; /*set line counter to -1*/
        while (ptr = fgets (str,Max_Line_Length+1,file_ps)) { /*keep parsing and first pass until EOF*/
            initilize_line (&cur_line);  /*call funtion to initilize current line struct*/
            cur_line.line_num++; /*increase line counter*/
            if (read_next_line (&ptr, &cur_line)) { 
                /*call main function to parse current line, store parsing at line struct and if succeed start first pass*/
                if (cur_line.directive_type==1 || cur_line.directive_type==2) { /*if directive is data or string*/
                    if (cur_line.label_flag==1) { /*if label flag is on*/
                    /* check if label already exist on symbol table*/
                        if (!check_dup_label(cur_line.label_name, symbol_table_ptr)) {print_error (ERROR_LABEL_EXIST,&cur_line); continue;}
                        update_symbol(&symbol_table_ptr,&cur_line,DC, cur_line.label_name,cur_line.directive_type); /*call function to update symbol table*/
                    }
                    if (cur_line.directive_type==1){ /*update data image with current line data */
                        DC = add_data_image (cur_line.data, &data_ptr, DC);
                    }
                    if (cur_line.directive_type==2){ /*update data image with current line string */
                        DC = add_string_image (cur_line.str, &data_ptr, DC);
                    }
                }
                else if (cur_line.directive_type==3) continue; /*if entry move to next line*/
                /*if external update symbol table*/
                else if (cur_line.directive_type==4) update_symbol(&symbol_table_ptr,&cur_line,FIRST_PASS_EXTERN_VAL, cur_line.str,cur_line.directive_type);
                else { /*****Code command *****/
                    if (cur_line.label_flag==1) { /*if label flag is on*/
                        /* check if label already exist on symbol table*/
                        if (!check_dup_label(cur_line.label_name, symbol_table_ptr)) {print_error (ERROR_LABEL_EXIST,&cur_line); continue;}
                        update_symbol(&symbol_table_ptr,&cur_line,IC, cur_line.label_name,SYMBOL_DIRECTIVE_CODE); /*call function to update symbol table*/
                    }
                    first_word =  generate_first_word_code (&cur_line); /*call function to generate first word and store results*/
                    /* print_word (first_word); option to run debugging to print the generated word */
                    write_first_word_code (first_word,&code_ptr, &IC); /*call function to write first word to code table*/
                    if (cur_line.num_words==1); /*if current line require 1 line of code */
                    else if (cur_line.op_src.flag==0){ /*if src operand flag is off */
                    /*if dst operand type is direct(1) relative (2) call function to create empty line at code table*/
                        if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) write_code_skip(&code_ptr, &IC); 
                      /*if dst operand type is immidiate(0) call function to write number to code table*/
                        if (cur_line.op_dst.type==0) write_code_number(cur_line.op_dst.value, &code_ptr, &IC);
                    }
                    else if (cur_line.op_src.flag==1){/*if src flag is on, need to read both operands*/
                    /*if src operand type is direct(1) relative (2) call function to create empty line at code table*/
                        if (cur_line.op_src.type==1 || cur_line.op_src.type==2) write_code_skip(&code_ptr, &IC);
                        /*if src operand type is immidiate(0) call function to write number to code table*/
                        if (cur_line.op_src.type==0) write_code_number(cur_line.op_src.value, &code_ptr, &IC);
                        /*if dst operand type is direct(1) relative (2) call function to create empty line at code table*/
                        if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) write_code_skip(&code_ptr, &IC);
                        /*if dst operand type is immidiate(0) call function to write number to code table*/
                        if (cur_line.op_dst.type==0) write_code_number(cur_line.op_dst.value, &code_ptr, &IC);
                    }
                    

                }

            }
            else { /*if parsing failed, raise the error_flag and print current line */
                if (cur_line.error.failed == 1) error_flag = 1; 
                printf ("Error on line: %s", str);
            }
            
        } /*finished parsing and first pass  */

        if (error_flag==1) return 1; /*stop program if error found during parsing*/
        ICF = IC; /*store final code counter*/
        DCF = DC; /*store final data counter*/
        update_symbol_first_pass (&symbol_table_ptr, ICF); /*update data items in symbol table with new counter*/
        
        /* Help debugging function to print tables /*
        /* print_tables(symbol_table_ptr,data_ptr, code_ptr,external_table_ptr); */

        /******Second Pass ************/

        rewind (file_ps); /*start second pass from start of file */
        IC = CODE_START; /*intilize code counter back to start*/

        while (ptr = fgets (str,Max_Line_Length+1,file_ps)) { /*start parsing and second pass until EOF*/
            initilize_line (&cur_line); /*initilize current line struct*/
            read_next_line (&ptr, &cur_line); /*call funtion to parse current line and store results at line struct*/
            /* if directive is data, string or external move to next line*/
            if (cur_line.directive_type==1 || cur_line.directive_type==2 || cur_line.directive_type == 4) continue;
            /* if directive is entry - update symbol entry flag to on, raise error if label is not found*/
            else if (cur_line.directive_type==3) {
                if (!update_symbol_entry(&symbol_table_ptr, cur_line.str,cur_line.line_num, cur_line.filename));
            }
            else { /*Code only */
                if (cur_line.num_words == 1) {IC++; continue;} /*if one word is required, increase counter and move to next line*/
                if (cur_line.op_src.flag==0 || cur_line.op_src.type==3 ){  /*if no source operand or if src operand is registery (3)*/
                    if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) { /*if destination operand is direct(1) or relative (2) */
                        int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_dst.name); /*find operand in symbol table and store address*/
                        if (!symbol_address) { /*raise error if label operand not found in symbol table*/
                            fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename);
                            break;
                        }
                        /* if adress is external (-1) update external table with next line counter and destination operand name*/
                        else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+1, cur_line.op_dst.name);};
                        /*if destination operand is relative, calculate the required code step*/
                        if (cur_line.op_dst.type==2) symbol_address = symbol_address - IC;
                        /*write second word to code table with updated adress, counter and operand type*/
                        write_second_word(symbol_address,IC+1,cur_line.op_dst.type, &code_ptr);
                        
                    }
                }
                else { /*Two additional words required*/
                    if (cur_line.op_src.type==1 || cur_line.op_src.type==2) {/*if source operand is direct(1) or relative (2) */
                        int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_src.name); /*find operand in symbol table and store address*/
                        if (!symbol_address) { /*raise error if label operand not found in symbol table*/
                            fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename); 
                            break;}
                        /* if adress is external (-1) update external table with next line counter and source operand name*/
                        else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+1, cur_line.op_src.name);};
                        /*if source operand is relative, calculate the required code step*/
                        if (cur_line.op_src.type==2) symbol_address = symbol_address - IC;
                        /*write second word to code table with updated adress, counter and operand type*/
                        write_second_word(symbol_address,IC+1,cur_line.op_src.type, &code_ptr);
                        
                    } /*if destination operand is direct(1) or relative (2) */
                    if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) {
                        int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_dst.name); /*find operand in symbol table and store address*/
                        if (!symbol_address) { /*raise error if label operand not found in symbol table*/
                            fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename); 
                            break;}
                         /* if adress is external (-1) update external table with next line counter and source operand name*/
                        else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+2, cur_line.op_dst.name);};
                        /*if destination operand is relative, calculate the required code step*/
                        if (cur_line.op_dst.type==2) symbol_address = symbol_address - IC;
                         /*write second word to code table with updated adress, counter and operand type*/
                        write_second_word(symbol_address,IC+2,cur_line.op_dst.type, &code_ptr);
                    }
                }
                
            }
            IC += cur_line.num_words; /*update code counter with number of code words in current line*/

        
        } /*finish second pass*/

        /*print_tables(symbol_table_ptr,data_ptr, code_ptr, external_table_ptr); Help debugging function to print tables*/
        print_code (code_ptr, data_ptr, ICF,DCF,files.name_ob); /*call funcation to create ob and write code and data table*/
        print_external (external_table_ptr,files.name_ext); /*call function to create ext file and write external table*/
        print_entry (symbol_table_ptr, files.name_ent); /*call function to create entry file and write entry table*/
        
        freelist_symbol (&symbol_table_ptr); /*free symbol table*/
        freelist_data (&data_ptr); /*free data image table*/
        freelist_code (&code_ptr); /*free code table*/
        freelist_external (&external_table_ptr); /*free external table*/
    } /*finished compile all files*/
    return 0;
}

