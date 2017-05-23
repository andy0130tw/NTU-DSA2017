# Naive Makefile
ECHO   = /bin/echo -e
CSRCS  = $(wildcard *.cpp)
CHDRS  = $(wildcard *.h)
COBJS  = $(addsuffix .o, $(basename $(CSRCS)))
CFLAGS = -std=c++11 -O2
# CFLAGS = -std=c++11 -O2

%.o : %.cpp
	@$(ECHO) "> compiling: \033[34m$<\033[0m -> \033[01m\033[34m$@\033[0m"
	@$(CXX) $(CFLAGS) -c -o $@ $<

all: objs hw5

objs: $(CSRCS)

hw5: $(COBJS)
	@$(ECHO) "> linking => \033[01m\033[34m$@\033[0m"
	@$(CXX) $(COBJS) -o $@

debug: CFLAGS += -g -O0 -Og
debug: all

clean:
	@rm -f $(COBJS) hw5
