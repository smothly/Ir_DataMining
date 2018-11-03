CURDIR  = ./
SRCDIR  = ./src/
OBJDIR  = ./obj/

SUFFIXES = .o .c .h
.SUFFIXES: $(SUFFIXES)

PROJ = index

CFILES = $(SRCDIR)indexS.c $(SRCDIR)readsent.c
OBJS = $(OBJDIR)indexS.o $(OBJDIR)readsent.o

CC= gcc
CFLAGS= -O3

all: index

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
index: $(OBJS) ./libindexTEST.a
	$(CC) -o ../exe/index $(OBJS) -L. ./libindexTEST.a

$(OBJDIR)indexS.o: $(SRCDIR)indexS.c
	$(CC) $(CFLAGS) -c $(SRCDIR)indexS.c -o $(OBJDIR)indexS.o
$(OBJDIR)readsent.o: $(SRCDIR)readsent.c
	$(CC) $(CFLAGS) -c $(SRCDIR)readsent.c -o $(OBJDIR)readsent.o
