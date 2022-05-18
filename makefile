#Please remove the build dir and update the .gitignore before changing this value
BUILDDIR=build
#Please remove the output dir and update the .gitignore before changeing this value
OUTPUTDIR=AlgoEVA

.PHONY: clean createBuildDir


all : $(OUTPUTDIR)/AlgoEVA

createBuildDir :
	@if [ -d $(BUILDDIR) ]; then echo "Build dir already exist"; else mkdir $(BUILDDIR); fi

$(BUILDDIR)/main.o : createBuildDir main.c header.h
	@gcc -c main.c -o $@
	
$(BUILDDIR)/gestiondossier.o : createBuildDir gestiondossier.c header.h
	@gcc -c gestiondossier.c -o $@

$(BUILDDIR)/fonctions.o : createBuildDir fonctions.c header.h
	@gcc -c fonctions.c -o $@

$(OUTPUTDIR)/AlgoEVA : $(BUILDDIR)/main.o $(BUILDDIR)/gestiondossier.o $(BUILDDIR)/fonctions.o
	@if [ -d $(OUTPUTDIR) ]; then echo "Output dir already exist"; else mkdir $(OUTPUTDIR); fi
	@gcc $^ -o $@
	@echo 'You can run the applcation in the $(OUTPUTDIR) folder : '
	@echo '- Go in the $(OUTPUTDIR) folder and runing AlgoEVA (cd $(OUTPUTDIR) then ./AlgoEVA)'
	@echo '- Or juste with : ./AlgoEVA/AlgoEVA'
	
clean :
	@if [ -d $(BUILDDIR) ]; then rm -fr $(BUILDDIR); else echo "Build dir already remove"; fi
	@if [ -d $(OUTPUTDIR) ]; then rm -fr $(OUTPUTDIR); else echo "Output dir already remove"; fi
