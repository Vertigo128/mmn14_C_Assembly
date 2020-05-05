#ifndef MMN14_PARSING_H
#define MMN14_PARSING_H

#include "consts.h"
#include "data.h"
#include "errors.h"
#include "helpers.h"
#include "printing.h"


int validate_directive (char* str);
int read_next_num (char** ptr, long int* temp_num);
int read_string (char** ptr, char** line_str);
int read_next_word (char** ptr,line* line, char** str_temp);
int check_label (char* str);
int read_directive (char** ptr, line* line);
int read_label (char** ptr, line* line);
int check_oper_group (char* str);
int read_operand (char** ptr,line* line, operand* target);
int read_operands (char** ptr,line* line);
int read_ops (char** ptr, line* line);
int check_oper_address_group (char* str);
int check_valid_operands (line* line);
int read_next_line (char** ptr, line* line);
void find_funct_opcode (char* str, int* funct, int* op_code);


#endif /* PARSING_H */