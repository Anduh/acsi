#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
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
		fprintf(stderr,"ACSI Usage: ./acsi <filename>\n",argv[0]);
		exit(-1);
	}
	if ((childpid = fork()) == 0){
		char *temp = malloc(50*sizeof(char));
		strcpy(temp, argv[1]);
		readcode(temp);//main portion of the program
		free(temp);
		exit(0);
	}
	while ((parentpid = wait(&status)) > 0);
	printf("ASCI finished succesfully!\n", getpid());
	return 1;
}
