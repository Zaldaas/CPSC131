define say
$(info [Project 1] $1)
endef

CC := g++
LINK_FLAGS := -std=c++17 -Wall -g
CC_FLAGS := $(LINK_FLAGS) -c 

# Determine some more paths
HOSTNAME := $(shell hostname)
SUBMISSION_DIR := .
RESULTS_JSON := ./results.json
RESULTS_JSON_FOR_GRADESCOPE := /autograder/results/results.json

$(call say,Hostname: $(HOSTNAME))
$(call say,Submission dir: $(SUBMISSION_DIR))
$(call say,Results path: $(RESULTS_JSON))
$(call say,Results path: $(RESULTS_JSON_FOR_GRADESCOPE))
#######


# Binary names
BIN_NAME := main
BIN := ./$(BIN_NAME)
#
TESTS_BIN_NAME := main-tests
TESTS_BIN := ./$(TESTS_BIN_NAME)
#
LEAK_CHECKER_BIN_NAME := leak-checker
LEAK_CHECKER_BIN = ./$(LEAK_CHECKER_BIN_NAME)


#
TESTS_DEPS := $(SUBMISSION_DIR)/CPP_Tests.cpp $(wildcard $(SUBMISSION_DIR)/puhp-tests/*.hpp) $(SUBMISSION_DIR)/MyVector.hpp
$(call say,Test dependencies: $(TESTS_DEPS))

$(info )


#
default: help
.PHONY: default


#
help:
	@echo "***** Makefile Menu *****"
	@echo
	@echo "make build         ==> Build source files"
	@echo
	@echo "make run           ==> Run the debugging sandbox"
	@echo "make debug         ==> Debug the debugging sandbox"
	@echo
	@echo "make test          ==> Run tests against your dynamic vector"
	@echo "make debug-test    ==> Debug the tests run against your dynamic vector"
	@echo
	@echo "make clean         ==> Clean temporary build files"
.PHONY: help


# Run the debug sandbox
run:	$(BIN)
	$(BIN)
.PHONY: run


# Debug the debug sandbox
debug:	$(BIN)
	gdb $(BIN) -ex run
.PHONY: debug


# Build everything
build:	$(BIN) $(TESTS_BIN)
.PHONY: build


# Run tests
gradescope: clean
gradescope: test
tests: test
test: $(TESTS_BIN) $(LEAK_CHECKER_BIN)
	$(call say,Executing: $<)
	$(TESTS_BIN)
.PHONY: tests test gradescope


# Debug the tests
debug-tests:		debug-test
debug-test:	$(TESTS_BIN)
	gdb $(TESTS_BIN) -ex run
.PHONY: debug-tests debug-test


#
$(SUBMISSION_DIR)/main.o:	$(SUBMISSION_DIR)/main.cpp $(SUBMISSION_DIR)/MyVector.hpp
	$(CC) $(CC_FLAGS) "$<" -o "$@"


#
$(SUBMISSION_DIR)/tests.o:	$(TESTS_DEPS)
	$(CC) $(CC_FLAGS) "$<" -o "$@"


#
$(SUBMISSION_DIR)/leak_checker.o:	$(SUBMISSION_DIR)/leak_checker.cpp $(SUBMISSION_DIR)/MyVector.hpp
	$(CC) $(CC_FLAGS) "$<" -o "$@"


# Link the main binary
$(BIN):	$(SUBMISSION_DIR)/main.o
	$(CC) $(LINK_FLAGS) $^ -o "$@"


# Link the test binary
$(TESTS_BIN):	$(SUBMISSION_DIR)/tests.o
	$(call say,Building: $@)
	$(CC) $(LINK_FLAGS) "$^" -o "$@"


# Link the leak checker binary
$(LEAK_CHECKER_BIN):	leak_checker.o
	$(call say,Building: "$@")
	$(CC) $(LINK_FLAGS) $^ -o "$@"


#
clean:
	-rm -f $(BIN)
	-rm -f $(TESTS_BIN)
	-rm -f $(LEAK_CHECKER_BIN)
	-rm -f $(RESULTS_JSON)
	-rm *.o
.PHONY: clean



