CURDIR  = ./
SRCDIR  = ./index/src-Test/
OBJDIR  = ../obj/

SUFFIXES = .o .c .h
.SUFFIXES: $(SUFFIXES)

PROJ = ham

CFILES = $(SRCDIR)cnn-test.c
OBJS = $(OBJDIR)cnn-test.o

CC= gcc
CFLAGS= -Wall

all: ham

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
ham: $(OBJS) ../libindex-TEST.a
	$(CC) -o ../EXE/cnn $(OBJS) -L.. ../libindex-TEST.a

$(OBJDIR)cnn-test.o: $(SRCDIR)cnn-test.c
	$(CC) $(CFLAGS) -c $(SRCDIR)cnn-test.c -o $(OBJDIR)cnn-test.o
