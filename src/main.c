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
	if (argc != 2){
		fprintf(stderr,"ACSI Usage: ./acsi <filename>\n");
		exit(-1);
	}
	char *temp = malloc(50*sizeof(char));
	strcpy(temp, argv[1]);
	if ((childpid = fork()) == 0){
		sigaction(SIGINT, &sa, NULL);// starts signal handler 
		strcpy(temp, argv[1]);
		readcode(temp);//main portion of the program
		exit(0);
	}
	while ((parentpid = wait(&status)) > 0);
	free(temp);
	return 1;
}
