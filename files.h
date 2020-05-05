#ifndef MMN14_FILES_H
#define MMN14_FILES_H

#include <stdio.h>
#include <string.h>
#include "consts.h"
#include "data.h"
#include "errors.h"


int open_ps_name (char** argv,filenames* files, int file_counter, FILE** file);
void print_code (generated_code* code_ptr, data_image* data_ptr, int ICF, int DCF, char* file_ob);
void print_external (external* external_ptr, char* file_ext);
void print_entry (symbol* symbol_ptr, char* file_ent);

#endif /*FILES_H*/