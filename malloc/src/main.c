#include "../lib/malloc.c"

int main() {
	int *arr, n = 10, i;
	arr = malloc(n);
	for (i = 0; i < n; i++) {
		arr[i] = i + 1;
	}
	for (i = 0; i < n; i++)
		printf("arr[%d] = %d\n", i, arr[i]);
	return 0;
}
