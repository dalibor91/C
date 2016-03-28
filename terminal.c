#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
#include <string.h>

char * getLine(limit) {
	int c=0, cur = 0;
	char line[limit];
	
	//Initilize whole char arr to \0
	int i ;
	for (i =0 ; i < limit; i++) {
		line[i] =0;
	}
	
	while (((c = getc(stdin)) != '\n')) {
		if ((cur < limit-1)) {
			line[cur] = c;
			cur++;
		}
	}
	
	fflush(stdin);

	return strdup(line);
}

char * trimString(char * str, char remove) {
	int i=0, 
		length = strlen(str);
	
	int indexfront = 0, 
		indexback = (length - 1),
		removed = 0;
	
	while ((str[indexfront] == remove || str[indexback] == remove)) {
		removed = 1;
		if (str[indexfront] == remove) {
			indexfront++;
		}
		
		if (str[indexback] == remove) {
			indexback--;
		}
		//whole string needs to be trimmed, return ""
		if (indexback < indexfront) {
			free(str);
			return strdup("");
		}
	} 
	
	if (removed == 1) {		
		char * copy = (char *)malloc((1+indexback-indexfront) * sizeof(char));

		for (i = indexfront; i<=indexback; i++) {			
			copy[i-indexfront] = str[i];
		}
		//mark end 
		copy[indexback+1] = '\0';
		//release original str
		free(str);
		return copy;
	}
	//return original str
	return str;
}

void startTerminal() {
	while (1) {
		printf ("%% "); 
		
		char *f = getLine(20);
		
		char *cmd = trimString(f ,' ');
		
		if (strcmp(cmd, "") == 0) {
			free(cmd);
			continue;
		}
		
		if (strcmp(cmd, "help") == 0) {
			printf("For help ask google\n");
		}
		
		printf ("%% Entered \"%s\"\n", cmd);
		free(cmd);
	}
}

int main () {
	
	startTerminal();
	
	return 0;
}
