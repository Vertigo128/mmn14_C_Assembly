#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"


/* Adds an element to a linked list pointer, and if it doesn't exist (ie. NULL), creates it and assigned to pointer 
inputs - linked_list-> pointer to list struct, data -> pointer of data to add*/
void list_add_element (list** linked_list, void* data) {
	if (data == NULL) { /*check if data exist, exit if not*/
		return;
	}

	if (*linked_list == NULL) { /*check if linked list alread exist */
		*linked_list = calloc(1, sizeof(list));
		
		if (*linked_list == NULL) { /* Check if calloc failed */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}
		(*linked_list)->value = data; /* Put new value in the new list node */
	} else {
		list* ptr = *linked_list;

		
		while (ptr->next != NULL) { /* Skip to last element */
			ptr = ptr->next;
		}

		ptr->next = calloc(1, sizeof(list)); /* Create new last node */

		
		if (ptr->next == NULL) { /* Check if calloc failed */
			fprintf(stderr, ERROR_OUT_OF_MEMORY);
			exit(1);
		}

		ptr->next->value = data; /* Put new values in last list node */
	}
}


/* Duplicates a long integer to allocated memory 
inputs - num-> number to copy
return the new pointer */
long int* int_dup (long int* num) {
	
    long int* num_cpy; /*temp variable to store the data*/

	if (!num) /*check if data exist, exit if not*/
		return NULL;

	num_cpy = malloc(sizeof((long int) num)); /* Create new pointer  */
	if (!num_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}
    *num_cpy = (long int) *num; /* Copy values to new pointer */

	return num_cpy;
}


/* Duplicates a string to allocated memory 
inputs - str-> char to copy
return the new pointer */
char* charecter_dup (char str) {
	char* str_cpy; /*temp variable to store the data*/

	str_cpy = malloc(sizeof(char)); /* Create new pointer */
	if (!str_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}

	*str_cpy =  str; /* Copy values to new pointer */

	return str_cpy;
}


/* Duplicates a string to allocated memory 
inputs - str-> string to cpy
return the new pointer */
char* str_dup (char* str) {
	char* str_cpy; /*temp variable to store the data*/

	if (!str)
		return NULL;

	str_cpy = malloc(strlen(str) + 1); /* Create new pointer */
	if (!str_cpy) {
		fprintf(stderr, ERROR_OUT_OF_MEMORY);
		exit(1);
	}

	strcpy(str_cpy, str); /* Copy values to new pointer */
	return str_cpy;
}


/*skip pointer to the next non-space, return the next non-space charecter
inputs - ptr-> pointer to string*/
char skip_space(char** ptr){
    while (isspace (**ptr)) (*ptr)++; /*keep skiping until non-space char is found*/
    return **ptr; /*return the non-space charecter*/
}


/*skip pointer to the next whitespace, if EOL found return 0
inputs - ptr-> pointer to string*/
int expect_non_EOL_WS (char** ptr){

    char c = skip_space(ptr); /*get the next non-space charercter*/
    if (c=='\0' || c=='\n') return 0; /*return 0 if it is EOL*/
    return 1; /*return 1 if non-EOL*/
}

/*search for a charecter in the next word, stop search at space/EOL
inputs - c-> charecter to find, ptr -> string to search
return 1 if charecter found, 0 if not*/
int search_word(char c, char* ptr){
    while (!isspace (*ptr) && *ptr != '\0' && *ptr != '\n' && *ptr != c) (ptr)++; /*search until EOL or charecter found*/
    if (*ptr == c) return 1; /*return 1 if found*/
    else return 0; /*return 0 if not found*/
}

