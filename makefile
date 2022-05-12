BUILDDIR=build
OUTPUTDIR=AlgoEVA

.PHONY: clean createBuildDir

all : $(OUTPUTDIR)/AlgoEVA

createBuildDir :
	@if [ -d $(BUILDDIR) ]; then echo "Build dir already exist"; else mkdir $(BUILDDIR); fi

$(BUILDDIR)/main.o : createBuildDir main.c header.h
	@gcc -c main.c -o $@

$(OUTPUTDIR)/AlgoEVA : $(BUILDDIR)/main.o
	@if [ -d $(OUTPUTDIR) ]; then echo "Output dir already exist"; else mkdir $(OUTPUTDIR); fi
	@gcc $^ -o $@
	@echo 'You can run the applcation in the folder $(OUTPUTDIR) by typing ./AlgoEVA in the therminal.'
	
clean :
	@if [ -d $(BUILDDIR) ]; then rm -r $(BUILDDIR); else echo "Build dir already remove"; fi
	@if [ -d $(OUTPUTDIR) ]; then rm -r $(OUTPUTDIR); else echo "Output dir already remove"; fi
