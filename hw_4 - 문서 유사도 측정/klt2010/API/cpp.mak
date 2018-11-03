CURDIR  = ./
SRCDIR  = ./index/src/
OBJDIR  = ../obj/

SUFFIXES = .o .cpp .h
.SUFFIXES: $(SUFFIXES)

PROJ = index

CFILES = $(SRCDIR)indexS.cpp $(SRCDIR)readsent.cpp
OBJS = $(OBJDIR)indexS.o $(OBJDIR)readsent.o

CC= g++
CFLAGS= -O3 -Wall

all: index

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
index: $(OBJS) ../libindex.a
	$(CC) -o ../EXE/indexcpp $(OBJS) -L.. ../libindex.a

$(OBJDIR)indexS.o: $(SRCDIR)indexS.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)indexS.cpp -o $(OBJDIR)indexS.o
$(OBJDIR)readsent.o: $(SRCDIR)readsent.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)readsent.cpp -o $(OBJDIR)readsent.o
