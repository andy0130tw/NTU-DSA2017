# Naive Makefile
ECHO   = /bin/echo -e
CFLAGS = -std=c++11 -O2 -DCHECK_QUERY -DVERBOSE
# CFLAGS = -std=c++11 -O2

% : %.cpp
	@$(ECHO) "> compiling: \033[34m$<\033[0m -> \033[01m\033[34m$@\033[0m"
	@$(CXX) $(CFLAGS) -o $@ $<

all: hw2
debug: CFLAGS += -g -O0 -Og
debug: hw2

clean:
	@rm -f hw2
