TARGET := data.c

EXTENSION := c
CC := gcc

INCLUDEDIR := include
OBJDIR := obj
SRCDIR := src
BINDIR := bin
LIBDIR := lib

SRCFILES := $(shell find $(SRCDIR) -type f \( -iname "*.$(EXTENSION)" \) -exec basename \{} \;)
HEADERFILES := $(cd.. & shell find $(INCLUDEDIR) -type f \( -iname "*.h" \) -exec basename \{} \;)
OBJFILES := $(SRCFILES:%.$(EXTENSION)=%.o)

FLAGS :=
BASEFLAGS :=
DEBUGFLAGS := $(BASEFLAGS) -g -Wall
RELEASEFLAGS := $(BASEFLAGS) -O2



debug: FLAGS = $(DEBUGFLAGS)
debug: clean setup $(TARGET)

release: FLAGS = $(RELEASEFLAGS)
release: clean setup $(TARGET)

#compile target
$(TARGET): $(OBJFILES)
	$(CC) $(FLAGS) $(addprefix $(OBJDIR)/,$^) -o $(addprefix $(BINDIR)/,$@)


#compile object files
%.o: $(SRCDIR)/%.$(EXTENSION)
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $^ -I $(INCLUDEDIR)
%.o: $(SRCDIR)/**/%.$(EXTENSION)
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $^ -I $(INCLUDEDIR)


#clean directory
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf $(LIBDIR)


#setup directory
setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)


#print makefile info
info:
	@echo TARGET = $(TARGET)
	@echo EXTENSION = $(EXTENSION)
	@echo INCLUDEDIR = $(INCLUDEDIR)
	@echo OBJDIR = $(OBJDIR)
	@echo SRCDIR = $(SRCDIR)
	@echo BINDIR = $(BINDIR)
	@echo LIBDIR = $(LIBDIR)
	@echo SRCFILES = $(SRCFILES)
	@echo HEADERFILES = $(HEADERFILES)
	@echo DEBUGFLAGS = $(DEBUGFLAGS)
	@echo RELEASEFLAGS = $(RELEASEFLAGS)
	@echo CC = $(CC)

test_data:
	$(CC) $(DEBUGFLAGS) -o obj/data.o -c src/data.c -I $(INCLUDEDIR) && $(CC) $(DEBUGFLAGS) tests/test_data.c -o bin/test_data obj/data.o -I $(INCLUDEDIR)

test_data_run:
	$(test_data)
	./bin/test_data

test_entry:
	$(test_data)
	gcc -g -Wall -o obj/entry.o -c src/entry.c -I include && gcc -g -Wall tests/test_entry.c -o bin/test_entry obj/data.o obj/entry.o -Iinclude

test_entry_run:
	$(test_entry)
	./bin/test_entry
