CURDIR  = ./
SRCDIR  = ./src/
OBJDIR  = ./obj/

SUFFIXES = .o .c .h
.SUFFIXES: $(SUFFIXES)

PROJ = index

CFILES = $(SRCDIR)indexT2.c $(SRCDIR)out-key.c $(SRCDIR)readsent.c
OBJS = $(OBJDIR)indexT2.o $(OBJDIR)out-key.o $(OBJDIR)readsent.o

CC= cc
CFLAGS= -O3

all: index

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
index: $(OBJS) ./libindex.a
	$(CC) -o ../exe/indexT2 $(OBJS) -L. ./libindex.a

$(OBJDIR)indexT2.o: $(SRCDIR)indexT2.c
	$(CC) $(CFLAGS) -c $(SRCDIR)indexT2.c -o $(OBJDIR)indexT2.o
$(OBJDIR)out-key.o: $(SRCDIR)out-key.c
	$(CC) $(CFLAGS) -c $(SRCDIR)out-key.c -o $(OBJDIR)out-key.o
$(OBJDIR)readsent.o: $(SRCDIR)readsent.c
	$(CC) $(CFLAGS) -c $(SRCDIR)readsent.c -o $(OBJDIR)readsent.o
