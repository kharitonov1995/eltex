#include "../lib/complex.c"

int main() {
	char ch = '\0';
	int count = 0;
	myComplex (**plugins)(myComplex, myComplex);
	char **namePlugins = NULL;
	
	namePlugins = getNamesPlugins(&count);
	printf("%d\n%s\n", count, namePlugins[0]);
	
	*plugins = malloc(sizeof(myComplex*) * count);
	
	/*while (1) {
		myComplex a, b, res;

		printf("Input A:\n");
		inputComplex(&a);
		printf("Input B:\n");
		inputComplex(&b);
				
		printf("1 - Abb\n2 - Sub\n3 - Mul\n4 - Div\n> ");
		system("stty -icanon");
		ch = getchar();
		system("stty cooked");
		
		printf("\n");
		printComplex(a);
		/*switch(ch) {
			case '1'://add;
				printf("+\n");
				res = Add(a, b);
				break;
			case '2'://sub;
				printf("-\n");
				res = Sub(a, b);
				break;
			case '3'://mul;
				printf("*\n");
				res = Mul(a, b);
				break;
			case '4'://div;
				printf("/\n");
				res = Div(a, b);
				break;
			default:
				break;
		}*/
		/*printComplex(b);
		printf("=\n");
		printComplex(res);
		getchar();
		system("clear");
	}*/
	return 0;
}
