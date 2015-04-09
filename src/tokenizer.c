#include <stdio.h>
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
	enum tokenize_state { NORMAL, QUOTES, QUOTES_BS, NUMBER, SYMBOL, DONE };

	while (is_whitespace(**pos) && remaining_size > 0) {
		(*pos)++;
		remaining_size--;
	}
	if (remaining_size <= 0) {
		return;
	}

	enum tokenize_state ts = NORMAL;

	char *start = *pos;
	if (is_number(**pos)) {
		ts = NUMBER;
	} else if (((**pos) == '\"') || ((**pos) == '\'')) {
		ts = QUOTES;
	} else if (!is_word(**pos)) {
		ts = SYMBOL;
	}
	int num_encountered_period = 0;
	while (ts != DONE) {
		(*pos)++;
		switch (ts) {
			case NORMAL:
				if (!is_word(**pos)) {
					ts = DONE;
				}
				break;
			case QUOTES:
				if ((**pos) == '\\') {
					ts = QUOTES_BS;
				} else if (((**pos) == '\"') || ((**pos) == '\'')) {
					(*pos)++; //To include the final quote char
					ts = DONE;
				}
				break;
			case QUOTES_BS:
				ts = QUOTES;
				break;
			case NUMBER:
				if ((num_encountered_period == 0) && (**pos == '.')) {
					num_encountered_period = 1;
					break;
				}
				if (!is_number(**pos)) {
					ts = DONE;
				}
				break;
			case SYMBOL:
				if ((is_whitespace(**pos) || is_word(**pos))) {
					ts = DONE;
				}
				break;
			case DONE:
				break;
			default:
				printf("Something has gone horribly wrong in tokenize_word\n");
				ts = DONE;
				break;
		}
	}			

	int token_len = (*pos) - start;
	*token = malloc(token_len + 1);
	memset(*token, 0, token_len + 1);
	strncpy(*token, start, token_len);
		
}

int is_whitespace(char c)
{
	return  (( c == ' ')  ||
	         ( c == '\n') ||
	         ( c == '\t'));
}

int is_word(char c)
{
	return (is_number(c) ||
		(( (int)c >= (int)'A') &&
	         ( (int)c <= (int)'z')));
}

int is_number(char c)
{
	return (( (int)c >= (int)'0') &&
		( (int)c <= (int)'9'));
}

void free_tokens(token_t *token_head) {
	if (token_head->next) {
		free_tokens(token_head->next);
		token_head->next = NULL;
	}
	free(token_head->token);
	free(token_head);
}
