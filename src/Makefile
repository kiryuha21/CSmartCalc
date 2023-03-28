CC = gcc

GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic

LINK_LIBRARIES = `pkg-config --libs gtk+-3.0` -lcheck
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LINK_LIBRARIES += -lsubunit -lm
endif
#ifeq ($(UNAME_S),Darwin)
#	something for MACOS
#endif

SRC_C_FILES = lib/*.c dynamic_structures/*.c
SRC_H_FILES = s21_smart_calc.h

TEST_C_FILES = unit_tests/*.c
TEST_H_FILES = unit_tests/unit_tests.h

TEST_TARGET = s21_test
TARGET = s21_calc

LIB = s21_smart_calc.a

REPORT_DIR = report

all: clean test gcov_report $(LIB)

apply_clang:
	cp ../materials/linters/.clang-format .
	clang-format -i *.[ch]
	clang-format -i */*.[ch]
	rm -f .clang-format

build: apply_clang $(SRC_C_FILES) $(SRC_H_FILES)
	$(CC) $(FLAGS) $(GTK_FLAGS) $(SRC_C_FILES) main.c -o $(TARGET) $(LINK_LIBRARIES)

test: apply_clang $(SRC_C_FILES) $(SRC_H_FILES)
	$(CC) $(FLAGS) $(SRC_C_FILES) $(TEST_C_FILES) -o $(TEST_TARGET) $(LINK_LIBRARIES)
	./$(TEST_TARGET)

gcov_report: apply_clang
	$(CC) --coverage $(FLAGS) $(SRC_C_FILES) $(TEST_C_FILES) -o $(TEST_TARGET) $(LINK_LIBRARIES)
	./$(TEST_TARGET)
	lcov -t "$(TEST_TARGET)" -o $(TEST_TARGET).info -c -d .
	genhtml -o report $(TEST_TARGET).info
	rm *.gcda *.gcno *.info
	open report/index.html

$(LIB): apply_clang
	$(CC) -c $(SRC_C_FILES) $(LINK_LIBRARIES)
	ar rcv $(LIB) *.o
	ranlib $(LIB)
	rm *.o

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

clean_test:
	rm -f $(TEST_TARGET)

clean_report:
	rm -rf $(REPORT_DIR)

clean_lib:
	rm -f $(LIB)

clean_obj:
	rm -f *.o

clean_gcov:
	rm -f *.gc*

clean: clean_test clean_report clean_lib clean_obj clean_gcov
