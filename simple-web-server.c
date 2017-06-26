#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>

#define	MY_PORT	9999

void PERROR(char* msg);
#define PERROR(msg)	{ perror(msg); abort(); }
#define MAXBUF	1024
char buffer[MAXBUF];
char *Host="127.0.0.1:9999";

#define MAXPATH	150
#define MAXNAME	25
#define MAXFILESIZE 1024*1024*1024

void printUse(char * program) {
printf (" Use:\n ");
printf ("     %s <directory_to_serve>\n", program);
}


char * printFile(char *file) {
	FILE * f = fopen(file, "r");
	if (f == NULL) {
		return NULL;
	}
	char * content = (char *)malloc(MAXFILESIZE * sizeof(char));

	if (content == NULL) {
		printf("Ne moze da alocira");
		return NULL;
	}

	char * cnt = content; int chr=0; int pos=0;
	while ((chr = getc(f)) != EOF) {
		*content = chr;
		*content++;
		pos++;
		if (pos == (MAXFILESIZE-2)) {
			break;
		}
	}
	*content = '\0';

	fclose(f);

	return cnt;
}

void PrintFileHeaders(char * filename, FILE * f) {

	char extension[MAXPATH] = { 0 };
	int position = strlen(filename) -1;
	int curpos = 0;

	while (filename[position] != '.' && filename[position] != '/' && position >= 0) {
		extension[curpos] = filename[position];
		position--;
		curpos++;
	}

	extension[curpos] = 0;

	printf ("Serve %s\n", extension);

	fprintf(f, "HTTP/1.0 200 Found\nStatus: 200 OK\n");
	if (strcmp(extension, "lmth") == 0) fprintf(f, "Content-Type: text/html\n"); //html
	else if (strcmp(extension, "ssc") == 0) fprintf(f, "Content-Type: text/css\n"); //css
	else if (strcmp(extension, "sj") == 0) fprintf(f, "Content-Type: application/javascript\n"); //js
	//images
	else if (strcmp(extension, "fig") == 0) fprintf(f, "Content-Type: image/gif\n"); //gif
	else if (strcmp(extension, "gepj") == 0) fprintf(f, "Content-Type: image/jpeg\n"); //jpeg
	else if (strcmp(extension, "gpj") == 0) fprintf(f, "Content-Type: image/jpeg\n"); //jpg
	else if (strcmp(extension, "gnp") == 0) fprintf(f, "Content-Type: image/png\n"); //png
	else if (strcmp(extension, "gvs") == 0) fprintf(f, "Content-Type: image/svg+xml\n"); //svg

	fprintf(f, "\n");
}

void ServeStatic(FILE *f, char* Path, char* initPath) {
	setbuf(f, NULL);
	char * reqPath = Path;
	if (strcmp(Path, "/") == 0) {
		reqPath = "/index.html";
	}

	char * requestedFile = (char *)malloc((strlen(initPath) + strlen(reqPath)) * sizeof(char));
	strcpy(requestedFile, initPath);
	strcat(requestedFile, reqPath);
	char * message = NULL;

	if (access(requestedFile, F_OK ) != -1 ) {
		printf("Print file: %s\n", Path);
		PrintFileHeaders(requestedFile, f);
		message = printFile(requestedFile);
	} else {
		fprintf(f, "HTTP/1.0 404 Not Found\nStatus: 404 Not Found\n\n");
	}

	char * c = message;

	while ((message != NULL) && (*c != '\0')){
		fprintf(f, "%c", *c);
		*c++;
	}

	message != NULL && free(message);
}

int main(int argc, char **argv) {
	struct sockaddr_in addr;
	int sd, addrlen = sizeof(addr);

	if (argc != 2) {
		printUse(argv[0]);
		return 0;
	}

	if ( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
		PERROR("Socket");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(MY_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if ( bind(sd, (struct sockaddr*)&addr, addrlen) < 0 )
		PERROR("Bind");

	if ( listen(sd, 20) < 0 )
		PERROR("Listen");

	printf ("Server listening on port: %d\n", MY_PORT);

	char *rootDir = argv[1];
	int lenRootDir = strlen(rootDir)-1;
	while(rootDir[lenRootDir] == '/') {
		rootDir[lenRootDir] = '\0';
		lenRootDir--;
	}

	while (1) {
		int len;
		int client = accept(sd, (struct sockaddr*)&addr, &addrlen);
		printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		if ( (len = recv(client, buffer, MAXBUF, 0)) > 0 ) {
			FILE* ClientFP = fdopen(client, "w");
			if ( ClientFP == NULL ) perror("fpopen");
			else {
				char Req[MAXPATH] = { 0 };
				sscanf(buffer, "GET %s HTTP", Req);
				printf("Request: \"%s\"\n", Req);
				ServeStatic(ClientFP, Req, rootDir);
				fclose(ClientFP);
			}
		}
		close(client);
	}
	return 0;
}
