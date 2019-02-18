#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include "acsilib.h"
#include "taglib.h"

/* taglib.c contains all the functions that deal with searching throught the file and doing text manipulation*/

void txtfind(FILE *fip, FILE *fop){
	char *tempstr = malloc(301*sizeof(char));
	char *tagstr = malloc(22*sizeof(char));
	
	if(savetxt(fip, tempstr) <= 1){ // if the section between two html tags contains only spaces and tabs, it ignore the the section and moves on 
		fputc('>',fop);
		fprintf(fop, "%s", tempstr);
		free(tempstr);
		free(tagstr);
		return;
	}
	strcpy(tagstr, tagname(tempstr)); //creates a i18n-tagname based on the il8n-field
	if (strlen(tagstr) >= 4)	
		placetag(fop, tagstr); //copies the tagname & of i18n-field in the output file
	placestr(fop, tempstr);
	free(tempstr);
	free(tagstr);

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

void placetag(FILE *fop, char *tag){//places the tag in the proper html section, just before where the segment is located
	fprintf(fop, " data-i18n=\"%s\" >", tag);
	return;
}

void placestr(FILE *fop, char *tempstr){//places the segment analyzed for
	fprintf(fop, "%s", tempstr);
	return;
}

void scriptsearch(FILE *fip, FILE *fop){// searches for sections with '<script *>'
	char a,b,c;
	if((a=fgetc(fip))=='s'){
		if((b=fgetc(fip))=='r'){
			if((c=fgetc(fip))=='i'){
				fputc(a,fop);
				fputc(b,fop);
				fputc(c,fop);
				skipscript(fip,fop);
			}
			else{
				fputc(a,fop);
				fputc(b,fop);
				fputc(c,fop);
			}
		}
		else{
			fputc(a,fop);
			fputc(b,fop);
		}			
	}
	else{
		fputc(a,fop);
	}
	return;
}

void skipscript(FILE *fip, FILE *fop){//skips character until if finds '</script>' that ends  sheetworker sections
	char a,b,c,d,e,f,g,h;
	while((a=fgetc(fip))!=EOF){
		if(a=='<'){
			if((b=fgetc(fip)) =='/'){
				if((c=fgetc(fip)) =='s'){
					if((d=fgetc(fip)) =='r'){
						if((e=fgetc(fip)) =='i'){
							if((f=fgetc(fip)) =='p'){
								if((g=fgetc(fip)) =='t'){
									if((h=fgetc(fip)) =='>'){
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		fputc(a, fop);
	}
	fprintf(fop,"</script>\n");
	return;
}

/*
void savelist(Taglist *list, int *listsize, char *tag, char *tempstr){ //saves the tagname and the data
	
	strcpy(list[&listsize]->tag, tag);
	strpcy(list[&listsize]->data, tempstr);
	&listsize++;
	
	return;
}*/
