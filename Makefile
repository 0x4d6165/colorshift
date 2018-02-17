FLAGS   = -std=c99 -Wall -lpcre
SOURCES = colorshift.c
OBJECTS = colorshift.o
EXEBIN  = colorshift

all: $(EXEBIN)

$(EXEBIN): $(OBJECTS)
	gcc -o $(EXEBIN) $(OBJECTS)

$(OBJECTS): $(SOURCES)
	gcc -c $(FLAGS) $(SOURCES)

clean:
	rm $(EXEBIN) $(OBJECTS)
