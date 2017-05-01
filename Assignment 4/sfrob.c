// sfrob.c
// By Naim Ayat

// takes two arguments a and b as input and returns an int result that is
// negative, zero, or positive depending on whether a is less than, equal to,
// or greater than b.

#include<stdlib.h>
#include<stdio.h>

// compares words a and b
// returns -1 if a < b, 0 if a == b, 1 if a > b
int frobcmp(char const *a, char const *b) {
	while (1) {
		if ((*a == ' ') && (*b == ' '))
			return 0;
		else if (*a == ' ')
			return -1;
		else if (*b == ' ')
			return 1;
		else if ((*a ^ 42) < (*b ^ 42))
			return -1;
		else if ((*a ^ 42) > (*b ^ 42))
			return 1;
		a++;
		b++;
	}
}

// check for input error
void checkInput()
{
	if (ferror(stdin)) {
		fprintf(stderr, "Input error.");
		exit(1);
	}
}

// helper function for qsort
int cmp(const void* first, const void* second)
{
	const char* x = *(const char**)first;
	const char* y = *(const char**)second;
	return frobcmp(x, y);
}

int main()
{
	// vars for malloc
	int buffer = 2048;
	int count = 0;

	char *array = (char*)malloc(sizeof(char) * buffer);

	if (array == NULL) {
		fprintf(stderr, "Memory allocation error.");
		exit(1);
	}

	int currentChar;
	int checkEmpty = 0;
	while (1)
	{
		currentChar = getchar();
		checkInput();
		if (currentChar == EOF)
			break;
		checkEmpty = 1;
		array[count++] = (char)currentChar;
		if (count == buffer) {
			array = (char*)realloc(array, buffer * 2);
			if (array == NULL) {
				fprintf(stderr, "Memory allocation error.");
				exit(1);
			}
			buffer = buffer * 2;
		}
	}

	// was an empty file passed in?
	if (checkEmpty == 0)
		return 0;

	// add a space to the end of a word if needed
	if (array[count - 1] != ' ')
		array[count++] = ' ';

	int numWords = 0;
	int i = 0;
	while (i < count)
	{
		if (array[i] == ' ')
			numWords++;
		i++;
	}

	char **ptrArray = (char**)malloc(sizeof(char*) * numWords);
	int *letters = (int*)malloc(sizeof(int) * numWords);
	if (ptrArray == NULL) {
		fprintf(stderr, "Memory allocation error.");
		exit(1);
	}
	if (letters == NULL) {
		fprintf(stderr, "Memory allocation error.");
		exit(1);
	}

	int ptrArrayIter = 0;
	char *ptrArrayPtr = array;
	i = 0;
	while (i < numWords)
	{
		if (i == 0 && *ptrArrayPtr == ' ')
		{
			ptrArray[ptrArrayIter++] = ptrArrayPtr;
			i++;
			ptrArrayPtr++;
		}
		ptrArray[ptrArrayIter++] = ptrArrayPtr;
		while (*ptrArrayPtr != ' ')
			ptrArrayPtr++;

		ptrArrayPtr++;
		i++;
	}

	qsort(ptrArray, ptrArrayIter, sizeof(char*), cmp);

	int lengthIndex = 0;
	int length = 1;
	i = 0;
	while (i < numWords)
	{
		ptrArrayPtr = ptrArray[i];
		while (*ptrArrayPtr != ' ')
		{
			ptrArrayPtr++;
			length++;
		}
		letters[lengthIndex++] = length;
		length = 1;
		i++;
	}

	i = 0;
	while (i < numWords)
	{
		char *out = ptrArray[i];
		int j = 0;
		for (; j < letters[i]; j++)
		{
			printf("%c", *out);
			out++;
		}
		i++;
	}

	// free all allocated memory
	free(array);
	free(ptrArray);
	free(letters);
	exit(0);
}