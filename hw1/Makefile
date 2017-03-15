# Naive Makefile
ECHO   = /bin/echo -e
CFLAGS = -std=c++11 -O2

% : %.cpp
	@$(ECHO) "> compiling: \033[34m$<\033[0m -> \033[01m\033[34m$@\033[0m"
	@$(CC) $(CFLAGS) -o $@ $<

all: hw1

clean:
	@rm -f hw1
