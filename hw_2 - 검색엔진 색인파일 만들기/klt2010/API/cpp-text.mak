CURDIR  = ./
SRCDIR  = ./index/src/
OBJDIR  = ../obj/

SUFFIXES = .o .cpp .h
.SUFFIXES: $(SUFFIXES)

PROJ = index

CFILES = $(SRCDIR)indexT.cpp
OBJS = $(OBJDIR)indexT.o

CC= g++
CFLAGS= -O3 -Wall

all: index

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
index: $(OBJS) ../libindex.a
	$(CC) -o ../EXE/indexcpp $(OBJS) -L.. ../libindex.a

$(OBJDIR)indexT.o: $(SRCDIR)indexT.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)indexT.cpp -o $(OBJDIR)indexT.o
