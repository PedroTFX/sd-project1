TARGET := example

EXTENSION := cpp
CC := gcc

INCLUDEDIR := include
OBJDIR := obj
SRCDIR := src
BINDIR := bin
LIBDIR := lib
ASMDIR := asm

SRCFILES := $(shell find $(SRCDIR) -type f \( -iname "*.$(EXTENSION)" \) -exec basename \{} \;)
HEADERFILES := $(shell find $(INCLUDEDIR) -type f \( -iname "*.h" \) -exec basename \{} \;)
OBJFILES := $(SRCFILES:%.$(EXTENSION)=%.o)

MACROS := MAKE
BASEFLAGS := $(addprefix -D ,$(MACROS))
DEBUGFLAGS := $(BASEFLAGS) -g -Wall
RELEASEFLAGS := $(BASEFLAGS) -O2

FLAGS :=
A :=

debug: FLAGS = $(DEBUGFLAGS)
debug: clean setup $(TARGET)

release: FLAGS = $(RELEASEFLAGS)
release: clean setup $(TARGET)

#build target
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
	@echo ASMDIR = $(ASMDIR)
	@echo SRCFILES = $(SRCFILES)
	@echo HEADERFILES = $(HEADERFILES)
	@echo MACROS = $(MACROS)
	@echo DEBUGFLAGS = $(DEBUGFLAGS)
	@echo RELEASEFLAGS = $(RELEASEFLAGS)
	@echo CC = $(CC)


#give execution permissions
permissions:
	chmod a+x $(BINDIR)/$(TARGET)


#make assembly files
%.s: $(SRCDIR)/%.$(EXTENSION)
	$(CC) $(FLAGS) -o $(addprefix $(ASMDIR)/,$@) -S $^ -I $(INCLUDEDIR)
%.s: $(SRCDIR)/**/%.$(EXTENSION)
	$(CC) $(FLAGS) -o $(addprefix $(ASMDIR)/,$@) -S $^ -I $(INCLUDEDIR)


#create delete files
c:
	mkdir -p $(shell dirname $(SRCDIR)/$(A).$(EXTENSION))
	touch $(SRCDIR)/$(A).$(EXTENSION)
	mkdir -p $(shell dirname $(INCLUDEDIR)/$(A).$(EXTENSION))
	touch $(INCLUDEDIR)/$(A).h
r:
	rm $(SRCDIR)/$(A).$(EXTENSION)
	rm $(INCLUDEDIR)/$(A).h


#exec with std args
exec:
	@$(BINDIR)/$(TARGET) $$(cat args.txt)


#run valgrind
valgrind:
	valgrind --leak-check=full $(BINDIR)/$(TARGET)


#get todo list
todo:
	@grep -R TODO -n | tr -s ' ' | grep -v makefile

test_data:
	$(CC) $(DEBUGFLAGS) -o obj/data.o -c src/data.c -I $(INCLUDEDIR) && $(CC) $(DEBUGFLAGS) tests/test_data.c -o bin/test_data obj/data.o -I $(INCLUDEDIR)

test_data_run: test_data
	./bin/test_data

test_entry: test_data_run
	$(CC) $(DEBUGFLAGS) -o obj/entry.o -c src/entry.c -I $(INCLUDEDIR) && $(CC) $(DEBUGFLAGS) tests/test_entry.c -o bin/test_entry obj/data.o obj/entry.o -I $(INCLUDEDIR)

test_entry_run: test_entry
	./bin/test_entry

test_tree: test_entry_run
	$(CC) $(DEBUGFLAGS) -o obj/tree.o -c src/tree.c -I $(INCLUDEDIR) && $(CC) $(DEBUGFLAGS) tests/test_tree.c -o bin/test_tree obj/data.o obj/entry.o obj/tree.o -I $(INCLUDEDIR)

test_tree_run: test_tree
	./bin/test_tree

