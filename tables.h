#ifndef MMN14_TABLES_H
#define MMN14_TABLES_H

#include "consts.h"
#include "data.h"
#include "errors.h"
#include "helpers.h"
#include "parsing.h"

int check_dup_label (char* str, symbol* symbol_table);
void update_symbol (symbol** symbol_table, line* line,int DC, char* target, int type); 
int add_data_image (list* cur_line_data, data_image** data_img, int DC);
int add_string_image (char* cur_line_string, data_image** data_img, int DC);
void freelist_symbol (symbol* *ptr);
void freelist_data (data_image* *ptr);
void freelist_code (generated_code* *ptr);
void freelist_external (external* *ptr);
int generate_first_word_code (line* line);
void write_first_word_code (unsigned int first_word,generated_code** code_ptr,int* IC);
void write_code_number (unsigned int number,generated_code** code_ptr,int* IC);
void write_code_skip (generated_code** code_ptr,int* IC);
void update_symbol_first_pass (symbol** symbol_table, int ICF);
int update_symbol_entry (symbol** symbol_table, char* str,int line, char* file);
int find_symbol_address (symbol** symbol_table, char* str);
void update_external_table (external** external_ptr, int IC, char* str);
void write_second_word (int symbol_address,int IC, int address_type, generated_code** code_ptr);

#endif /* TABLES_H */
