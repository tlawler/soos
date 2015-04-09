/* Soos programming language */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "tokenizer.h"

#define MAX_LINE_LEN 160

typedef struct {
	int placeholder;
} options_t;

//enum token_state = {START, WHITESPACE, WORD, SYMBOL};

int parse_cmdline(char **argv, int argc, char **filename, options_t *opts);
int load_file(char *filename, char **file);

int main(int argc, char **argv)
{
	options_t *opts = malloc(sizeof(options_t));
	char *filename = NULL;
	char *file = NULL;
	token_t *tokens = NULL;
	if (parse_cmdline(argv, argc, &filename, opts)) {
		//parse_cmdline should have already printed appropriate error message
		return -1;
	}

	int fsize = load_file(filename, &file);
	if (fsize < 0) {
		return -1;
	}
	if (tokenize(file, fsize, &tokens)) {
		return -1;
	}

	token_t *tokens_orig = tokens;

	while (tokens) {
		printf("%s\n",tokens->token);
		tokens = tokens->next;
	}

	tokens = tokens_orig;

	free_tokens(tokens);

	return 0;
}

int parse_cmdline(char **argv, int argc, char ** filename, options_t *opts)
{
	if (argc != 2) {
		printf("USAGE: ./soos filename\n");
		return -1;
	}

	*filename = malloc (strlen(argv[1])+1);
	strcpy(*filename, argv[1]);
	return 0;
}

int load_file(char *filename, char **file)
{
	FILE *fd = fopen(filename,"rb");
	if (!fd) {
		printf("Error: Cannot open %s for reading\n", filename);
		return -1;
	}

	fseek(fd, 0, SEEK_END);
	long fsize = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	*file = malloc(fsize + 1);
	if (0 == fread(*file, 1, fsize, fd)) {
		printf("Error: Read failed on file: %s\n", filename);
		return -1;
	}

	fclose(fd);

	(*file)[fsize] = '\0';

	return fsize;
}

