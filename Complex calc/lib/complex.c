#include "../include/complex.h"

void scan(char *format, void *out) {
	const int sizeBuf = 10;
	char *buf;
	char ch;
	int count = 0;
	int flagDot = 0;
	
	buf = malloc(sizeof(char) * sizeBuf);
	memset(buf, '\0', sizeBuf);
	
	fflush(NULL);
	
	if (strcmp("%d", format) == 0 || strcmp("%f", format) == 0) {
		if (system("stty -icanon") < 0) {
			perror("stty -icanon: ");
			exit(4);
		}
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
		if (system("stty cooked") < 0) {
			perror("stty cooked: ");
			exit(5);
		}
		
		if (strcmp("%d", format) == 0) {
			*(int*) out = atoi(buf);
		} else if (strcmp("%f", format) == 0) {
			*(double*) out = atof(buf);
		}
	} else if (strcmp("%s", format) == 0) {
		char *pos;
		
		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			perror("fgets error: ");
			exit(5);
		}
		if ((pos = strchr(buf, '\n')) != NULL)
			*pos = '\0';
		memcpy(out, buf, strlen(buf));
	} else if (strcmp("%c", format) == 0) {
		ch = fgetc(stdin);
		memcpy(out, &ch, sizeof(ch));
	}
	free(buf);
}

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

plugin *initPlugins(plugin *plugins, struct dirent **names, int count) {
	int j = 0, indx = 0;
	
	*(void **) &plugins->funcPlug = malloc(sizeof(myComplex*) * count);
	
	while (indx < count) {
		if (strncmp(names[indx]->d_name, "lib", 3) == 0) {
			plugins[j].nameFunc = parseName(names[indx]->d_name);
			plugins[j].execFile = names[indx]->d_name;
			j++;
		}
		indx++;
	}
	
	return plugins;
}

int filter(const struct dirent *entry) {
	return strstr(entry->d_name, ".so") == NULL ? 0 : 1; 
}

char *parseName(char *name) {
	const int begin = 3;
	int len = 0;
	char *temp = NULL, *result;
	
	temp = strchr(name, '.');
	len = temp - name;
	result = strndup(name + begin, len - 3);
	return result;
}

void printComplex(myComplex complx) {
	printf("Rb = %.4f Im = %.4f\n", complx.Rb, complx.Im);
}

void inputComplex(myComplex *complx) {
	printf("Rb:\n> ");
	scan("%f", &complx->Rb);
	
	printf("Im:\n> ");
	scan("%f", &complx->Im);
}
