EXEC=bjcalc
GG=g++
FLAGS= -pedantic -Wall -Wextra -MMD -Wno-unused-parameter -O3

SRCS = $(wildcard *.cc)
OBJECTS = $(SRCS:.cc=.o)
DEPENDS = $(OBJECTS:.o=.d)

${EXEC}: ${OBJECTS}
	${GG} ${FLAGS} ${OBJECTS} -o ${EXEC}

%.o: %.cc
	${GG} ${FLAGS} -c $<

-include ${DEPENDS}

.PHONY: clean

clean:
	rm *.o *.d ${EXEC}
