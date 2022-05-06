# Project: UML Editor
# Course: ICP VUT FIT
# Description: Makefile for UML Editor which uses qMakefile
#
# Authors:
#	Martin Talajka (xtalaj00)
#	Ondrej Kováč (xkovac57)


all: update
	make -f qMakefile

run: all
	./editor

update: project.pro
	qmake -o qMakefile project.pro

doxygen:
	doxygen doc/Doxyfile

clean:
	rm -f editor *.o ui_*.h moc_*.cpp moc_*.h .qmake.stash qMakefile
	rm -rf temp
	rm -rf doc/html
	rm -rf 1-xtalaj00-xkovac57.tar.gz

pack:
	tar -czf 1-xtalaj00-xkovac57.tar.gz doc/Doxyfile examples src README.txt Makefile project.pro