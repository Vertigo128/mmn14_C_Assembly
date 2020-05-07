#ifndef MMN14_DATA_H
#define MMN14_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct list { /*general chained list to store data */
	void* value;
	struct list* next;
} list;

typedef struct { /*struct to store operands data*/
    int type; /* address type 0-3 */
    char* name; /*store the address label name*/
    int value; /*store the operand numeric value*/
    int flag; /*store on/off flag*/
}operand;


typedef struct external { /*store external table chained list*/
    char* name; /*label name*/
    int value; /*code address value*/
    struct external* next;
} external;

typedef struct symbol { /*struct to store symbol table*/
    char* name; /*label name*/
    int value; /*label address*/
    int type; /*0 for code, 1 for data, 2 for string, 3 for entry, 4 for extern*/
    int type_e; /*entry 1*/
    struct symbol* next;
} symbol;


typedef struct generated_code { /*struct to store code table*/
    unsigned int word; /*code to store*/
    int counter; /*IC counter*/
    struct generated_code* next; 
} generated_code;

typedef struct data_image { /*struct to store data image table*/
    int type; /* 1 for data int, 2 for char */
    int counter; /*DC counter*/
    void* value; /*pointer value to store*/
    struct data_image* next;
} data_image;

typedef struct error_stat { /*store current error at current line struct*/
    char* err_code; /*error code*/
    int failed; /*Failure flag*/
} error_stat;

typedef struct filenames { /*store the filenames of current file*/
    char name_as[MAX_FILENAME_LENGTH]; /*as file*/
    char name_ob[MAX_FILENAME_LENGTH]; /*ob file*/
    char name_ent[MAX_FILENAME_LENGTH]; /*ent file*/
    char name_ext[MAX_FILENAME_LENGTH]; /*ext file*/
} filenames;

typedef struct { /*struct to store the parsing results of current line*/
    int label_flag; /*Label flag */
    char* label_name;  /*name of label*/
    int op_type;  /*0 for data/directive, 1 for code*/
    int directive_type;  /*1 for data, 2 for string, 3 for entry, 4 for extern*/
    int num_oper; /*number of required operands*/
    list* data; /*linked list to store data*/
    char* str; /*store string directives*/
    operand op_src; /*source operand*/
    operand op_dst; /*destination operand*/
    int num_words; /*calculate the words required for current line*/    
    error_stat error; /*store error flag/type*/
    char* filename; /*current filename*/
    int line_num; /*current line number*/
    

} line;

#endif /* MMN14_DATA_H */

