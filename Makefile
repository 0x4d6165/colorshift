FLAGS   = -std=c99 -Wall
SOURCES = colorshift.c
OBJECTS = colorshift.o
EXEBIN  = colorshift

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin

all: $(EXEBIN)

$(EXEBIN): $(OBJECTS)
	gcc -o $(EXEBIN) $(OBJECTS)

$(OBJECTS): $(SOURCES)
	gcc -c $(FLAGS) $(SOURCES)

clean:
	rm $(EXEBIN) $(OBJECTS)

install: $(EXEBIN)
	install -D $(EXEBIN) $(BINDIR)/$(EXEBIN)
