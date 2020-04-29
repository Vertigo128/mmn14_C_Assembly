#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct list {
	void* value;
	struct list* next;
} list;

typedef struct {
    int type; /* address type 0-3*/
    char* name;
    int value;
}operand;

typedef struct external {
    char* name;
    int value;
    struct external* next;
} external;

typedef struct symbol {
    char* name;
    int value;
    int type; /*data 0, code 1*/
    int type_e; /*entry 0, external 1*/
    struct symbol* next;
} symbol;

typedef struct error_stat {
    char* err_code;
    int failed;
} error_stat;

typedef struct {
    int label_flag;
    char* label_name; 
    int op_type;  /*0 for data/directive, 1 for code*/
    int directive_type;  /*1 for data, 2 for string, 3 for entry, 4 for extern*/
    int num_oper;
    list* data;
    char* str;
    operand op_src;
    operand op_dst;
    int num_words; /*calculate the words required for current line*/    
    error_stat error;
    char* filename;
    int line_num;
    

} line;

typedef struct {
    int IC; 
    int ARE;  /*0-3*/
    int op_src_type;
    int op_src_reg;
    int op_dst_type;
    int op_dst_reg;
    int op_code;
    int func;
    void* word1;
    void* word2;     
} first_word;
