#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include "acsilib.h"
#include "taglib.h"

/* taglib.c contains all the functions that deal with searching throught the file and doing text manipulation*/

void txtfind(FILE *fip, FILE *fop, FILE *fjson,int usemode){
	char *tempstr = malloc(301*sizeof(char));
    char *contentstr = malloc(301*sizeof(char));
	char *tagstr = malloc(42*sizeof(char));
	
	if(savetxt(fip, tempstr, contentstr) <= 2){ // if the section between two html tags contains only spaces and tabs, it ignore the the section and moves on 
		fputc('>',fop);
		fprintf(fop, "%s", tempstr);
		free(tempstr);
		free(tagstr);
        free(contentstr);
		return;
	}
	strcpy(tagstr, tagname(tempstr)); //creates a i18n-tagname based on the il8n-field
	if (strlen(tagstr) >= 3){	
		placetag(fop, tagstr); //copies the tagname & of i18n-field in the output file
        if (usemode == 1){//prints to console
            printf("    \"%s\":",tagstr);
            printf("\"%s\"\n\n",contentstr);
        }
        if (usemode == 2){//prints to json file and console
            if(tagcheck(fjson,tagstr) == 0){
                printf("    \"%s\":",tagstr);
                printf("\"%s\"\n\n",contentstr);
                
                fprintf(fjson, "    \"%s\":",tagstr);
                fprintf(fjson,"\"%s\",\n",contentstr);
            }
        }
        if (usemode == 3){//prints only to json file
            if (tagcheck(fjson,tagstr) == 0){
                fprintf(fjson, "    \"%s\":",tagstr);
                fprintf(fjson,"\"%s\",\n",contentstr);
            }
        }
    }
	placestr(fop, tempstr);
	free(tempstr);
	free(tagstr);

	return;
}

int savetxt(FILE *fip, char *tempstr,char *contentstr){//copies the content of the il8n-field
	char c;
	int i;
    int j = 0;
	int aflag = 0;	
	for(i = 0;i <= 300;i++){
		c = fgetc(fip);
		if (c == EOF){
			break;
		}
        if (c == '\n' || c == '\t' || c == '\v' || c == '\f'  || c == '\r'  || c == '\b'){
            tempstr[i] = c;
        }
        if (aflag == 0 && c == ' '){
            tempstr[i] = c;
        }
        else{
            tempstr[i] = c;
            	if(c == '<'){
			        i++;
			        break;
		        }
            strncat(contentstr, &c, 1);
            j++;
        }
		if(isalpha(c)){
			aflag++;
		}
	}
	tempstr[i] = '\0';

	return aflag;
}

char *tagname(char *txtstr){
	//creates the 'i18n' tagname based on what the field contains, stripping numericals, special characters and spaces, reducing it to a reasonable length and making it lowercase
	int x;
	int caps = 0;
	int alpha = 0;
	char *tagstr = malloc(42*sizeof(char));
	for( x= 0;x < strlen(txtstr);x++){
		if(x == 41){
			break;
 		}
		if(isalpha(txtstr[x])){
			alpha++;
			if (isupper(txtstr[x])){
				caps++;
			}	
			tagstr[x] = tolower(txtstr[x]);
		}
        else if(isdigit(txtstr[x])){
            tagstr[x] = txtstr[x];
        }
        else if(txtstr[x] == ' ' && alpha > 0){
            tagstr[x] = '-';
        }
        else if(txtstr[x] == '\n' || txtstr[x] == '\t'){
            tagstr[x] = '-';
        }
        else if(ispunct(txtstr[x])){
            tagstr[x] = '-';
		}
	}

	if(alpha != 0 && caps != 0 && alpha == caps){//if the content was completly uppercase, the tagname gets a '-u' suffix to distinguish it from sections that might contain identical content but have different case
		tagstr[x-2] = '-';
		tagstr[x-1] = 'u';
		tagstr[x] = '\0';	
	}
    if(tagstr[x-1] == '-'){
        tagstr[x-1] = '\0';
    }
	else{
		tagstr[x] = '\0';
	}
	return tagstr;
}

void placetag(FILE *fop, char *tag){//places the tag in the proper html section, just before where the segment is located
	fprintf(fop, " data-i18n=\"%s\" >", tag);
	return;
}

void placestr(FILE *fop, char *tempstr){//places the segment analyzed for
	fprintf(fop, "%s", tempstr);
	return;
}

int tagcheck(FILE *fjson, char *tagname){
    char str[400];
    rewind(fjson);
    
    strcpy(str, "\"");
    strcat(str, tagname);
    strcat(str, "\"");    

    while(1){
        fgets(str, 399, fjson);
        if (feof(fjson)){
            break;
        }
        if (strstr(str, tagname) != NULL){
            return 1;
        }

    }
    return 0;
}
