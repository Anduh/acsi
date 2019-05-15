// taglib header for ACSI

/* taglib.c contains all the functions that deal with searching throught the file and doing text manipulation*/

typedef struct translationtaglist {
//saves the tagname & text it's associated with in a 2-column list that can later be used to check for tagname duplicates, generate translation.json file or other useful things
	char tag[42];
	char data[301];
	struct translationtaglist *next;
} taglist;

void txtfind(FILE *fip, FILE *fop, FILE *fjson,int usemode);
int savetxt(FILE *fip, char *tempstr, char *contentstr);
char *tagname(char *txtstr);
void placetag(FILE *fop, char *tag);
void placestr(FILE *fop, char *tempstr);
int tagcheck(FILE *fjson, char *tagname);
void scriptsearch(FILE *fip, FILE *fop,int usemode);
void skipscript(FILE *fip, FILE *fop,int usemode);
