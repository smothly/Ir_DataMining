CURDIR  = ./
SRCDIR  = ./src/
OBJDIR  = ./obj/

SUFFIXES = .o .c .h
.SUFFIXES: $(SUFFIXES)

PROJ = index

CFILES = $(SRCDIR)indexQ.c $(SRCDIR)readsent.c
OBJS = $(OBJDIR)indexQ.o $(OBJDIR)readsent.o

CC= cc
CFLAGS= -O3

all: index

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
index: $(OBJS) ./libindex.a
	$(CC) -o ../exe/indexQ $(OBJS) -L. ./libindex.a

$(OBJDIR)indexQ.o: $(SRCDIR)indexQ.c
	$(CC) $(CFLAGS) -c $(SRCDIR)indexQ.c -o $(OBJDIR)indexQ.o
$(OBJDIR)readsent.o: $(SRCDIR)readsent.c
	$(CC) $(CFLAGS) -c $(SRCDIR)readsent.c -o $(OBJDIR)readsent.o
