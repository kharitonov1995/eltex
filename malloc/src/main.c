#include "../lib/malloc.c"

int main() {
	int *arr, *arr1, n = 10, i;
	arr = malloc(n);
	arr1 = malloc(n);
	for (i = 0; i < n; i++) {
		arr[i] = i + 1;
		arr1[(n - 1) - i] = i;
	}
	for (i = 0; i < n; i++)
		printf("arr[%d] = %d\n", i, arr[i]);
	for (i = 0; i < n; i++)
		printf("arr1[%d] = %d\n", i, arr1[i]);

	free(arr);

	arr = malloc(n);
	for (i = 0; i < n; i++)
		printf("arr[%d] = %d\n", i, arr[i]);
	return 0;
}
