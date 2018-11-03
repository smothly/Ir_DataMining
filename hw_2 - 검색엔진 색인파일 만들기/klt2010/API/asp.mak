CURDIR  = ./
SRCDIR  = ./index/src-Test/
OBJDIR  = ../obj/

SUFFIXES = .o .c .h
.SUFFIXES: $(SUFFIXES)

PROJ = ham

CFILES = $(SRCDIR)asp-test.c
OBJS = $(OBJDIR)asp-test.o

CC= gcc
CFLAGS= -Wall

all: ham

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
ham: $(OBJS) ../libindex-TEST.a
	$(CC) -o ../EXE/asp $(OBJS) -L.. ../libindex-TEST.a

$(OBJDIR)asp-test.o: $(SRCDIR)asp-test.c
	$(CC) $(CFLAGS) -c $(SRCDIR)asp-test.c -o $(OBJDIR)asp-test.o
