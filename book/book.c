#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int countAbonents = 2, nameLen = 10, numberLen = 3;

struct Abonent {
	char *number;
	char *name;
};

char **split(char *str, char *delim) {
	char **res = (char**) malloc(sizeof(char*) * 2);
	
	res[0] = strtok(str, delim);
	for (int i = 1; i < 2; i++) {
		res[i] = strtok(NULL, delim);
		if (res[i] == NULL)
			break;
	}
	return res;
}

void writeToFile(FILE *file, struct Abonent *abonents) {
	int i;
	char *writeString;
	
	for (i = 0; i < countAbonents; i++) {
		writeString = (char*) malloc(sizeof(char) * 
			(strlen(abonents[i].name) + strlen(abonents[i].number) + 5));
		sprintf(writeString, "%d;%s %s\n", 
			(int)(strlen(abonents[i].name) + strlen(abonents[i].number) + 1),
			abonents[i].name, abonents[i].number);
			
		fwrite(writeString, sizeof(char), strlen(writeString), file);
		free(writeString);
	}
} 

void readFromFile(FILE *file, int reverse, struct Abonent *abonents) {
	fseek(file, 0, SEEK_SET);
	
	int lenString, lenStrings[countAbonents], i, j = 0;//??????????????????????????????????????????????????????????
	char ch, readString[nameLen + numberLen + 1];
		
	if (!reverse) {
		for (i = 0; i < countAbonents; i++) {
			while(fread(&ch, sizeof(char), 1, file) > 0) {
				if (ch == ';') {
					lenString = atoi(readString);
					fread(readString, sizeof(char), lenString, file);
					//printf("%s\n", readString);
					
					char **spl = split(readString, " ");
					
					strcpy(abonents[i].name, spl[0]);
					strcpy(abonents[i].number, spl[1]);
										
					memset(readString, '\0', (nameLen + numberLen + 1));
					fseek(file, 1, SEEK_CUR);
					break;
				} else {
					sprintf(readString, "%s%c", readString, ch);
				}
			}	
		}
	} else {
		int rank, last;
		float temp;
		while(fread(&ch, sizeof(char), 1, file) > 0) {
			if (ch == ';') {
				lenStrings[j] = atoi(readString);
				memset(readString, '\0', sizeof(char) * (nameLen + numberLen + 1));
				fseek(file, lenStrings[j] + 1, SEEK_CUR);
				j++;
				if (j == countAbonents)
					break;
			} else {
				sprintf(readString, "%s%c", readString, ch);
			}
		}
		
		fseek(file, -1, SEEK_END);
		for (i = countAbonents - 1, last = i; i >= 0; i--) {
			for (j = last; j > i; j--) {
				if (i < countAbonents - 1) {
					rank = 0;
					temp = lenStrings[j];
					while (temp > 1) {
						temp /= 10;
						rank++;
					}
					fseek(file, -(lenStrings[j] + rank + 2), SEEK_CUR);
					last = i;
				} else {
					break;	
				}
			}
			fseek(file, -lenStrings[i], SEEK_CUR);
			fread(readString, sizeof(char), lenStrings[i], file);
			//printf("%s\n", readString);
			char **spl = split(readString, " ");					
			
			strcpy(abonents[i].name, spl[0]);
			strcpy(abonents[i].number, spl[1]);
			
			memset(readString, '\0', (nameLen + numberLen + 1));
		}
	}
}

int main() {
	int i, j;
	FILE *file;
	char ch;
	struct Abonent abonents[countAbonents];
	
	file = fopen("book.txt", "wb+");
	
	for (i = 0; i < countAbonents; i++) {
		abonents[i].number = (char*) malloc(sizeof(char) * numberLen);
		abonents[i].name = (char*) malloc(sizeof(char) * nameLen);
	}
	
	for (i = 0; i < countAbonents; i++) {
		printf("Input name #%d abonent: ", i + 1);
		scanf("%s", abonents[i].name);
		
		printf("Input number #%d abonent: ", i + 1);

		for (j = 0; j < numberLen; j++) {
			while (1) {
				system("stty -icanon");
				ch = getchar();
				if ((int)ch > 47 && (int)ch < 58) {
					abonents[i].number[j] = ch;
					break;
				} else {
					//printf("\nWrong character, try again\n");
					printf("%s", abonents[i].number);
				}
			}
			system ("stty cooked");
		}
		printf("\n");
	}
	
	writeToFile(file, abonents);
	for (i = 0; i < countAbonents; i++) {
		memset(abonents[i].number, '\0', numberLen);
		memset(abonents[i].name, '\0', nameLen);
	}
	
	readFromFile(file, 0, abonents);
	for (i = 0; i < countAbonents; i++) {
		printf("Name #%d abonent: %s\n", i + 1, abonents[i].name);
		printf("Number #%d abonent: %s\n", i + 1, abonents[i].number);
	}
	
	for (i = 0; i < countAbonents; i++) {
		free(abonents[i].number);
		free(abonents[i].name);
	}
	fclose(file);
	
	return 0;
}
