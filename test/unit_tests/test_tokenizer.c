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

	char *file1_expected[7] = {"float", "bmi", "=", "weight", "/", "height", ";"};
	char *file2_expected[5] = {"float", "pi", "=", "3.14159265", ";"};
	char *file3_expected[7] = {"print", "\"Hello World!\"", ";", "foo", "(", ")", ";"}; 

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

	tokenize_word(&string, &token, 21);
	CuAssertStrEquals(tc, " is a sentence.\n", string);
	CuAssertStrEquals(tc, "This", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 16);
	CuAssertStrEquals(tc, " a sentence.\n", string);
	CuAssertStrEquals(tc, "is", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 13);
	CuAssertStrEquals(tc, " sentence.\n", string);
	CuAssertStrEquals(tc, "a", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 11);
	CuAssertStrEquals(tc, ".\n", string);
	CuAssertStrEquals(tc, "sentence", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 2);
	CuAssertStrEquals(tc, "\n", string);
	CuAssertStrEquals(tc, ".", token);
	free(token);
	token = NULL;

	tokenize_word(&string, &token, 1);
	CuAssertStrEquals(tc, "", string);
	CuAssertPtrEquals(tc, NULL, token);

}

CuSuite *TokenizerGetSuite() {
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_is_word);
	SUITE_ADD_TEST(suite, test_is_whitespace);
	SUITE_ADD_TEST(suite, test_tokenize_word);
	SUITE_ADD_TEST(suite, test_tokenize);
	return suite;
}
