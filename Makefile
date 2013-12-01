CC := clang++
CFLAGS := -g -Wall `llvm-config --cppflags` -O3
OFLAGS := -rdynamic
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
TEST_DIR := test
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_SRC_FILES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ_FILES := $(TEST_SRC_FILES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)
LDLIBS := -lpthread -lrt `llvm-config --ldflags --libs core jit native`
TARGET := my-lisp

all: lint $(TARGET) test

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -g $(OFLAGS) -o $@ $^ $(LDLIBS)

test: $(TEST_DIR)/test

$(TEST_DIR)/test: $(TEST_OBJ_FILES) $(filter-out $(OBJ_DIR)/$(TARGET).o, $(OBJ_FILES))
	$(CC) $(CFLAGS) -g $(OFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%_test.o: $(TEST_DIR)/%_test.cpp
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/test_%.cpp
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $< 

LINT := cppcheck
LINTFLAGS := -q --enable=all --check-config -I$(INC_DIR) -I`llvm-config --includedir`

lint:
	$(LINT) $(LINTFLAGS) $(SRC_DIR) $(TEST_DIR)

clean:
	rm -f $(OBJ_FILES) $(TEST_OBJ_FILES) *~ $(TARGET) $(TEST_DIR)/test

install:
	cp $(TARGET) /usr/local/bin

uninstall:
	rm /usr/local/bin/$(TARGET)
