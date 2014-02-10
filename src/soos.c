/* Soos programming language */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define MAX_LINE_LEN 160

typedef struct {
	int placeholder;
} options_t;

int parse_cmdline(char **argv, int argc, char **filename, options_t *opts);

int main(int argc, char **argv)
{
	options_t *opts = malloc(sizeof(options_t));
	char *filename = NULL;
	if (parse_cmdline(argv, argc, &filename, opts)) {
		//parse_cmdline should have already printed appropriate error message
		return -1;
	}

	FILE *fd = fopen(filename,"r");
	if (!fd) {
		printf("Error: Cannot open %s for reading\n",filename);
		return -1;
	}

	char line[MAX_LINE_LEN + 1];
	while (fgets(line,MAX_LINE_LEN,fd) && !feof(fd)) {
		// TODO: Check if MAX_LINE_LEN was exceeded
		printf("%s",line);	
	}

	if (!feof) {
		printf("Error reading from %s\n",filename);
	}

	fclose(fd);

	return 0;
}

int parse_cmdline(char **argv, int argc, char ** filename, options_t *opts)
{
	if (argc != 2) {
		printf("%d\n",argc);
		printf("USAGE: ./soos filename\n");
		return -1;
	}

	*filename = malloc (strlen(argv[1])+1);
	strcpy(*filename, argv[1]);
	return 0;
}
