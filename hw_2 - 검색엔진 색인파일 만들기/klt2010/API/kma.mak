CURDIR  = ./
SRCDIR  = ./kma/src/
OBJDIR  = ../obj/

SUFFIXES = .o .c .h
.SUFFIXES: $(SUFFIXES)

PROJ = ham

CFILES = $(SRCDIR)kma.c $(SRCDIR)out-ham.c $(SRCDIR)out-tok.c \
	$(SRCDIR)readsent.c $(SRCDIR)err-code.c
OBJS = $(OBJDIR)kma.o $(OBJDIR)out-ham.o $(OBJDIR)out-tok.o \
	$(OBJDIR)readsent.o $(OBJDIR)err-code.o

CC= gcc
CFLAGS= -O3 -Wall

all: ham

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
ham: $(OBJS) ../libindex-TEST.a
	$(CC) -o ../EXE/kma $(OBJS) -L.. ../libindex-TEST.a

$(OBJDIR)kma.o: $(SRCDIR)kma.c
	$(CC) $(CFLAGS) -c $(SRCDIR)kma.c -o $(OBJDIR)kma.o
$(OBJDIR)out-ham.o: $(SRCDIR)out-ham.c
	$(CC) $(CFLAGS) -c $(SRCDIR)out-ham.c -o $(OBJDIR)out-ham.o
$(OBJDIR)out-tok.o: $(SRCDIR)out-tok.c
	$(CC) $(CFLAGS) -c $(SRCDIR)out-tok.c -o $(OBJDIR)out-tok.o
$(OBJDIR)readsent.o: $(SRCDIR)readsent.c
	$(CC) $(CFLAGS) -c $(SRCDIR)readsent.c -o $(OBJDIR)readsent.o
$(OBJDIR)err-code.o: $(SRCDIR)err-code.c
	$(CC) $(CFLAGS) -c $(SRCDIR)err-code.c -o $(OBJDIR)err-code.o
