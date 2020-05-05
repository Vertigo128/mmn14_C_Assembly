#ifndef MMN14_PRINTING_H
#define MMN14_PRINTING_H

#include <stdio.h>
#include <string.h>
#include "consts.h"
#include "data.h"
#include "errors.h"

void print_error (char* type,line* line);
void print_line (line* line);
void print_word (int word);
void print_tables (symbol* symbol_table_ptr, data_image* data_ptr, generated_code* code_ptr, external* external_ptr);

#endif /*PRINTING_H */