// tr2b.c
// By Naim Ayat

// uses buffered I/O (getchar and putchar) to copy the standard input to the
// standard output with substitution or deletion of selected characters.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Error: wrong number of arguments.");
		exit(1);
	}
	char* from = argv[1];
	char* to = argv[2];
	if (strlen(from) != strlen(to)) {
		fprintf(stderr, "Error: arguments must contain the same number of characters.");
		exit(1);
	}
	size_t i, j;
	for (i = 0; i < strlen(from); i++) {
		for (j = i + 1; j < strlen(from); j++) {
			if (from[i] == from[j]) {
				fprintf(stderr, "Error: first (from) argument cannot contain duplicate characters.");
				exit(1);
			}
		}
	}
	char current;
	int put = 1;
	while ((current = getchar()) != EOF) {
		for (i = 0; i < strlen(from); i++) {
			if (current == from[i]) {
				putchar(to[i]);
				put = 0;
				break;
			}
			else put = 1;
		}
		if (put) putchar(current);
	}
	return 0;
}
