#ifndef MMN14_ASSMBLER_H
#define MMN14_ASSMBLER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "consts.h"
#include "data.h"
#include "errors.h"
#include "helpers.h"
#include "parsing.h"
#include "tables.h"
#include "files.h"
const char* ops[NUM_OPS] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"}; /*list of code command*/
const char* directive[NUM_DIRECTIVE] = {"data", "string","entry","extern"}; /*list of directives*/
const char* regs[NUM_REGS] = {"r1", "r2","r3","r4","r5","r6","r7"}; /*list of registers name*/

void initilize_line (line* line); /*define function to initilize current line struct*/

#endif /* ASSMBLER_H */