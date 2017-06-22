#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <dirent.h>

#include "../include/complex.h"

void scan(char *format, void *out) {
	const int sizeBuf = 10;
	char buf[sizeBuf];
	char ch;
	int count = 0;
	int flagDot = 0;
	
	memset(buf, '\0', sizeBuf);
	
	fflush(NULL);
	
	if (strcmp("%d", format) == 0 || strcmp("%f", format) == 0) {
		system("stty -icanon");
		while (1) {
			ch = fgetc(stdin);
			
			if ((int) ch == 45 && buf[0] == '\0')
				sprintf(buf, "%s%c", buf, ch);
			
			if (strcmp("%f", format) == 0 && (int) ch == 46 && flagDot == 0) {
				flagDot = 1;
				sprintf(buf, "%s%c", buf, ch);
			}
			
			if ((int) ch > 47 && (int) ch < 58) {
				sprintf(buf, "%s%c", buf, ch);
				count++;
			}
			
			if (count == sizeBuf - 1 || (int) ch == 10) {
				break;
			}	
		}
		system("stty cooked");
		
		
		if (strcmp("%d", format) == 0) {
			*(int*) out = atoi(buf);
		} else if (strcmp("%f", format) == 0) {
			*(double*) out = atof(buf);
		}
	} else if (strcmp("%s", format) == 0) {
		char *pos;
		
		fgets(buf, sizeof(buf), stdin);
		if ((pos = strchr(buf, '\n')) != NULL)
			*pos = '\0';
		memcpy(out, buf, sizeof(buf));
	} else if (strcmp("%c", format) == 0) {
		ch = fgetc(stdin);
		memcpy(out, &ch, sizeof(ch));
	}
}
/*
char **split(char *str, char *delim) {
	int i;
	char **res;
	res = malloc(sizeof(char*) * 2);
	
	res[0] = strtok(str, delim);
	for (i = 1; i < 2; i++) {
		res[i] = strtok(NULL, delim);
		if (res[i] == NULL)
			break;
	}
	return res;
}
*/
int countPlugins() {
	int count = 0;
	DIR *dir;
	struct dirent *ent = NULL;
	char *path;
	
	path = getenv("LD_LIBRARY_PATH");
	if (path == NULL) {
		perror("LD_LIBRARY_PAT is not set: ");
		exit(3);
	}
	
	dir = opendir(path);
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strncmp(ent->d_name, "p_", 2) == 0) {
				count++;
			}
		}
		closedir(dir);
	} else {
		perror("Can not open directory: ");
		exit(2);
	}
	
	return count;
}

char **getNamesPlugins(int *count) {
	int i = 0, indx = 0;
	DIR *dir = NULL;
	struct dirent *ent = NULL;
	char *path = NULL, **names = NULL;
	
	if (*count == 0)
		*count = countPlugins();
	
	path = getenv("LD_LIBRARY_PATH");
	if (path == NULL) {
		perror("LD_LIBRARY_PAT is not set: ");
		exit(3);
	}
	
	names = calloc((*count), sizeof(char*));
	for (i = 0 ; i < *count; i++)
		names[i] = calloc(8, sizeof(char));
	
	i = 0;
	dir = opendir(path);
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strncmp(ent->d_name, "p_", 2) == 0) {
				sprintf(names[indx], "%s", parseName(ent->d_name));
				indx++;
			}
		}
		closedir(dir);
	} else {
		perror("Can not open directory: ");
		exit(2);
	}
	
	return names;
}

char *parseName(char *name) {
	const int begin = 2;
	int len = 0;
	char *temp = NULL;
	
	temp = strchr(name, '.');
	len = temp - name;
	
	return strndup(name + begin, len - 2);
}

void printComplex(myComplex complx) {
	printf("Rb = %.4f Im = %.4f\n", complx.Rb, complx.Im);
}

void inputComplex(myComplex *complx) {
	printf("Re:\n> ");
	scan("%f", &complx->Rb);
	
	printf("Im:\n> ");
	scan("%f", &complx->Im);
}
