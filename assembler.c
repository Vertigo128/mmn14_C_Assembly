#include "assembler.h"


int main (int argc, char *argv[]){
    
    
    FILE *file_as; /*pointer to current as file*/
    char str[Max_Line_Length +1 ] ; /*string to store current line*/
    char *ptr = str; /*create pointer to current line string*/
    line cur_line; /*create current line parsing struct*/
    filenames files; /*create filenames struct */
    int file_counter = 1; /*count number of files*/
    unsigned int first_word; /*store current line first code word after parsing */
    int ICF; /*ICF define final code counter after first pass*/
    int DCF; /*DCF define final data counter after first pass*/
    generated_code* code_ptr = NULL; /*initilize code table pointer*/
    data_image* data_ptr = NULL; /*initilize data image table pointer*/
    symbol* symbol_table_ptr =NULL; /*initilize symbol table pointer*/
    external* external_table_ptr = NULL; /*initilize external table pointer*/
    
    
    if (argc==1) {fprintf (stderr, ERROR_MISSING_FILE); return (1);}; /*raise error and exit if no as file passed*/

    while (file_counter<=(argc-1)) { /*keep while there is more as files to compile*/
        int DC = 0; /*initilize data counter*/
        int IC = CODE_START; /*initilize code counter*/
        int error_flag = 0; /*set error flag to 0 */
        freelist_symbol (&symbol_table_ptr); /*free symbol table*/
        freelist_data (&data_ptr); /*free data image table*/
        freelist_code (&code_ptr); /*free code table*/
        freelist_external (&external_table_ptr); /*free external table*/
        if (!open_as_name (argv, &files, file_counter, &file_as)) {return 0;}; /*call function to open next file and save to file_as pointer, exit if failed*/
        file_counter++; /*increase file counter by 1*/
        cur_line.filename = files.name_as; /*set and store file name*/
        cur_line.line_num = -1; /*set line counter to -1*/
        while ((ptr = fgets (str,Max_Line_Length+1,file_as))) { /*keep parsing and first pass until EOF*/
            initilize_line (&cur_line);  /*call funtion to initilize current line struct*/
            cur_line.line_num++; /*increase line counter*/
            if (read_next_line (&ptr, &cur_line)) { 
                /*call main function to parse current line, store parsing at line struct and if succeed start first pass*/
                if (cur_line.directive_type==1 || cur_line.directive_type==2) { /*if directive is data or string*/
                    if (cur_line.label_flag==1) { /*if label flag is on*/
                    /* check if label already exist on symbol table*/
                        if (!check_dup_label(cur_line.label_name, symbol_table_ptr)) {print_error (ERROR_LABEL_EXIST,&cur_line);error_flag = 1; continue;}
                        update_symbol(&symbol_table_ptr,&cur_line,DC, cur_line.label_name,cur_line.directive_type); /*call function to update symbol table*/
                    }
                    if (cur_line.directive_type==1){ /*update data image with current line data */
                        DC = add_data_image (cur_line.data, &data_ptr, DC);
                    }
                    if (cur_line.directive_type==2){ /*update data image with current line string */
                        DC = add_string_image (cur_line.str, &data_ptr, DC);
                    }
                }
                else if (cur_line.directive_type==3) continue; /*if entry move to next line*/
                /*if external update symbol table*/
                else if (cur_line.directive_type==4) update_symbol(&symbol_table_ptr,&cur_line,FIRST_PASS_EXTERN_VAL, cur_line.str,cur_line.directive_type);
                else { /*****Code command *****/
                    if (cur_line.label_flag==1) { /*if label flag is on*/
                        /* check if label already exist on symbol table*/
                        if (!check_dup_label(cur_line.label_name, symbol_table_ptr)) {print_error (ERROR_LABEL_EXIST,&cur_line); error_flag = 1; continue;}
                        update_symbol(&symbol_table_ptr,&cur_line,IC, cur_line.label_name,SYMBOL_DIRECTIVE_CODE); /*call function to update symbol table*/
                    }
                    first_word =  generate_first_word_code (&cur_line); /*call function to generate first word and store results*/
                    /* print_word (first_word); option to run debugging to print the generated word */
                    write_first_word_code (first_word,&code_ptr, &IC); /*call function to write first word to code table*/
                    if (cur_line.num_words==1); /*if current line require 1 line of code */
                    else if (cur_line.op_src.flag==0){ /*if src operand flag is off */
                    /*if dst operand type is direct(1) relative (2) call function to create empty line at code table*/
                        if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) write_code_skip(&code_ptr, &IC); 
                      /*if dst operand type is immidiate(0) call function to write number to code table*/
                        if (cur_line.op_dst.type==0) write_code_number(cur_line.op_dst.value, &code_ptr, &IC);
                    }
                    else if (cur_line.op_src.flag==1){/*if src flag is on, need to read both operands*/
                    /*if src operand type is direct(1) relative (2) call function to create empty line at code table*/
                        if (cur_line.op_src.type==1 || cur_line.op_src.type==2) write_code_skip(&code_ptr, &IC);
                        /*if src operand type is immidiate(0) call function to write number to code table*/
                        if (cur_line.op_src.type==0) write_code_number(cur_line.op_src.value, &code_ptr, &IC);
                        /*if dst operand type is direct(1) relative (2) call function to create empty line at code table*/
                        if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) write_code_skip(&code_ptr, &IC);
                        /*if dst operand type is immidiate(0) call function to write number to code table*/
                        if (cur_line.op_dst.type==0) write_code_number(cur_line.op_dst.value, &code_ptr, &IC);
                    }
                    

                }
        	/*Help debugging function to print tables  
           print_tables(symbol_table_ptr,data_ptr, code_ptr,external_table_ptr); */

            }
            else { /*if parsing failed, raise the error_flag and print current line */
                if (cur_line.error.failed == 1) {
		    error_flag = 1; 
                    printf ("Error on line: %s", str);
		}
            }
            
        } /*finished parsing and first pass  */

        if (error_flag==1) continue; /*move to next file if error found during parsing*/
        ICF = IC; /*store final code counter*/
        DCF = DC; /*store final data counter*/
        update_symbol_first_pass (&symbol_table_ptr, ICF); /*update data items in symbol table with new counter*/
        
        /*Help debugging function to print tables  
           print_tables(symbol_table_ptr,data_ptr, code_ptr,external_table_ptr); */

        /******Second Pass ************/

        rewind (file_as); /*start second pass from start of file */
        IC = CODE_START; /*intilize code counter back to start*/

        while ((ptr = fgets (str,Max_Line_Length+1,file_as))) { /*start parsing and second pass until EOF*/
            initilize_line (&cur_line); /*initilize current line struct*/
            read_next_line (&ptr, &cur_line); /*call funtion to parse current line and store results at line struct*/
            /* if directive is data, string or external move to next line*/
            if (cur_line.directive_type==1 || cur_line.directive_type==2 || cur_line.directive_type == 4) continue;
            /* if directive is entry - update symbol entry flag to on, raise error if label is not found*/
            else if (cur_line.directive_type==3) {
                if (!update_symbol_entry(&symbol_table_ptr, cur_line.str,cur_line.line_num, cur_line.filename));
            }
            else { /*Code only */
                if (cur_line.num_words == 1) {IC++; continue;} /*if one word is required, increase counter and move to next line*/
                if (cur_line.op_src.flag==0 || cur_line.op_src.type==3 ){  /*if no source operand or if src operand is registery (3)*/
                    if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) { /*if destination operand is direct(1) or relative (2) */
                        int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_dst.name); /*find operand in symbol table and store address*/
                        if (!symbol_address) { /*raise error if label operand not found in symbol table*/
                            fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename);
                            break;
                        }
                        /* if adress is external (-1) update external table with next line counter and destination operand name*/
                        else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+1, cur_line.op_dst.name);};
                        /*if destination operand is relative, calculate the required code step*/
                        if (cur_line.op_dst.type==2) symbol_address = symbol_address - IC;
                        /*write second word to code table with updated adress, counter and operand type*/
                        write_second_word(symbol_address,IC+1,cur_line.op_dst.type, &code_ptr);
                        
                    }
                }
                else { /*Two additional words required*/
                    if (cur_line.op_src.type==1 || cur_line.op_src.type==2) {/*if source operand is direct(1) or relative (2) */
                        int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_src.name); /*find operand in symbol table and store address*/
                        if (!symbol_address) { /*raise error if label operand not found in symbol table*/
                            fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename); 
                            break;}
                        /* if adress is external (-1) update external table with next line counter and source operand name*/
                        else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+1, cur_line.op_src.name);};
                        /*if source operand is relative, calculate the required code step*/
                        if (cur_line.op_src.type==2) symbol_address = symbol_address - IC;
                        /*write second word to code table with updated adress, counter and operand type*/
                        write_second_word(symbol_address,IC+1,cur_line.op_src.type, &code_ptr);
                        
                    } /*if destination operand is direct(1) or relative (2) */
                    if (cur_line.op_dst.type==1 || cur_line.op_dst.type==2) {
                        int symbol_address = find_symbol_address (&symbol_table_ptr, cur_line.op_dst.name); /*find operand in symbol table and store address*/
                        if (!symbol_address) { /*raise error if label operand not found in symbol table*/
                            fprintf (stderr, ERROR_LABEL_NOT_FOUND,cur_line.line_num, cur_line.filename); 
                            break;}
                         /* if adress is external (-1) update external table with next line counter and source operand name*/
                        else if (symbol_address == -1 ) {update_external_table (&external_table_ptr, IC+2, cur_line.op_dst.name);};
                        /*if destination operand is relative, calculate the required code step*/
                        if (cur_line.op_dst.type==2) symbol_address = symbol_address - IC;
                         /*write second word to code table with updated adress, counter and operand type*/
                        write_second_word(symbol_address,IC+2,cur_line.op_dst.type, &code_ptr);
                    }
                }
                
            }
            IC += cur_line.num_words; /*update code counter with number of code words in current line*/

        
        } /*finish second pass*/

        /* print_tables(symbol_table_ptr,data_ptr, code_ptr, external_table_ptr); Help debugging function to print tables */
        print_code (code_ptr, data_ptr, ICF,DCF,files.name_ob); /*call funcation to create ob and write code and data table*/
        print_external (external_table_ptr,files.name_ext); /*call function to create ext file and write external table*/
        print_entry (symbol_table_ptr, files.name_ent); /*call function to create entry file and write entry table*/
        

    } /*finished compile all files*/
    printf ("\n Done! \n");
    return 0;
}


/* Initilize current line parsing struct
inputs - line ->pointer line struct */
void initilize_line (line* line){ /*set struct main values to 0*/
    line->op_dst.type = 0;
    line->op_src.type = 0;
    line->op_dst.flag = 0;
    line->op_src.flag = 0;
    line->op_src.value = 0;
    line->op_dst.value = 0;
    line->op_src.name = NULL;
    line->op_dst.name = NULL;
    line->label_flag = 0;
    line->data = NULL;
    line->str = NULL;
    line->num_words = 1;
    line->error.failed =0;
    line->directive_type=0;
    line->op_type=0;
}
