typedef struct token_t {
	char *token;
	struct token_t *prev;
	struct token_t *next;
} token_t;

typedef struct symbol_node {
	char sym;
	struct symbol_node *sib;
	struct symbol_node *child;
} symbol_node;

int tokenize(char *file, int fsize, token_t **tokens);
int tokenize_word(char **pos, char **token, int remaining_size, symbol_node *head);
int is_whitespace(char c);
int is_word(char c);
int is_number(char c);
void parse_symbol(char **pos, char **token);
void free_tokens(token_t *token_head);
symbol_node *load_symbol_tree();
void free_symbol_tree(symbol_node *head);
int is_valid_symbol_char(char sym, symbol_node **cur);
