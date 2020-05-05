
#include <stdio.h>
#include <string.h>
#include "parsing.h"




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
            if (isdigit(CUR_CHAR) || CUR_CHAR != '+' || CUR_CHAR != '-') { /*if valid number char, start reading number*/
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

return 1;
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
    return 1;
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
        if (isdigit(CUR_CHAR) || CUR_CHAR != '+' || CUR_CHAR != '-') { /*check next char is digit*/
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
    return 1;
}


/*read current operation and check if valid, store results on line struct
inputs - ptr-> pointer to input string, line ->pointer line struct
return 0 if failed, 1 if passed */
int read_ops (char** ptr, line* line){
    char str[MAX_OPS_Char_Length+1]; /*temp char array*/
    int i=0; /*array counter */
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

