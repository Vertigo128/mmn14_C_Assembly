#ifndef MMN14_helpers_H
#define MMN14_helpers_H

#include <stdio.h>
#include <string.h>
#include "consts.h"
#include "data.h"
#include "errors.h"
#include "helpers.h"


void list_add_element (list** linked_list, void* data);
long int* int_dup (long int* num);
char* charecter_dup (char str);
char* str_dup (char* str);
char skip_space(char** ptr);
int expect_non_EOL_WS (char** ptr);
int search_word(char c, char* ptr);

#endif /* HELPERS_H */
