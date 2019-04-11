CC := clang
OS := $(shell uname)
CFLAGS := -Werror -g
OBJDIR := obj
OBJ := pft.o
DEPS := 
ifeq (${OS},Linux)
	LIBS := -lrt -lm -lpthread
endif

default: pft

pft: $(OBJ) $(DEPS)
	gcc -o pft $(OBJ) $(CFLAGS) $(LIBS)
	chmod 755 pft

.PHONY: clean
clean:
	rm -rf *.dSYM *.o pft

.PHONY: test
test: pft
	./testit.sh 10 100

.PHONY: gotest
gotest: pft
	go run test-pft.go
