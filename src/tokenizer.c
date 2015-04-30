#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

// List of valid symbols in a soos program, terminated by the empty string
// If you want to use introduce a new symbol to the language, add it here
const char *symbols[] = { "+","-","/","*","(",")","+=","-=","/=","*=","=","==","!=",";","<",">","#","++","--",""};

int tokenize(char *file, int fsize, token_t **tokens)
{
	char *pos = file;

	symbol_node *head = load_symbol_tree();

	token_t *cur = malloc(sizeof(token_t));
	*tokens = cur;
	cur->prev = NULL;

	if(tokenize_word(&pos, &cur->token, fsize - (pos - file), head)) {
		return -1;
	}


	while (pos - file < fsize) {
		char *token = NULL;
	
		if(tokenize_word(&pos, &token, fsize - (pos - file), head)) {
			return -1;
		}

		if (token) {
			cur->next = malloc(sizeof(token_t));
			cur->next->prev = cur;
			cur = cur->next;
			cur->next = NULL;
			cur->token = token;
		}
	}		

	free_symbol_tree(head);

	return 0;
}

/* Returns 0 on success, -1 on failure */
int tokenize_word(char **pos, char **token, int remaining_size, symbol_node *head)
{
	enum tokenize_state { NORMAL, QUOTES, QUOTES_BS, NUMBER, SYMBOL, DONE, FAILURE };

	while (is_whitespace(**pos) && remaining_size > 0) {
		(*pos)++;
		remaining_size--;
	}
	if (remaining_size <= 0) {
		// This means all the remaining characters were newlines
		return 0;
	}

	enum tokenize_state ts = NORMAL;

	char *start = *pos;
	if (is_number(**pos)) {
		ts = NUMBER;
	} else if (((**pos) == '\"') || ((**pos) == '\'')) {
		ts = QUOTES;
	} else if (is_valid_symbol_char(**pos, &head)) {
		ts = SYMBOL;
	} 
	int num_encountered_period = 0;
	while (ts != DONE && ts != FAILURE) {
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
				if (!is_valid_symbol_char(**pos, &head)) {
					ts = DONE;
				}
				break;
			case DONE:
				break;
			default:
				ts = FAILURE;
				break;
		}
	}			

	if (FAILURE == ts) {
		return -1;
	}

	int token_len = (*pos) - start;
	if (token_len < 1) {
		return -1;
	}

	*token = malloc(token_len + 1);
	memset(*token, 0, token_len + 1);
	strncpy(*token, start, token_len);

	return 0;
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

symbol_node *load_symbol_tree() {

	symbol_node *head = malloc(sizeof(symbol_node));
	head->child = NULL;
	head->sib = NULL;

	head->sym = symbols[0][0]; 

	int i = 0;
	while(strcmp(symbols[i],"")) {
		symbol_node *cur = head;
		int j = 0;
		int done = 0;
		while (!done) {
			if (cur->sym == symbols[i][j]) {
				j++;
				if (cur->child == NULL) {
					cur->child = malloc(sizeof(symbol_node));
					cur = cur->child;
					cur->child = NULL;
					cur->sib = NULL;
					cur->sym = symbols[i][j];
					if (cur->sym == '\0') {
						done = 1;
					}
				} else {
					cur = cur->child;
				}
			} else if (cur->sib != NULL) {
				cur = cur->sib;
			} else {
				cur->sib = malloc(sizeof(symbol_node));
				cur = cur->sib;
				cur->child = NULL;
				cur->sib = NULL;
				cur->sym = symbols[i][j];
				if (cur->sym == '\0') {
					done = 1;
				}
			}
		}
		i++;
		j=0;
	}
	return head;
}

void free_symbol_tree(symbol_node *ptr) {

	if (ptr->sib) {
		free_symbol_tree(ptr->sib);
		ptr->sib = NULL;
	}
	if (ptr->child) {
		free_symbol_tree(ptr->child);
		ptr->child = NULL;
	}

	free(ptr);
}

/* Takes a character and pointer into the symbol tree and returns 1 if that character is a sibling
 * of the pointer, and 0 otherwise.  Modifies cur to point to the first child of the found symbol on
 * success, and the end of the sibling list on failure
 */
int is_valid_symbol_char(char sym, symbol_node **cur) {

	if (sym == '\0') {
		return 0;
	}

	while(((*cur)->sym != sym) && ((*cur)->sib != NULL)) {
		(*cur) = (*cur)->sib;
	}

	if ((*cur)->sym == sym) {
		(*cur) = (*cur)->child;
		return 1;
	}
	return 0;
}
