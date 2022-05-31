## E.V.A. C implementation project

--**Quick description**--

This program will help your PPM files take less space on your storage.  
It uses the Ehanced Value Algorithm to compress or decompress multiple images in one go !  
Because we care a lot about customer transparency, our program will tell you precisely how much time its operations take and how much space you save for each image.  

--**How to use**--

1) Download our files on your computer from this repository and put them in a directory of your choosing.
2) Before starting, it is recommended that you create a directory named "ppm" next to the files. No worries if you haven't done it, our program will do it for you !
3) In ppm, put all the PPM images or compressed files that you want to modify.
4) On your terminal, go to your chosen directory and type command "make" to begin.
5) Follow the instructions that appear on screen.
6) When compressing or uncompressing, old files are not removed to make sure no accidents happen.
7) If you ever want to stop using EVA, type "make clean" in your terminal to delete all our executables / object files.
8) Have fun !


--**List of modules**--

main.c :                 The main function.  
fonctions.c :            C file containing all intermediary functions.  
ppm_lib.c / ppm_lib.h :  C package containing intermediary functions used to handle PPM files.  
list.c / list.h :       C package containing intermediary functions used to manipulate linked lists.  
files.c :                C file containing intermediary functions used to manage directories and files.  
header.h :               C header. Contains libraries inclusions, declaration of constants and prototypes.  
makefile :               Our makefile.


Libraries used : stdio.h ; stdlib.h ; dirent.h ; <sys/time.h> ; string.h ; math.h ; errno.h

-Théo Kalifa & Alexandre Grisez \ PréIng1 Groupe 6 MI \ CY TECH
