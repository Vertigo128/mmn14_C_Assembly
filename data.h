#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct list {
	void* value;
	struct list* next;
} list;

typedef struct {
    int type; /* address type 0-3 */
    char* name;
    int value;
    int flag;
}operand;


typedef struct external {
    char* name;
    int value;
    struct external* next;
} external;

typedef struct symbol {
    char* name;
    int value;
    int type; /*0 for code, 1 for data, 2 for string, 3 for entry, 4 for extern*/
    int type_e; /*entry 1*/
    struct symbol* next;
} symbol;


typedef struct generated_code {
    unsigned int word; 
    int counter;
    struct generated_code* next; 
} generated_code;

typedef struct data_image {
    int type; /* 1 for data int, 2 for char */
    int counter;
    void* value;
    struct data_image* next;
} data_image;

typedef struct error_stat {
    char* err_code;
    int failed;
} error_stat;

typedef struct filenames {
    char name_ps[MAX_FILENAME_LENGTH];
    char name_ob[MAX_FILENAME_LENGTH];
    char name_ent[MAX_FILENAME_LENGTH];
    char name_ext[MAX_FILENAME_LENGTH];
} filenames;

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


