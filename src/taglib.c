#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include "acsilib.h"
#include "taglib.h"

/* taglib.c contains all the functions that deal with searching throught the file and doing text manipulation*/

void txtfind(FILE *fip, FILE *fop,int testmode){
	char *tempstr = malloc(301*sizeof(char));
	char *tagstr = malloc(22*sizeof(char));
	
	if(savetxt(fip, tempstr, testmode) <= 2){ // if the section between two html tags contains only spaces and tabs, it ignore the the section and moves on 
		fputc('>',fop);
		fprintf(fop, "%s", tempstr);
		free(tempstr);
		free(tagstr);
		return;
	}
	strcpy(tagstr, tagname(tempstr, testmode)); //creates a i18n-tagname based on the il8n-field
	if (strlen(tagstr) >= 3)	
		placetag(fop, tagstr); //copies the tagname & of i18n-field in the output file
	placestr(fop, tempstr);
	free(tempstr);
	free(tagstr);

	return;
}

int savetxt(FILE *fip, char *tempstr,int testmode){//copies the content of the il8n-field
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
	if (aflag >= 2 && testmode == 1){
		printf("Visible txt: ---> %s  <-----\n",tempstr);
	}
	return aflag;
}

char *tagname(char *txtstr,int testmode){
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
        else if(isdigit(txtstr[x])){
            tagstr[x] = txtstr[x];
        }
        else if(txtstr[x] == ' '){
            tagstr[x] = '-';
        }
		else if(txtstr[x] == '.' || txtstr[x] == ',' || txtstr[x] == ':' || txtstr[x] == ';' || txtstr[x] == '!' || txtstr[x] == '\n' || txtstr[x] == '\t' || txtstr[x] == '!' || txtstr[x] == '?' || txtstr[x] == '-'){
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
	if (testmode == 1)
		printf("TAGNAME: %s  <----------\n",tagstr);
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

void scriptsearch(FILE *fip, FILE *fop,int testmode){// searches for sections with '<script *>'
	char a,b,c,d,e,f;
	if((a=fgetc(fip))=='c'){
		if((b=fgetc(fip))=='r'){
			if((c=fgetc(fip))=='i'){
				if((d=fgetc(fip))=='p'){
					if((e=fgetc(fip))=='t'){
						if((f=fgetc(fip))==' '){
							if (testmode == 1)
								printf("scriptsearch 3 started\n");
							fputc(a,fop);
							fputc(b,fop);
							fputc(c,fop);
							fputc(d,fop);
							fputc(e,fop);
							fputc(f,fop);
							if (testmode == 1)
								printf("scriptskip started\n");
							skipscript(fip,fop, testmode);
							return;
						}
						else {
							fputc(a,fop);
							fputc(b,fop);
							fputc(c,fop);
							fputc(d,fop);
							fputc(e,fop);
							fputc(f,fop);
						}
					}
					else{
						fputc(a,fop);
						fputc(b,fop);
						fputc(c,fop);
						fputc(d,fop);
						fputc(e,fop);
					}
				}
				else{
					fputc(a,fop);
					fputc(b,fop);
					fputc(c,fop);
					fputc(d,fop);
				}
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
	else if(a == 'o'){
		if((b=fgetc(fip))=='l'){
			if((c=fgetc(fip))=='l'){
				if((d=fgetc(fip))=='t'){
					fputc(a,fop);
					fputc(b,fop);
					fputc(c,fop);
					fputc(d,fop);
					if (testmode == 1)
						printf("rollskip started\n");
					skipscript(fip,fop, testmode);
					return;
				}
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

void skipscript(FILE *fip, FILE *fop,int testmode){//skips character until if finds '</script>' that ends  sheetworker sections
	char a,b,c,d,e,f,g,h;
	while((a=fgetc(fip))!=EOF){
		if(a=='<'){
			if((b=fgetc(fip)) =='/'){
				if((c=fgetc(fip)) =='s'){
					if((d=fgetc(fip)) =='c'){
						if (testmode == 1)
							printf("</script> found\n");
						if((e=fgetc(fip)) =='r'){
							if((f=fgetc(fip)) =='i'){
								if((g=fgetc(fip)) =='p'){
									if((h=fgetc(fip)) =='t'){
											fprintf(fop,"</script");
										if (testmode == 1)
											printf("script found\n");
										return;
									}
								}
							}
						}
					}
					else{
					fputc(a, fop);
					fputc(b, fop);
					fputc(c, fop);
					fputc(d, fop);
					}
				}else if(c== 'r'){
					if (testmode == 1)
						printf("</rolltemplate> found\n");
					fprintf(fop,"</rolltemplate>\n\n");
					fseek(fip, 14,SEEK_CUR);
					return;
				}else{
					fputc(a, fop);
					fputc(b, fop);
					fputc(c, fop);
				}
			}
			else{
				fputc(a, fop);
				fputc(b, fop);
			}
		}
		else
			fputc(a, fop);
	}
	return;
}

/*
void savelist(Taglist *list, int *listsize, char *tag, char *tempstr){ //saves the tagname and the data
	
	strcpy(list[&listsize]->tag, tag);
	strpcy(list[&listsize]->data, tempstr);
	&listsize++;
	
	return;
}*/
