#include <stdlib.h>
#include <string.h>

#include "../cutest/CuTest.h"
#include "../../src/tokenizer.h"

void check_tokens(CuTest *tc, token_t *head, char *expected[], int expected_len) {
	int i = 0;
	while (head) {
		CuAssertStrEquals(tc, expected[i], head->token);
		head = head->next;
		i++;
	}
	CuAssertIntEquals(tc, expected_len, i);
}

void test_tokenize(CuTest *tc) {
	char *file1 = "float bmi = weight/height;";
	char *file2 = "float pi = 3.14159265;";
	char *file3 = "print \"Hello World!\";\nfoo();\n";
	char *file4 = "print \"\\\"Hello World!\\\"\";\nint i = 0;\n\nbar(i++);\\";

	char *file1_expected[7] = {"float", "bmi", "=", "weight", "/", "height", ";"};
	char *file2_expected[5] = {"float", "pi", "=", "3.14159265", ";"};
	char *file3_expected[7] = {"print", "\"Hello World!\"", ";", "foo", "(", ")", ";"};
	char *file4_expected[15] = {"print", "\"\\\"Hello World!\\\"\"", ";", "int", "i", "=",\
					 "0", ";", "bar", "(", "i", "++", ")", ";", "\\" };

	token_t *head = NULL;

	CuAssertIntEquals(tc, 0, tokenize(file1, strlen(file1), &head));
	
	check_tokens(tc, head,  file1_expected, 7);
	free_tokens(head);
	head = NULL;

	CuAssertIntEquals(tc, 0, tokenize(file2, strlen(file2), &head));

	check_tokens(tc, head,  file2_expected, 5);
	free_tokens(head);
	head = NULL;

	CuAssertIntEquals(tc, 0, tokenize(file3, strlen(file3), &head));

	check_tokens(tc, head, file3_expected, 7);
	free_tokens(head);
	head = NULL;

	CuAssertIntEquals(tc, 0, tokenize(file4, strlen(file4), &head));

	check_tokens(tc, head, file4_expected, 15);
	free_tokens(head);
	head = NULL;
}

void test_is_word(CuTest *tc) {
	CuAssertTrue(tc, is_word('c'));
	CuAssertTrue(tc, is_word('A'));
	CuAssertTrue(tc, is_word('3'));
	CuAssertTrue(tc, is_word('1'));
	CuAssertTrue(tc, is_word('z'));
	CuAssertTrue(tc, is_word('Z'));
	CuAssertTrue(tc, is_word('0'));
	CuAssertTrue(tc, !is_word('.'));
	CuAssertTrue(tc, !is_word('\"'));
	CuAssertTrue(tc, !is_word('\0'));
	CuAssertTrue(tc, !is_word('#'));
	CuAssertTrue(tc, !is_word(';'));
}

void test_is_whitespace(CuTest *tc) {
	CuAssertTrue(tc, is_whitespace(' '));
	CuAssertTrue(tc, is_whitespace('\n'));
	CuAssertTrue(tc, is_whitespace('\t'));
	CuAssertTrue(tc, !is_whitespace('.'));
	CuAssertTrue(tc, !is_whitespace('1'));
}

void test_tokenize_word(CuTest *tc) {
	char *string = "This is a sentence.\n";
	char *token = NULL;

	symbol_node *head = load_symbol_tree();

	tokenize_word(&string, &token, 21, head);
	CuAssertStrEquals(tc, " is a sentence.\n", string);
	CuAssertStrEquals(tc, "This", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 16, head);
	CuAssertStrEquals(tc, " a sentence.\n", string);
	CuAssertStrEquals(tc, "is", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 13, head);
	CuAssertStrEquals(tc, " sentence.\n", string);
	CuAssertStrEquals(tc, "a", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 11, head);
	CuAssertStrEquals(tc, ".\n", string);
	CuAssertStrEquals(tc, "sentence", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 2, head);
	CuAssertStrEquals(tc, "\n", string);
	CuAssertStrEquals(tc, ".", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 1, head);
	CuAssertStrEquals(tc, "", string);
	CuAssertPtrEquals(tc, NULL, token);

	free_symbol_tree(head);

}

void test_load_symbol_tree(CuTest *tc) {

	symbol_node *head = load_symbol_tree();
	symbol_node *ptr = head;

	CuAssertPtrNotNull(tc, ptr);

	CuAssertTrue(tc, ('+' == ptr->sym));
	CuAssertPtrNotNull(tc, ptr->child);

	CuAssertPtrNotNull(tc, ptr->sib);
	ptr = ptr->sib;

	CuAssertTrue(tc, ('-' == ptr->sym));
	CuAssertPtrNotNull(tc, ptr->sib);

	CuAssertPtrNotNull(tc, ptr->child);
	ptr = ptr->child;

	CuAssertTrue(tc, ('\0' == ptr->sym));
	CuAssertPtrEquals(tc, NULL, ptr->child);

	CuAssertPtrNotNull(tc, ptr->sib);
	ptr = ptr->sib;

	CuAssertTrue(tc, ('=' == ptr->sym));

	free_symbol_tree(head);
	
}

void test_is_valid_symbol_char(CuTest *tc) {

	symbol_node *head = load_symbol_tree();
	symbol_node *pos = head;

	CuAssertTrue(tc, is_valid_symbol_char('+',&pos));
	CuAssertTrue(tc, is_valid_symbol_char('=',&pos));
	CuAssertTrue(tc, !is_valid_symbol_char('=',&pos));

	pos = head;	

	CuAssertTrue(tc, is_valid_symbol_char('/',&pos));
	CuAssertTrue(tc, !is_valid_symbol_char('+',&pos));

	free_symbol_tree(head);
}

CuSuite *TokenizerGetSuite() {
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_is_word);
	SUITE_ADD_TEST(suite, test_is_whitespace);
	SUITE_ADD_TEST(suite, test_tokenize_word);
	SUITE_ADD_TEST(suite, test_tokenize);
	SUITE_ADD_TEST(suite, test_load_symbol_tree);
	SUITE_ADD_TEST(suite, test_is_valid_symbol_char);
	return suite;
}
