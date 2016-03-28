#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_KEY 	255
#define MAX_VALUE 	1024
#define MAX_ROWS 	1024*1024

struct Row {
	char key[MAX_KEY];
	int id;
	char value[MAX_VALUE];
} Row;

struct Database {
	struct Row rows[MAX_ROWS];
} Database;

struct Connection {
	FILE *file;
	struct Database *db;
} Connection;

void clean(struct Connection * c) {
	if (c->db != NULL) {
		free(c->db);
	}
	
	free(c);
}

//!buffer overflow
int addToDatabase(struct Database *db, char * key, char * val) {
	int i, lastid =1;
	for (i=0; i< (sizeof(db->rows)/sizeof(struct Row)); i++) {
		
		struct Row *r = &db->rows[i]; 
		
		if (r->id != 0) {
			lastid = r->id;
		} else {
			r->id = (lastid+1); 
			strcpy(r->key, key);
			strcpy(r->value, val);
			
			return r->id;
		}
	}
	return 0;
}

void readFromDatabase(struct Database *db, int id) {
	int i;
	for (i=0; i< (sizeof(db->rows)/sizeof(struct Row)); i++) {
		
		struct Row * r = &db->rows[i]; 
		
		if (r != 0 && r->id == id) {
			printf("ID: %d\nKey: %s\n Val: %s\n", r->id, r->key, r->value);
			return ;
		} 
	}
	
	printf ("Unable to find id %d \n", id);
}




void die(char * msg) {
	fprintf(stderr, "%s", msg);
	exit(1);
}

int main() {

	struct Connection *con = malloc(sizeof(struct Connection));

	if (!con) {
		die("Unable to alocate connection");
	}
	
	con->db = (struct Database *) malloc(sizeof(struct Database));
	
	if (!con->db) {
		die("unable to allocate memory for database...");
	}
	
	int cnt = 0;
	
	
	printf("Commandlist \n\ta - add new record\n\tg - get record \n\te - exit\n");
	
	
	while(1) {
		
		printf("Your command: ");
		
		int c = getc(stdin), 
			exitfromloop = 0;
		
		switch (c) {
			case 'a' :
				printf("Enter key: ");
				char key[MAX_KEY];
				
				scanf ("%s", &key[0]);
				printf("Enter value: "); 
				char val[MAX_VALUE];
				scanf ("%s", &val[0]);
				
				//printf("adding %s:%s\n", key, val);
				int id = addToDatabase(con->db, key, val); 
				if (id > 0) {
					printf("Added to db, id: %d\n", id);
				} else {
					printf("Unable to add :( \n");
				}

				break;
			case 'g': 
				 
				printf("Enter id: ");
				int readID;
				scanf("%d", &readID); 
				printf ("\n"); 
				readFromDatabase(con->db, readID);
				fseek(stdin, 0, SEEK_END);
				break;
				
			case 'e':
				exitfromloop =1;
				break;
			default:
				printf("Unknown command %c", c);
		}
			
		if (exitfromloop) {
			break;
		}
		
	}
	
	clean(con);
	
	return 0;
}




