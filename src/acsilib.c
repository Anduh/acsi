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

void readcode(char *filename, char *arguments){
	FILE *fip, *fop, *fjson;

	int usemode = 0;// usermode 0 is the default, when ACSI creates a translated html file & translation.json
	fip = fopen(filename, "r");
	if(fip == NULL){
		errhandler(errno);
	}
	
	char *html = ".html";
	if (strstr(filename, html) == NULL){
		fprintf(stderr,"Not a html file. ACSI Usage: ./acsi <filename>\n");
		exit(-1);
	}

    //if no secondary argument is given, acsi defaults to only generate the translated html and doesn't send testoutput to console
	if (strchr(arguments, 't') != NULL){
		//test output to console
		usemode = 1;	
	}
	else if (strchr(arguments, 'a') != NULL){
		//test output to console and create translation.json file
		usemode = 2;	
	}
	else if (strchr(arguments, 'j') != NULL){
		//neither creates .json only
		usemode = 3;
	}
	char *outputfile, *jsonfile;
	outputfile = malloc((strlen(filename)+7)*sizeof(char));
    jsonfile = malloc((strlen(filename)+1)*sizeof(char));

	// creates the filename of the translated html file
    strncpy(outputfile, filename, (strlen(filename)-5));
    strcat(outputfile, "-i18n.html");

    //create translation file
    strncpy(jsonfile, filename, (strlen(filename)-5));
    strcat(jsonfile, ".json");
	
	fop = fopen(outputfile, "w+");
	//If the outputfile couldnt be opened, closes inputfile and frees outputname
	if(fop == NULL){
		fclose(fip);
		free(outputfile);
		errhandler(errno);
	}
	
    fjson = fopen(jsonfile, "w+");
	//If the translation file couldnt be opened, closes inputfile & outputfile and frees outputname &translation filename
	if(fjson == NULL){
		fclose(fip);
        fclose(fop);
        free(outputfile);
		free(fjson);
		errhandler(errno);
	}
    fprintf(fjson,"{\n");

	char c;
	//Main section of the program. searches through the inputfile until it finds a relevant character that results in jumping to work with the 'taglib.c' functions.
	while((c=fgetc(fip))!=EOF){
		
		if (c == '>'){
			txtfind(fip,fop,fjson,usemode);
		}
		else
			fputc(c,fop);
	}
    fprintf(fjson,"}");

    if (usemode == 0 || usemode == 1){
	    printf("%screated.\n",outputfile);
	}
    else if (usemode == 2 || usemode == 3){
	    printf("%s and %s created.\n",outputfile,jsonfile);
	}
 
    addcredits(fop);

	fclose(fip);
	fclose(fop);
	free(outputfile);
    if (usemode == 2 || usemode == 3){
        fclose(fjson);
        free(jsonfile);
    }
	return;
}

void addcredits(FILE *fop){
	fprintf(fop, "\n\n<!--\n------------- i18n-tags generated with ACSI V0.50 (Automated Character Sheet Internationalizer)-------------\n\tACSI author: https://github.com/Anduh\n\tACSI Sourcecode: https://github.com/Anduh/acsi\n-->");
	return;
}
