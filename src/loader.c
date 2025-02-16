#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "loader.h"
#include "memory.h"

/*
char * getline(FILE * f)
{
    char * buf;
    int result = fscanf(f,"%as\n",&buf);
    if (result < 0) {
        if (NULL != buf) free(buf);
        return NULL;
    }
    return buf;
}
*/

/* Convenience function to check if a string starts with a given prefix */
bool prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


/* Load program to memory from provided file */
uint8_t read_file(FILE *fin)
{
   char *line = NULL;
   size_t linecap = 0;
   ssize_t linelen;
   enum Segment segment;
   
   while ((linelen = getline(&line, &linecap, fin)) > 0) {
     
     if (prefix(".text", line)) {
       segment = USER_TEXT;
     }
     else if (prefix(".ktext", line)) {
       segment = KERNEL_TEXT;
     }
     else if (prefix(".data", line)) {
       segment = USER_DATA;
     }
     else if (prefix(".stack", line)) {
       segment = USER_STACK;
     }
     else if (prefix(".kdata", line)) {
       segment = KERNEL_DATA;
     }
     
     /* Read line, parse and store value to memory */
     char str[20];
     if (sscanf(line, ":%s", str) == 1) {
       if (strlen(str) == 18) {
	 /* Extract address */
	 char address_str[7];
	 strncpy(address_str,(const char *)str,6);
	 address_str[6] = '\0';
	 uint32_t address = (uint32_t)strtol(address_str, NULL, 16);
	 address -= 0x040000;
	 
	 /* Extract value */
	 char value_str[9];
	 strncpy(value_str,(const char *)&str[8],8);
	 value_str[8] = '\0';
	 uint32_t value = (uint32_t)strtol(value_str, NULL, 16);	   
	 
	 /* Modify address according to segment start address */
	 switch (segment) {
	 case USER_TEXT: {
	   address = USER_TEXT_BASE | (address & USER_TEXT_MASK);
	   break;
	 }
	 case KERNEL_TEXT: break;
	 case USER_DATA: {
	   address = USER_DATA_BASE | (address & USER_DATA_MASK);
	   break;
	 }
	 case USER_STACK: break;
	 case KERNEL_DATA: break;
	 default:
	   fprintf(stderr,"Unrecognised segment ID\n");
	   break;
	 } // switch

	 /* Write value to right address in memory */
	 write_word(address, value);
       }
     }
     
   } // while
   
   return 1;
}
