BUILDDIR=build

all : AlgoEVA

createDir :
	if [ -d $(BUILDDIR) ]; then echo "Build dir already exist"; else mkdir $(BUILDDIR); fi

$(BUILDDIR)/main.o : createDir main.c header.h
	gcc -c main.c -o $@
	
AlgoEVA : $(BUILDDIR)/main.o
	gcc $^ -o $@
	
clean : 
	   rm $(BUILDDIR)/*
