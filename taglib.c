#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include "acsilib.h"
#include "taglib.h"

/* taglib.c contains all the functions that deal with searching throught the file and doing text manipulation*/

void txtfind(char a, FILE *fip, FILE *fop){
	if( a == '>'){
		char *tempstr = malloc(301*sizeof(char));
		char *tagstr = malloc(22*sizeof(char));
		
		if(savetxt(fip, tempstr) <= 1){ // if the section between two html tags contains only spaces and tabs, it ignore the the section and moves on 
			fputc(a,fop);
			fprintf(fop, "%s", tempstr);
			free(tempstr);
			free(tagstr);
			return;
		}
		strcpy(tagstr, tagname(tempstr)); //creates a i18n-tagname based on the il8n-field
		placetag(fop, tagstr, tempstr); //copies the tagname & content of i18n-field in the output file
		free(tempstr);
		free(tagstr);
	}else{
		fputc(a,fop);
	}
	return;
}

int savetxt(FILE *fip, char *tempstr){//copies the content of the il8n-field
	char c;
	int i;
	int aflag = 0;	
	for(i = 0;i <= 300;i++){
		c = fgetc(fip);
		if (c == EOF){
			break;
		}
		tempstr[i] = c;
		if(c == '<'){
			i++;
			break;
		}
		if(isalpha(c)){
			aflag++;
		}
	}
	
	tempstr[i] = '\0';
	if (aflag >= 2){
		//printf("Visible txt: ---> %s  <-----\n",tempstr);
	}
	return aflag;
}

char *tagname(char *txtstr){
	//creates the 'i18n' tagname based on what the field contains, stripping numericals, special characters and spaces, reducing it to a reasonable length and making it lowercase
	int x;
	int caps = 0;
	int alpha = 0;
	char *tagstr = malloc(22*sizeof(char));
	for( x= 0;x < strlen(txtstr);x++){
		if(x == 21){
			break;
 		}
		if(isalpha(txtstr[x])){
			alpha++;
			if (isupper(txtstr[x])){
				caps++;
			}	
			tagstr[x] = tolower(txtstr[x]);

		}
		else if(txtstr[x] == ' '){
			tagstr[x] = '-';
		}
	}
	if(alpha != 0 && caps != 0 && alpha == caps){//if the content was completly uppercase, the tagname gets a '-u' suffix to distinguish it from sections that might contain identical content but have different case
		tagstr[x-2] = '-';
		tagstr[x-1] = 'u';
		tagstr[x] = '\0';	
	}
	else{
		tagstr[x] = '\0';
	}
	//printf("TAGNAME: %s  <----------\n",tagstr);
	return tagstr;
}

void placetag(FILE *fop, char *tag, char *tempstr){//places the tag in the proper html section, just before where the segment is located
	fprintf(fop, " data-i18n=\"%s\" >%s", tag, tempstr);
	return;
}

/*
void savelist(Taglist *list, int *listsize, char *tag, char *tempstr){ //saves the tagname and the data
	
	strcpy(list[&listsize]->tag, tag);
	strpcy(list[&listsize]->data, tempstr);
	&listsize++;
	
	return;
}*/