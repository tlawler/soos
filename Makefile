UNIT_TESTS_PATH = test/unit_tests

NON_MAIN_OBJS = src/tokenizer.o
OBJS = src/soos.o ${NON_MAIN_OBJS}
TEST_OBJS = ${NON_MAIN_OBJS} ${UNIT_TESTS_PATH}/test_tokenizer.o

DEBUG?=1

CFLAGS=--std=c99 -Wall -fstack-protector-strong

ifeq (${DEBUG}, 1)
	CFLAGS+=-g
else
	CFLAGS+=-O2
endif

TOKENIZER_DEPS = src/tokenizer.h
MAIN_DEPS = ${TOKENIZER_DEPS}

all: soos

tests: ${UNIT_TESTS_PATH}/AllTests ${TEST_OBJS}
	${UNIT_TESTS_PATH}/AllTests

src/soos.o: src/soos.c ${MAIN_DEPS}
	gcc -c ${CFLAGS} src/soos.c -o src/soos.o

src/tokenizer.o: src/tokenizer.c ${TOKENIZER_DEPS}
	gcc -c ${CFLAGS} src/tokenizer.c -o src/tokenizer.o

soos: ${OBJS}
	gcc ${CFLAGS} ${OBJS} -o soos

${UNIT_TESTS_PATH}/test_tokenizer.o: ${UNIT_TESTS_PATH}/test_tokenizer.c ${TOKENIZER_DEPS}
	gcc -c ${CFLAGS} ${UNIT_TESTS_PATH}/test_tokenizer.c -o ${UNIT_TESTS_PATH}/test_tokenizer.o

${UNIT_TESTS_PATH}/AllTests: ${TEST_OBJS}
	gcc ${CFLAGS} test/cutest/CuTest.c ${UNIT_TESTS_PATH}/AllTests.c ${TEST_OBJS} -o ${UNIT_TESTS_PATH}/AllTests

clean:
	rm -f soos src/*.o ${UNIT_TESTS_PATH}/*.o ${UNIT_TESTS_PATH}/AllTests
