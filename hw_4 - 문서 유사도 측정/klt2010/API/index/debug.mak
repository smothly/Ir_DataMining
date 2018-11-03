CURDIR  = ./
SRCDIR  = ./src/
OBJDIR  = ./obj/

SUFFIXES = .o .c .h
.SUFFIXES: $(SUFFIXES)

PROJ = indexd

CFILES = $(SRCDIR)indexS.c $(SRCDIR)readsent.c
OBJS = $(OBJDIR)indexS.o $(OBJDIR)readsent.o

CC= gcc
CFLAGS= -g

all: indexd

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
indexd: $(OBJS) ./libindexD.a
	$(CC) -o ../exe/indexd $(OBJS) -L. ./libindexD.a

$(OBJDIR)indexS.o: $(SRCDIR)indexS.c
	$(CC) $(CFLAGS) -c $(SRCDIR)indexS.c -o $(OBJDIR)indexS.o
$(OBJDIR)readsent.o: $(SRCDIR)readsent.c
	$(CC) $(CFLAGS) -c $(SRCDIR)readsent.c -o $(OBJDIR)readsent.o
