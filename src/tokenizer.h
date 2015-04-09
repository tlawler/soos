typedef struct token_t {
	char *token;
	struct token_t *prev;
	struct token_t *next;
} token_t;

int tokenize(char *file, int fsize, token_t **tokens);
void tokenize_word(char **pos, char **token, int remaining_size);
int is_whitespace(char c);
int is_word(char c);
void parse_symbol(char **pos, char **token);
void free_tokens(token_t *token_head);


