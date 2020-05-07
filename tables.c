#include <stdio.h>
#include <stdlib.h>
#include "tables.h"


/* Check if current label already exist on symbol table
inputs - str -> string with current label, symbol_table -> current symbol tabel 
return 0 if found, return 1 if passed without duplication */
int check_dup_label (char* str, symbol* symbol_table) {
    symbol* ptr = symbol_table; /*set pointer*/
    if (ptr==NULL) return 1; /*return 1 if table still empty*/
    while (ptr->next != NULL) { /*keep check all nodes of symbol table and check if match to current label*/
        if (!strcmp(ptr->name,str)) return 0; /*if label match to table node, return 0 (failed) */
        ptr = ptr->next; /*advance tabel pointer*/
    }
    if (!strcmp(ptr->name,str)) return 0; /*if label match to table node, return 0 (failed) */
    return 1; /*pass, current label doesn't exist on symbol table*/
}


void update_symbol (symbol** symbol_table, line* line,int DC, char* target, int type);

/* Adds an element to a linked list pointer, and if it doesn't exist (ie. NULL), creates it and assigned to pointer 
inputs - symbol_table -> pointer to symbol table, line -> current line struct, DC - data counter, 
          target ->operand to update, type -> new symbol name*/
void update_symbol (symbol** symbol_table, line* line,int DC, char* target, int type) {
	
    if (*symbol_table == NULL) { /*if symbol table note exist yet, create first node*/
	    *symbol_table = calloc(1, sizeof(symbol));

  	    if (*symbol_table == NULL) { /* Check if calloc failed and exit*/
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
 	    }

	/* update new symbol node with relevant values */
	(*symbol_table)->value = DC; /*update counter*/
        (*symbol_table)->type = type; /*update type*/
        (*symbol_table)->name = str_dup (target); /*copy name to allocated memory pointer in symbol table*/
        (*symbol_table)->next = NULL; /*set next node to null*/

	} else { /*table already exist*/
		symbol* ptr = *symbol_table; /*create ptr as pointer to symbol table*/

		while (ptr->next != NULL) { /* Skip to last element */
			ptr = ptr->next;
		}
		ptr->next = calloc(1, sizeof(symbol)); /*create new node after last node*/
		if (ptr->next == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		/* Put new values in list node */
		ptr->next->value = DC; /*update counter*/
        ptr->next->type = type; /*update type*/
        ptr->next->name = str_dup (target); /*copy name to allocated memory pointer in symbol table*/
        ptr->next->next = NULL; /*set next node to null*/
	}
}

/*Add current line data numbers to data image table
inputs - cur_line_data -> pointer list node containing data from current line, data_img -> pointer to data image table pointer, DC - data counter, 
return the new DC counter*/
int add_data_image (list* cur_line_data, data_image** data_img, int DC){
    int end_flag = 0; /*flag as end of table*/
    if (*data_img == NULL) { /*data image table is not exist, create new*/
		*data_img = calloc(1, sizeof(data_image));

		
		if (*data_img == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
        
        /*update the new node with first data node*/
        (*data_img)->counter = DC++; /*store counter and advance*/
        (*data_img)->type = 1; /*set type to 1 - data*/
        (*data_img)->value = int_dup( cur_line_data->value); /*store results as allocated memory pointer*/
        (*data_img)->next = NULL; /*set next node to null*/
        if (cur_line_data->next != NULL)  cur_line_data = cur_line_data->next; /*advance to next data node of current line if available*/
        else end_flag = 1; /*no further data node exist*/
    }
    /* data image table already exist*/
	{
        data_image* ptr = *data_img; /*set ptr to data image table*/
        

        while (ptr->next != NULL) { /*get to last node of data_image*/
            ptr = ptr->next;
        }
        while (end_flag == 0) { /*while not reach last data node of current line*/
                ptr->next = calloc(1, sizeof(data_image)); /*create new node at data_image table*/
                if (ptr->next == NULL) { /* Check if calloc failed and exit*/
                    fprintf(stderr, ERROR_OUT_OF_MEMORY);
                    exit(1);
                }
                ptr->next->counter = DC++; /*store and update counter*/
                ptr->next->type = 1; /*set type to 1 - data*/
                ptr->next->value = int_dup( cur_line_data->value); /*store results as allocated memory pointer*/
                ptr = ptr->next; /*advance data image table pointer*/
                if (cur_line_data->next != NULL)  cur_line_data = cur_line_data->next; /*advance to next data node of current line if available*/
                else break; /*no further data node exist*/
        }
    }
return DC; /*return the new DC counter*/
}
    

/*Add current line string to data image table
inputs - cur_line_data -> string  from current line, data_img -> pointer to data image table pointer, DC - data counter, 
return the new DC counter*/
int add_string_image (char* cur_line_string, data_image** data_img, int DC){
    if (*data_img == NULL) { /*data image table is not exist, create new*/
		*data_img = calloc(1, sizeof(data_image));

		if (*data_img == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}        
        /*update the new node with first data node*/
        (*data_img)->counter = DC++; /*store and update counter*/
        (*data_img)->type = 2; /*set type to 2 - string*/
        (*data_img)->value = charecter_dup(*cur_line_string); /*copy charecter pointer to data image table*/
        cur_line_string++;  /*advance string pointer*/

    }
    {
        data_image* ptr_img = *data_img; /*set ptr to data image table*/
        while (ptr_img->next != NULL) { /*get to last node of data_image*/
            ptr_img = ptr_img->next;
        }
        while (*cur_line_string != '\0'){  /*keep store data until end of string*/
                ptr_img->next = calloc(1, sizeof(data_image));  /*create new node at data_image table*/
                if (ptr_img->next == NULL) { /* Check if calloc failed and exit*/
                    fprintf(stderr, ERROR_OUT_OF_MEMORY);
                    exit(1);
                }
                ptr_img->next->counter = DC++; /*store and update counter*/
                ptr_img->next->type = 2; /*set type to 2 - string*/
                ptr_img->next->value = charecter_dup(*cur_line_string); /*copy charecter pointer to data image table*/
                ptr_img = ptr_img->next; /*advance to next data image node*/
                cur_line_string++; /*advance string pointer*/
        }
        /*add last charected /0 node at data image table*/
        ptr_img->next = calloc(1, sizeof(data_image)); /*create new nore*/
        if (ptr_img->next == NULL) { /* Check if calloc failed and exit */
            fprintf(stderr, ERROR_OUT_OF_MEMORY);
            exit(1);
        }
        ptr_img->next->counter = DC++;  /*store and update counter*/
        ptr_img->next->type = 2; /*set type to 2 - string*/
        ptr_img->next->value = charecter_dup('\0'); /*add pointer to \0 character*/
        ptr_img->next->next = NULL; /*set next node to NULL*/
        return DC; /*return the updated DC*/
    }
    
}


/* Function to free the symbol table memory
inputs - ptr -> pointer to symbol table*/
void freelist_symbol (symbol* *ptr){
    symbol* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr; 
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}


/* Function to free the data table memory
inputs - ptr -> pointer to data table*/
void freelist_data (data_image* *ptr){
    data_image* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr;
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}

/* Function to free the code table memory
inputs - ptr -> pointer to code table*/
void freelist_code (generated_code* *ptr){
    generated_code* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr;
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}

/* Function to free the external table memory
inputs - ptr -> pointer to external table*/
void freelist_external (external* *ptr){
    external* temp_ptr; /*set temp pointer*/
    while (*ptr){ /*loop through nodes while exist*/
        temp_ptr = *ptr;
        *ptr = temp_ptr->next; /*move to next node*/
        free (temp_ptr); /*free previous node*/
    }
}

/* Write the first word code to the code table struct 
inputs - line -> pointer to line struct*/
int generate_first_word_code (line* line){
    unsigned int ARE = 1<<2; /*A bit is always 1 on first word*/
    unsigned int funct; /*bits 3-7*/
    unsigned int reg_src = 0; /*bits 13-15*/
    unsigned int address_src =0; /*bits 16-17*/
    unsigned int reg_dst = 0 ; /*bits 18-10*/
    unsigned int address_dst = 0; /*bits 11-12*/
    unsigned int opcode; /*bits 18-23*/
    unsigned int word; /*final word*/

    find_funct_opcode (line->str, &funct, &opcode);  /*call function to get funct and op_code values*/
    if (line->op_src.type == 3) reg_src = (int) (line->op_src.name[1]) - 48; /*store source registery number*/
    if (line->op_dst.type == 3) reg_dst = (int) (line->op_dst.name[1]) - 48; /*store destination registery number*/
    address_src = line->op_src.type; /*store source address*/
    address_dst = line->op_dst.type; /*store destination  address*/
     /*bitwise shift values according to definitions*/
    reg_src <<= REG_SRC_FIRST_BIT;
    reg_dst <<= REG_DST_FIRST_BIT;
    funct <<= FUNCT_FIRST_BIT;
    address_src <<= ADDRESS_SRC_FIRST_BIT;
    address_dst <<= ADDRESS_DST_FIRST_BIT;
    opcode <<=OP_CODE_FIRST_BIT;
    word = ARE | funct | reg_dst | address_dst | reg_src | address_src | opcode;  /*combine all variables with bitwise or*/
    return word; /*return calculated number*/
}

/* write the generated code in the next available code node , advance IC by 1 
inputs - first_word -> word to write to code table, code_ptr -> pointer to code table, IC - pointer to current IC counter*/
void write_first_word_code (unsigned int first_word,generated_code** code_ptr,int* IC){
    if (*code_ptr == NULL) { /*if code table not exixt, create one*/
		*code_ptr = calloc(1, sizeof(generated_code));

		
		if (*code_ptr == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		/* update values at first node */
		(*code_ptr)->word = first_word; /*store word*/
        (*code_ptr)->counter = *IC; /*store IC counter*/
        (*code_ptr)->next = NULL; /*set next node to NULL*/
        (*IC)++; /*updat counter*/

	} else { /*Code table already defined*/
        generated_code* ptr = *code_ptr; /*set ptr to code table*/

		/* Skip to last element */
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = calloc(1, sizeof(generated_code)); /*create new node*/
		if (ptr->next == NULL) { /* Check if calloc failed and exit*/
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		ptr->next->word = first_word; /*store word*/
        ptr->next->counter = *IC; /*store IC counter*/
        ptr->next->next = NULL;/*set next node to NULL*/
        (*IC)++; /*updat counter*/
    }
}


/* write the immidite number code in the next available code node, advance IC by 1 
inputs - first_word -> word to write to code table, code_ptr -> pointer to code table, IC - pointer to current IC counter*/
void write_code_number (unsigned int number,generated_code** code_ptr,int* IC){

    int ARE = 1<<2; /*A bit is on for immidiate address*/
    int calculated_num = (number << 3 | ARE); /*shift stored number by 3 and combine with ARE*/
    generated_code* ptr = *code_ptr; /*set ptr to code table*/

    /* Skip to last element */
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = calloc(1, sizeof(generated_code)); /*create new node*/
    if (ptr->next == NULL) { /* Check if calloc failed  and exit */
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(1);
    }
    ptr->next->word = calculated_num;  /*store calculated number and ARE as word*/
    ptr->next->counter = *IC; /*store IC counter*/
    ptr->next->next = NULL; /*set next node to NULL*/
    (*IC)++; /*updat counter*/
   
}

/* add empty code line for second pass, advance IC by 1 
inputs - code_ptr -> pointer to code table, IC - pointer to current IC counter*/
void write_code_skip (generated_code** code_ptr,int* IC){

    generated_code* ptr = *code_ptr; /*set ptr to code table*/
    /* Skip to last element */
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = calloc(1, sizeof(generated_code)); /*create new node*/
    if (ptr->next == NULL) {  /* Check if calloc failed and exit*/
        fprintf(stderr, ERROR_OUT_OF_MEMORY);
        exit(1);
    }
    ptr->next->counter = *IC; /*store IC counter*/
    ptr->next->next = NULL; /*set next node to NULL*/
    (*IC)++; /*updat counter*/
}



/* update symbol data table to add ICF to every data label value 
inputs - symbol_table -> pointer to symbol table pointer, ICF - final code counter*/
void update_symbol_first_pass (symbol** symbol_table, int ICF) {
	
		symbol* ptr = *symbol_table; /*set pointer*/
		while (ptr->next != NULL) { /*keep looping while not last node*/
            if (ptr->type == 1 || ptr->type == 2) ptr->value += ICF; /*if symbol type is data/string then update counter by ICF*/
			ptr = ptr->next;
		}
        if (ptr->type == 1) ptr->value += ICF; /*update last node - if symbol type is data/string then update counter by ICF*/
	
}

/* update symbol data entry flag 
inputs - symbol_table -> pointer to symbol table pointer, str - current entry label, line - current input line, file - current filename
return 0 if no symbol found, 1 if pass */
int update_symbol_entry (symbol** symbol_table, char* str,int line, char* file){
    symbol* ptr = *symbol_table; /*set pointer*/
    int found_flag = 0; /*flag if symbol found*/
    while (ptr->next != NULL) { /*keep looping while not last node*/
            if (!strcmp(str,ptr->name)){ /*keep search symbol table, if match to current, set entry flag to 1*/
                ptr->type_e = 1;
                found_flag = 1;
            }
			ptr = ptr->next;
		}
    if (found_flag==0) {fprintf (stderr, ERROR_ENTRY_LABEL_NOT_FOUND, line, file ); return 0;} /*raise error if entry label note found on symbol table*/
    return 1; /*pass */
}


/* find current operand label in symbol table  
inputs - symbol_table -> pointer to symbol table pointer, str - current entry label.
return 0 if no symbol found, -1 if label is external and adress counter if found */
int find_symbol_address (symbol** symbol_table, char* str){
  
    symbol* ptr = *symbol_table; /*set pointer*/
    while (ptr->next != NULL) { /*keep looping while not last node*/
        if (!strcmp(str,ptr->name)) { /*check if str match current node*/
            if (ptr->type != 4) return (ptr->value); /*if match to current and not external return adress*/
            else return -1; /*return -1 if external */
        }
        ptr = ptr->next; 
	}
    if (!strcmp(str,ptr->name)) { /*check also last node*/
            if (ptr->type != 4) return (ptr->value); /*if match to current and not external return adress*/
            else return -1; /*return -1 if external */
        }
    return 0; /*no symbol found */
}


/* Create and update external table 
inputs - external_table -> pointer to external table pointer, IC -> code counter, str ->current label.*/
void update_external_table (external** external_ptr, int IC, char* str) {
    if (*external_ptr == NULL) { /*if table not exist, create new*/
		*external_ptr = calloc(1, sizeof(external));

		
		if (*external_ptr == NULL) { /* Check if calloc failed  and exit */ 
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		(*external_ptr)->name = str_dup (str); /*store current label at new node*/
        (*external_ptr)->value = IC; /*store current counter at new node*/

	} else { /*External table exist*/
        external* ptr = *external_ptr; /*set ptr to external table*/
		while (ptr->next != NULL) { /* Skip to last element */
			ptr = ptr->next;
		}
		ptr->next = calloc(1, sizeof(external)); /*create new node*/
		if (ptr->next == NULL) { /* Check if calloc failed and exit */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		ptr->next->name = str_dup (str); /*store current label at new node*/
        ptr->next->value = IC; /*store current counter at new node*/
        ptr->next->next = NULL; /*set next node ot NULL */
    }

}


/* write the second pass, add missing labels code in the next relevant code list , advance IC by 1 
inputs - symbol_address - adress of current symbol, IC -> code counter, 
          address+type -> type of address, code_ptr-> pointer to code table pointer */
void write_second_word (int symbol_address,int IC, int address_type, generated_code** code_ptr){
    
    int ARE; /*define ARE bits*/
    int calculated_num; /*temp number for calculation*/
    if (symbol_address == -1) calculated_num = 1; /*if external, calculated number is 1*/
    else {
        ARE = 1<<address_type; /* shift ARE per address type, if direct (1) R bit is on, if relative (2) L bit is on*/
        calculated_num =  (symbol_address<<3 | ARE); /*calculate the required word by combine the ARE and the shifted number to bits 3-24*/
    }
    {
	generated_code* ptr = *code_ptr; /*set pointer*/

	while (ptr->counter != IC) { /*skip to last node*/
	    ptr = ptr->next;
	}

	ptr->word = calculated_num; /*store the calculated word*/
    }

}
