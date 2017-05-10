// tr2u.c
// By Naim Ayat

// uses unbuffered I/O (read and write) to copy the standard input to the
// standard output with substitution or deletion of selected characters.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Error: wrong number of args");
		exit(1);
	}
	char* from = argv[1];
	char* to = argv[2];
	if (strlen(from) != strlen(to)) {
		fprintf(stderr, "Error: args must be the same length");
		exit(1);
	}
	size_t i, j;
	for (i = 0; i < strlen(from); i++) {
		for (j = i + 1; j < strlen(from); j++) {
			if (from[i] == from[j]) {
				fprintf(stderr, "Error: first arg cannot contain duplicates");
				exit(1);
			}
		}
	}
	char current[1];
	while (read(0, current, 1) > 0) {
		for (i = 0; i < strlen(from); i++) {
			if (current[0] == from[i]) {
				current[0] = to[i];
				break;
			}
		}
		write(1, current, 1);
	}
	return 0;
}