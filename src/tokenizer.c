#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

int tokenize(char *file, int fsize, token_t **tokens)
{
	char *pos = file;

	token_t *cur = malloc(sizeof(token_t));
	*tokens = cur;
	cur->prev = NULL;

	tokenize_word(&pos, &cur->token, fsize - (pos - file));

	while (pos - file < fsize) {
		char *token = NULL;
		tokenize_word(&pos, &token, fsize - (pos - file));

		if (token) {
			cur->next = malloc(sizeof(token_t));
			cur->next->prev = cur;
			cur = cur->next;
			cur->next = NULL;
			cur->token = token;
		}
	}		

	return 0;
}

void tokenize_word(char **pos, char **token, int remaining_size)
{
	while (is_whitespace(**pos) && remaining_size > 0) {
		(*pos)++;
		remaining_size--;
	}
	if (remaining_size <= 0) {
		return;
	}
	if (is_word(**pos)) {
		char *word_start = *pos;
		while (is_word(**pos)) {
			(*pos)++;
		}
		int token_len = (*pos) - word_start;
		*token = malloc(token_len + 1);
		memset(*token, 0, token_len + 1);
		strncpy(*token, word_start, token_len);
		
	} else {
		parse_symbol(pos, token);
	}
}

int is_whitespace(char c)
{
	return  (( c == ' ')  ||
	         ( c == '\n') ||
	         ( c == '\t'));
}

int is_word(char c)
{
	return (( (int)c >= (int)'0') &&
	        ( (int)c <= (int)'z'));
}

void parse_symbol(char **pos, char **token)
{
	char *sym_start = *pos;
	while (!is_whitespace(**pos) && !is_word(**pos)) {
		(*pos)++;
	}
	size_t symbol_len = (*pos) - sym_start;
	*token = malloc(symbol_len);
	strncpy(*token, sym_start, symbol_len);
}

void free_tokens(token_t *token_head) {
	if (token_head->next) {
		free_tokens(token_head->next);
		token_head->next = NULL;
	}
	free(token_head->token);
	free(token_head);
}
