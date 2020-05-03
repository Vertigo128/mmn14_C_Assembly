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

// typedef struct base2 {
//     unsigned int num:24; 
// } base2;

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

// typedef struct {
//     int IC; 
//     int ARE;  /*0-3*/
//     int op_src_type;
//     int op_src_reg;
//     int op_dst_type;
//     int op_dst_reg;
//     int op_code;
//     int func;
//     void* word1;
//     void* word2;     
// } first_word;



typedef struct {
	/* Assembler tables */
    list* symbol;
	// list* entry_table; /* List of type 'entry_with_line_num' */
	// list* extern_table; /* List of type 'char*' */
	// list* extern_refs_table; /* List of type 'ref_in_code' */
	// list* data_labels_table; /* List of type 'data_label' */
	// list* code_labels_table; /* List of type 'code_label' */
	// list* code_label_refs_table; /* List of type 'ref_in_code' */
	// cpu_word* data_table; /* Points to start of data table */
	// unsigned int data_counter;
	// cpu_word* code_table; /* Points to start of code table */
	// unsigned int code_counter;

	// /* Current file info */
	// char* current_file_name;
	// FILE* current_file_ptr;
	// int current_line_num;
	// int failed;
} status;
