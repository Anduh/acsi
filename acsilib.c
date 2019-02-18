#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include "acsilib.h"
#include "taglib.h"

/* acsilib.c contains all the general functions that doesn't directly relate to the text manipulation syntax, such a opening closing files, managing memory and error handling*/

void handler(int signo){
	if (signo == SIGINT){
		printf("User exited ACSI(pid:%d) with Ctrl-C\n", getpid());
		exit(-1);
	}
	else if (signo == SIGSEGV){
		printf("Prosess %d SEG FAULT while running ACSI.\n", getpid());
		exit(-1);
	}
}

void errhandler(int errnum){
		if (errnum == ENOENT){
			printf("ERROR: File doesn't exist. Perhaps the achives are incomplete?\n");
			exit(-1);
		}
		else{
			perror("ERROR");
			exit(-1);
		}
}

void readcode(char* string){
	FILE *fip, *fop;
	fip = fopen(string, "r");
	if(fip == NULL){
		errhandler(errno);
	}

	/*
	char *html = ".html";
	if (strstr(string, html) == NULL){
		fprintf(stderr,"Not a html file. ACSI Usage: ./acsi <filename>\n");
		exit(-1);
	}*/
	
	char *outputfile;
	outputfile = malloc((strlen(string)+7)*sizeof(char));

	// creates the filename of the output file
	strcpy(outputfile, "i18n-");
	strcat(outputfile, string);
	
	fop = fopen(outputfile, "w+");
	//If the outputfile couldnt be opened, closes inputfile and frees outputname
	if(fop == NULL){
		fclose(fip);
		free(outputfile);
		errhandler(errno);
	}
	
	char c;
	//Main section of the program. searches through the inputfile until it finds a relevant character that results in jumping to work with the 'taglib.c' functions.
	while((c=fgetc(fip))!=EOF){
		if (c == 's' || c == 'r'){
			fputc(c,fop);
			scriptsearch(fip,fop);
		}		
		else if (c == '>'){
			txtfind(fip,fop);
		}
		else
			fputc(c,fop);
	}
	addcredits(fop);
	fclose(fip);
	fclose(fop);
	free(outputfile);
	return;
}

void addcredits(FILE *fop){
	fprintf(fop, "\n\n<!--\n------------- i18n-tags generated with ACSI V0.2 (Automated Character Sheet Internationalizer)-------------\n\tACSI author: github.com/Anduh\n\tSource: github.com/Anduh/acsi\n-->");
	return;
}
