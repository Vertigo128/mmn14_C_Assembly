#ifndef MMN14_CONST_H
#define MMN14_CONST_H

#define Max_Line_Length 80 /*maximum line length*/
#define MAX_Label_Length 31 /*maximum allowed label length*/
#define MAX_Directive_Length 6 /*maximum number of characters in directive command*/
#define MAX_OPS_Char_Length 4 /*maximum number of characters in code command*/
#define MAX_Data_Value 83888608 /*2^23 allowed bits*/
#define MIN_Data_Value -83888608 /*2^23 allowed bits*/
#define MAX_NUM_Value 1048576 /*2^21 allowed bits*/
#define MIN_NUM_Value -1048576 /*2^21 allowed bits*/
#define NUM_DIRECTIVE 4 /*number of directives*/
#define NUM_OPS 16 /*number of code operations*/
#define NUM_REGS 7 /*number of registery */
#define CODE_START 100 /*code counter start */
#define FIRST_PASS_EXTERN_VAL 0 /*the adress value of extern directive*/
#define SYMBOL_DIRECTIVE_CODE 0 /*directive code on symbol table*/
#define MAX_FILENAME_LENGTH 128 /*define maximum length for input file name*/
#define FUNCT_FIRST_BIT 3 /*first code word funct start bit*/
#define REG_DST_FIRST_BIT 8 /*first code word  desination register start bit*/
#define ADDRESS_DST_FIRST_BIT 11 /*first code word destination address start bit*/
#define REG_SRC_FIRST_BIT 13 /*first code word source register start bit*/
#define ADDRESS_SRC_FIRST_BIT 16 /*first code word source address start bit*/
#define OP_CODE_FIRST_BIT 18 /*first code word op_code start bit*/
#define CUR_CHAR **ptr /*Helper macro to input pointer*/

#define CUR_CHAR **ptr /*Helper macro to input pointer*/

extern const char* ops[NUM_OPS];
extern const char* directive[NUM_DIRECTIVE];
extern const char* regs[NUM_REGS];


#endif /* CONST_H */