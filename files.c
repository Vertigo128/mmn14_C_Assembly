#include "files.h"

/*The function open the next input file and save results to file pointer
inputs - argv -> pointer to argv array of input file names, files -> pointer to struct with current file names
         file_counter -> counter of number of files, file -> pointer to new file
return 0 if failed, 1 if passed*/
int open_ps_name (char** argv,filenames* files, int file_counter, FILE** file){
    
    char str_temp[MAX_FILENAME_LENGTH]; /*create temp file name string*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ps,strcat(str_temp, ".ps")); /*concatenate .ps extention and store result at files struct*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ent,strcat(str_temp, ".ent")); /*concatenate .ent extention and store result at files struct*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ext,strcat(str_temp, ".ext")); /*concatenate .ext extention and store result at files struct*/
    strcpy (str_temp,argv[file_counter]); /*copy current filename to str_temp*/
    strcpy (files->name_ob,strcat(str_temp, ".ob")); /*concatenate .ob extention and store result at files struct*/
        
    if (!(*file = fopen (files->name_ps,"r"))){ /*open ps file in read mode before parsing*/
        fprintf (stderr, ERROR_OPEN_FILE); /*send error and exit if failed open file*/
        return (0); 
    }
    return 1; /*pass*/
}


/* write the generated code to the ob file 
inputs - code_ptr -> pointer to code table, data_ptr -> pointer to data image table, 
        ICF - final code counter, DCF - final data counter, file_ob - file name to save results. */
void print_code (generated_code* code_ptr, data_image* data_ptr, int ICF, int DCF, char* file_ob){
    FILE *current_file = NULL; /*create file pointer */
    char str[10]; /*set temp char array*/
    char str_cpy[6]; /*set temp char array at final size*/
    int num; /*temp num */
    int str_len; /*store the converted to hex str length*/

    if (!(current_file = fopen (file_ob,"wt"))){ /*open at write mode the ob file*/
        fprintf (stderr, ERROR_WRITE_FILE);
        exit (0); 
    }
    fprintf (current_file,"\t%d %d", ICF-100, DCF); /*print header to file */
    while (code_ptr!=NULL) { /*Printing code section*/
        num = (unsigned int) code_ptr->word; /*cast word to number */
        sprintf(str, "%06x", num); /*convert word to hex as string */
        str_len = strlen(str); /*get string length*/
        if (str_len>6) strcpy (str,str+(str_len-6) ); /*trim the string back to 6 digits*/
        fprintf (current_file,"\n%07d ", (code_ptr->counter)); /*print to file current counter*/
        fprintf (current_file,"%s", str); /*print to file converted hex string*/
        if (code_ptr->next !=NULL) code_ptr = code_ptr->next; /*skip to next node*/
        else break;    
    }
    while (data_ptr!=NULL) { /*Printing data section*/
        num = *(unsigned int*) data_ptr->value; /*cast word to number */
        sprintf(str, "%06x", num); /*convert word to hex as string */
        str_len = strlen(str); /*get string length*/
        if (str_len>6) strcpy (str,str+(str_len-6) ); /*trim the string back to 6 digits*/
        fprintf (current_file,"\n%07d ", (data_ptr->counter + ICF)); /*print to file current counter + ICF*/
        fprintf (current_file,"%s", str); /*print to file converted hex string*/
        if (data_ptr->next !=NULL) data_ptr = data_ptr->next; /*skip to next node*/
        else break;
    }
    fclose(current_file); /*close file*/
    return;
}


/* write the external table to the ext file 
inputs - external_ptr -> pointer to external table, file_ext -> string of ext file name */
void print_external (external* external_ptr, char* file_ext){
    FILE *current_file = NULL; /*create file pointer */
    if (!(current_file = fopen (file_ext,"wt"))) {  /*open at write mode the ob file*/
        fprintf (stderr, ERROR_WRITE_FILE);
        exit (0); 
    }
    while (external_ptr!=NULL) { /* loop until last node */
        fprintf (current_file,"%s %07d\n" ,external_ptr->name, external_ptr->value ); /*print to file external label name and code addres*/
        if (external_ptr->next !=NULL) external_ptr = external_ptr->next; /*move to next node*/
        else break;
    }

    fclose(current_file);
    return;
}

/* write the entry table to the ent file 
inputs - symbol_ptr -> pointer to symbol table, file_ent -> string of ent file name */
void print_entry (symbol* symbol_ptr, char* file_ent){
    
    FILE *current_file = NULL;  /*create file pointer */

    if (!(current_file = fopen (file_ent,"wt"))){ /*open at write mode the ob file*/
        fprintf (stderr, ERROR_WRITE_FILE);
        exit (0); 
    }

    while (symbol_ptr!=NULL) { /* loop until last node of symbol table */
        if (symbol_ptr->type_e ==1 ){ /*check if current symbol has entry flag on */
            fprintf (current_file,"%s %07d\n" ,symbol_ptr->name, symbol_ptr->value ); /*print to file entry label name and addres*/
        }
        
        if (symbol_ptr->next !=NULL) symbol_ptr = symbol_ptr->next; /*move to next node*/
        else break;
    }

    fclose(current_file); /*close file */
    return;
}