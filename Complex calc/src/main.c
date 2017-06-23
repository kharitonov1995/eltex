#include "../lib/complex.c"

char NAME_PLUGIN[] = "NAME_PLUGIN";

int main() {
	int count = 0, i, ch = 0;
	plugin *plugins = NULL;
	char *path = NULL, *fullPath = NULL;
	struct dirent **namesPlugs = NULL;
	void *handler = NULL;
	
	path = getenv("LD_LIBRARY_PATH");
	if (path == NULL) {
		perror("LD_LIBRARY_PATH is not set: ");
		exit(2);
	}
	
	count = scandir(path, &namesPlugs, filter, alphasort);
	if (count < 0) {
		perror("Fails scan directory: ");
		exit(3);
	}
	
	*(void **) &plugins = malloc(sizeof(plugin) * count);
	plugins = initPlugins(plugins, namesPlugs, count);
	
	fullPath = malloc((strlen(path) + 16) * sizeof(char));
	
	for (i = 0; i < count; i++) {
		sprintf(fullPath, "%s%s", path, plugins[i].execFile);
		handler = dlopen(fullPath, RTLD_LAZY);
		if (!handler) {
			perror("dlopen() error: ");
			exit(7);
		}
		plugins[i].namePlugin = dlsym(handler, NAME_PLUGIN);
		*(void **) (&plugins[i].funcPlug) = dlsym(handler, plugins[i].nameFunc);
		memset(fullPath, 0, strlen(fullPath));
	}
	
	while (1) {
		myComplex a, b, res;
		ch = 0;
		for (i = 0; i < count; i++) {
			printf("%d - %s\n", i + 1, plugins[i].namePlugin);
		}
		printf("%d - Exit\n", i + 1);
		printf("\n> ");
		scan("%d", &ch);
		
		if (ch == count + 1)
			break;
		
		printf("Input A:\n");
		inputComplex(&a);
		printf("Input B:\n");
		inputComplex(&b);
		printf("\n");
		printComplex(a);
		
		res = (plugins[ch - 1].funcPlug)(a, b);
		
		if (strcmp(plugins[ch - 1].namePlugin, "Add") == 0)
			printf("+\n");
		else if (strcmp(plugins[ch - 1].namePlugin, "Sub") == 0)
			printf("-\n");
		else if (strcmp(plugins[ch - 1].namePlugin, "Division") == 0)
			printf("/\n");
		else if (strcmp(plugins[ch - 1].namePlugin, "Mul") == 0)
			printf("*\n");
		else
			break;
			
		printComplex(b);
		printf("=\n");
		printComplex(res);
		
		getchar();
		
		fflush(NULL);
		if (system("clear") < 0) {
			perror("system clear: ");
			exit(6);
		}
	}
	return 0;
}
