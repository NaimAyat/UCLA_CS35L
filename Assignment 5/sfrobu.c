// sfrobu.c
// By Naim Ayat

// takes two arguments a and b as input and returns an int result that is
// negative, zero, or positive depending on whether a is less than, equal to,
// or greater than b.

// accepts an option -f to ignore character case.

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

// check for errors reading input
void readCheck(ssize_t temp) {
	if (temp < 0) {
		fprintf(stderr, "Read error.");
		exit(1);
	}
}

// check for memory allocation errors
void memCheck(void* pointer) {
	if (pointer == NULL) {
		fprintf(stderr, "Memory allocation error.");
		exit(1);
	}
}

// compares words a and b
// returns -1 if a < b, 0 if a == b, 1 if a > b
int frobcmp(const char *a, const char *b) {
	for (;; a++, b++) {
		if (*a == ' ' && *b == ' ') {
			return 0;
		}
		else if (*a == ' ' || ((*a ^ 42) < (*b ^ 42))) {
			return -1;
		}
		else if (*b == ' ' || ((*a ^ 42) > (*b ^ 42))) {
			return 1;
		}
	}
}

// same as frobcmp, but for -f command
int toupperFrobcmp(const char *a, const char *b) {
	if ((*a ^ 42) > UCHAR_MAX || (*a ^ 42) < 0 || (*b ^ 42) > UCHAR_MAX || (*b ^ 42) < 0) {
		fprintf(stderr, "Invalid toupper.");
		exit(1);
	}
	for (;; a++, b++) {
		if (*a == ' ' && *b == ' ') {
			return 0;
		}
		else if (*a == ' ' || (toupper(*a ^ 42) < toupper(*b ^ 42))) {
			return -1;
		}
		else if (*b == ' ' || (toupper(*a ^ 42) > toupper(*b ^ 42))) {
			return 1;
		}
	}
}

// helper function for qsort
int cmpForQsort(const void *a, const void *b) {
	const char *x = *(const char **)a;
	const char *y = *(const char **)b;
	return frobcmp(x, y);
}

// helper function for qsort for -f command
int toupperCmpForQsort(const void *a, const void *b) {
	const char *x = *(const char **)a;
	const char *y = *(const char **)b;
	return toupperFrobcmp(x, y);
}

int main(int argc, char* argv[]) {
	char* wordArrayComplete;
	char** wordArray;
	if (argc > 2) {
		fprintf(stderr, "Maximum of one argument\n");
		exit(1);
	}
	// basically a bool variable, but there's no bool type in C
	int toupper = 0;
	if (argc == 2) {
		if (strcmp(argv[1], "-f") == 0) {
			toupper = 1;
		}
		else {
			fprintf(stderr, "Invalid option.");
			exit(1);
		}
	}
	struct stat filestat;
	if (fstat(0, &filestat) < 0) {
		fprintf(stderr, "Error reading filestat.");
		exit(1);
	}
	if (filestat.st_size > 0) {
		wordArrayComplete = (char*)malloc(filestat.st_size + 1);
		memCheck(wordArrayComplete);
		ssize_t temp = read(0, wordArrayComplete, filestat.st_size);
		readCheck(temp);
	}
	else {
		wordArrayComplete = (char*)malloc(1);
	}
	// for file that grows during reading
	char current;
	ssize_t newTemp = read(0, &current, 1);
	int additional_chars = 0;
	while (newTemp > 0) {
		char* temp = realloc(wordArrayComplete, filestat.st_size + additional_chars + 2);
		memCheck(temp);
		wordArrayComplete = temp;
		wordArrayComplete[filestat.st_size + additional_chars] = current;
		++additional_chars;
		newTemp = read(0, &current, 1);
	}
	readCheck(newTemp);
	// array of ptrs to char arrays
	int wordcount = 0;
	int i;
	if (filestat.st_size + additional_chars == 1) {
		if (wordArrayComplete[0] != ' ') {
			++wordcount;
		}
	}
	else if (filestat.st_size + additional_chars == 2) {
		if (wordArrayComplete[0] != ' ' || wordArrayComplete[1] != ' ') {
			++wordcount;
		}
	}
	else {
		for (i = 0; i < filestat.st_size + additional_chars - 1; ++i) {
			if (wordArrayComplete[i] != ' ' && (wordArrayComplete[i + 1] == ' ' || i == filestat.st_size + additional_chars - 2)) {
				++wordcount;
			}
		}
	}
	wordArray = (char**)malloc(wordcount * sizeof(char*));
	memCheck(wordArray);
	// check if file is empty
	if (wordcount == 0) {
		free(wordArrayComplete);
		return 0;
	}
	int wordItr = 0;
	// check if file has only one char
	if (filestat.st_size + additional_chars == 1) {
		wordArray[0] = wordArrayComplete;
		wordArrayComplete[1] = ' ';
		++wordItr;
	}
	else {
		char* curr = &wordArrayComplete[0];
		char* next = &wordArrayComplete[1];
		if (*curr != ' ') {
			wordArray[wordItr] = curr;
			++wordItr;
		}
		while (next != &wordArrayComplete[filestat.st_size + additional_chars]) {
			if (*curr == ' ' && *next != ' ') {
				wordArray[wordItr] = next;
				++wordItr;
			}
			++curr; ++next;
		}

		if (*next != ' ') {
			wordArrayComplete[filestat.st_size + additional_chars] = ' ';
		}
	}
	if (toupper) {
		qsort(wordArray, wordItr, sizeof(char*), toupperCmpForQsort);
	}
	else {
		qsort(wordArray, wordItr, sizeof(char*), cmpForQsort);
	}
	int j;
	for (j = 0; j < wordItr; ++j) {
		char* tempChar = wordArray[j];
		while (*tempChar != ' ') {
			write(1, tempChar, 1);
			++tempChar;
		}
		char SP = ' ';
		write(1, &SP, 1);
	}
	free(wordArray);
	free(wordArrayComplete);
	return 0;
}