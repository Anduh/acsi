#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include "acsilib.h"
#include "taglib.h" 

int main (int argc, char *argv[]){
	pid_t childpid, parentpid;
	int status = 0;
	struct sigaction sa;
	sa.sa_handler = handler;
	if (argc > 3 || argc == 1){
		fprintf(stderr,"ACSI Usage: ./acsi <filename>\n");
		exit(-1);
	}
	char *filename = malloc(50*sizeof(char));
	strcpy(filename, argv[1]);
	char *arguments = malloc(10*sizeof(char));
	strcpy(arguments, " ");
	if (argv[2] != NULL){
		//copies and saves secondary arguments from the user, like 't' for test or similar
		strcpy(arguments, argv[2]);
	}
	if ((childpid = fork()) == 0){
		sigaction(SIGINT, &sa, NULL);// starts signal handler 
		readcode(filename, arguments);//main portion of the program
		exit(0);
	}
	while ((parentpid = wait(&status)) > 0);
	free(filename);
	free(arguments);
	return 1;
}
