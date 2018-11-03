CURDIR  = ./
SRCDIR  = ./kma/src/
OBJDIR  = ../obj/

SUFFIXES = .o .cpp .h
.SUFFIXES: $(SUFFIXES)

PROJ = ham

CFILES = $(SRCDIR)kma.cpp $(SRCDIR)out-ham.cpp $(SRCDIR)out-tok.cpp \
	$(SRCDIR)readsent.cpp $(SRCDIR)err-code.cpp
OBJS = $(OBJDIR)kma.o $(OBJDIR)out-ham.o $(OBJDIR)out-tok.o \
	$(OBJDIR)readsent.o $(OBJDIR)err-code.o

CC= g++
CFLAGS= -O3 -Wall

all: ham

tag: $(HDRS) $(CFILES)
	ctags $(HDRS) $(CFILES)
ham: $(OBJS) ../libindex.a
	$(CC) -o ../EXE/kmacpp $(OBJS) -L.. ../libindex.a

$(OBJDIR)kma.o: $(SRCDIR)kma.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)kma.cpp -o $(OBJDIR)kma.o
$(OBJDIR)out-ham.o: $(SRCDIR)out-ham.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)out-ham.cpp -o $(OBJDIR)out-ham.o
$(OBJDIR)out-tok.o: $(SRCDIR)out-tok.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)out-tok.cpp -o $(OBJDIR)out-tok.o
$(OBJDIR)readsent.o: $(SRCDIR)readsent.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)readsent.cpp -o $(OBJDIR)readsent.o
$(OBJDIR)err-code.o: $(SRCDIR)err-code.cpp
	$(CC) $(CFLAGS) -c $(SRCDIR)err-code.cpp -o $(OBJDIR)err-code.o
