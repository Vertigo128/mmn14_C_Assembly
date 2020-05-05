#include "printing.h"

/* the function print input error and store error on current line struct
inputs - type-> type of error, line -> pointer to current line struct*/
void print_error (char* type,line* line){
    line->error.failed = 1; /* set error flag to 1 */
    line->error.err_code = type; /*store the error flag */
    fprintf (stderr, type, line->line_num, line->filename ); /*print error to stderr*/

}

/* helper function for debugging to print the struct line 
inputs - line ->pointer line struct */
void print_line (line* line){
    
    if (line->label_flag==1)  printf("Label Flag is: %d. name is %s\n",line->label_flag, line->label_name); /*print label if exist*/

    if (line->op_type==1) { /*print operation and operands if operation is code*/
        printf ("Oper name: %s\n",line->str);
        printf ("Num ops: %d\n",line->num_oper);
        printf ("Src type: %d\n",line->op_src.type);        
        printf ("Dest type: %d\n",line->op_dst.type);
        printf ("Src value: %d\n",line->op_src.value);        
        printf ("Dest value: %d\n",line->op_dst.value);
        printf ("Src name: %s\n",line->op_src.name);        
        printf ("Dest name: %s\n",line->op_dst.name);

    }
    if (line->op_type==0){ /*print directive name and values if operation is data*/
        printf ("Directive_type: %d\n",line->directive_type);
        if (line->directive_type==1) printf ("First Num is : %ld\n",*(long int*) line->data->value);
        else printf ("string/directive lable is : %s\n",line->str);
    }

}


/* Helper Function to print to screen current word
inputs - word -> current word*/
void print_word (int word){

    int j=23;
    int i;

    for (i =1 << 23; i ; i>>=1, j--){ /*loop through bits and print value*/
        printf ("%c", word&i? '1' : '0');
        /* if (j==3 || j== 8 || j==11 || j== 13 || j==16 || j==18) printf ("|");  option  to add delimiters per fields*/
    }
    printf ("\n");
}


/* Debug helper function to print generated tables 
inputs - symbol_table_ptr -> pointer to symbol table, data_ptr -> pointer to data image table, 
        code_ptr -> pointer to code table, external_par -> pointer to external table*/
void print_tables (symbol* symbol_table_ptr, data_image* data_ptr, generated_code* code_ptr, external* external_ptr){
    
    while (symbol_table_ptr!=NULL) { /*loop through symbol table and pring values*/
        printf ("\n Symbols: val : %d, name : %s, type : %d, type_e : %d", symbol_table_ptr->value, symbol_table_ptr->name, symbol_table_ptr->type, symbol_table_ptr->type_e);
        if (symbol_table_ptr->next !=NULL) symbol_table_ptr= symbol_table_ptr ->next;
        else break;
    }
    while (data_ptr!=NULL) { /*loop through data image table and pring values*/
        printf ("\n Data: type : %d, counter : %d", data_ptr->type, data_ptr->counter);
        if (data_ptr->type==1) printf (" values is : %ld", *(long int*) data_ptr->value);
        else if (data_ptr->type==2) printf (" values is : %c ", *(char*) data_ptr->value);
        if (data_ptr->next !=NULL) data_ptr = data_ptr->next;
        else break;
    }

    while (code_ptr!=NULL) { /*loop through code table and pring values*/
        printf ("\n Code: counter : %d, code :  " , code_ptr->counter);
        print_word (code_ptr->word);
        if (code_ptr->next !=NULL) code_ptr = code_ptr->next;
        else break;
    }

    while (external_ptr!=NULL) {/*loop through external table and pring values*/
        printf ("\n External: counter : %d, code : %s " , external_ptr->value, external_ptr->name);
        if (external_ptr->next !=NULL) external_ptr = external_ptr->next;
        else break;
    }
    printf ("\n");

}

