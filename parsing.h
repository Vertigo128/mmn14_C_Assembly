#define Max_Line_Length 80
#define Word_Length 24
#define MAX_Label_Length 31
#define MAX_Directive_Length 6
#define MAX_OPS_Char_Length 4
#define MAX_Data_Value 83888608 /*2^23 allowed bits*/
#define MIN_Data_Value -83888608 /*2^23 allowed bits*/
#define MAX_NUM_Value 1048576 /*2^21 allowed bits*/
#define MIN_NUM_Value -1048576 /*2^21 allowed bits*/
#define NUM_DIRECTIVE 4
#define NUM_OPS 16
#define NUM_REGS 7

// #define reg_mex - 7
// #define reg_size - 24 ?
// #define ops_max - 15
// #define funct_min - 1
// #define label_max - 31
// #define code_start - 100

const char* ops[NUM_OPS] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"};
const char* directive[NUM_DIRECTIVE] = {"data", "string","entry","extern"};
const char* regs[NUM_REGS] = {"r1", "r2","r3","r4","r5","r6","r7"};



