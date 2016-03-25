#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
#include <string.h>

char * getLine(limit) {
	int c, cur = 0;
	char line[limit];
	
	while (((c = getc(stdin)) != '\n')) {
		if ((cur < limit-1)) {
			line[cur] = c;
			cur++;
		}
	}
	
	line[cur+1] = '\0'; 
	char * str = ((char *) malloc( sizeof(line) ));
	strcpy(str, line);
	return str;
}

void trimString(char * str, char remove) {
	int index=0, 
		i=0, 
		length = (sizeof(str) / sizeof(char));
	
	int indexfront = 0, 
		indexback = (length - 1);

	if (str[indexfront] == remove || str[indexback] == remove) {
		//todo
	} 
}

void startTerminal() {
	while (1) {
		printf ("%% "); 
		
		char *f = getLine(20);
		
		trimString(f ,' ');
		
		if (strcmp(f, "help") == 0) {
			printf("For help ask google\n");
		}
		
		printf ("%% Entered \"%s\"\n", f);
		free(f);
	}
}

int main () {
	
	startTerminal();
	
	return 0;
}





