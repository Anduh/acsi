// taglib header for ACSI

/* taglib.c contains all the functions that deal with searching throught the file and doing text manipulation*/

typedef struct translationtaglist {
//saves the tagname & text it's associated with in a 2-column list that can later be used to check for tagname duplicates, generate translation.json file or other useful things
	char *tag;
	char *data;
	struct translationtaglist *next;
} Taglist;

void txtfind(FILE *fip, FILE *fop,int testmode);
int savetxt(FILE *fip, char *tempstr,int testmode);
char *tagname(char *txtstr,int testmode);
void placetag(FILE *fop, char *tag);
void placestr(FILE *fop, char *tempstr);
void scriptsearch(FILE *fip, FILE *fop,int testmode);
void skipscript(FILE *fip, FILE *fop,int testmode);
void savelist(Taglist *list, int *listsize, char *tag, char *tempstr);
