/* ACSIlib header file */

/* acsilib.c contains all the general functions that doesn't directly relate to the text manipulation syntax, such a opening closing files, managing memory and error handling*/

void handler(int signo);
void errhandler(int errnum);
void readcode(char *filename,char *arguments);
void addcredits(FILE *fop);


