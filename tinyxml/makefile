# Jm�no prelo�en�ho programu
outDir=out
tmpDir=temp
program=libtinyxml.a

# Seznam objektov�ch souboru pou�ijeme na dvou m�stech.
OBJ=${tmpDir}/ticpp.o ${tmpDir}/tinystr.o ${tmpDir}/tinyxml.o ${tmpDir}/tinyxmlerror.o ${tmpDir}/tinyxmlparser.o

# M�ra optimalizace prekladace gcc
#OPT=-ggdb3 -m32
OPT=-O2 -m32

# C�lum build, install, uninstall, clean a distrib neodpov�d� pr�mo ��dn� soubor

.PHONY: build
.PHONY: install
.PHONY: uninstall
.PHONY: clean
.PHONY: distrib

# Prvn� c�l je implicitn�, nen� treba volat 'make build', stac� 'make'.
# C�l build nem� ��dnou akci, jen z�vislost.

build: ${program}

# install z�vis� na prelo�en� projektu, volat ho mu�e jen root
#install: build
#	cp ${program} /usr/bin

# uninstall m� jenom akci a ��dnou z�vislost, volat ho mu�e jen root
#uninstall:
#	rm -f /usr/bin/${program}

# clean sma�e soubory po prekladu
clean:
	rm -f ${tmpDir}/*.o ${program}

# distrib vytvor� bal�cek s kompletn�m zdrojov�m k�dem

# akce na dva r�dky se nap�e pomoc� zpetn�ho lom�tka

#distrib:
#	tar -c funkce.c main.c funkce.h Makefile > c17.tar; \
#	gzip c17.tar

${program}: ${OBJ}
	ar cr ${outDir}/${program} ${OBJ}

OBJ=${tmpDir}/ticpp.o ${tmpDir}/tinystr.o ${tmpDir}/tinyxml.o ${tmpDir}/tinyxmlerror.o ${tmpDir}/tinyxmlparser.o

${tmpDir}/ticpp.o: ticpp.cpp ticpp.h
	gcc ticpp.cpp -c ${OPT} -o ${tmpDir}/ticpp.o

${tmpDir}/tinystr.o: tinystr.cpp tinystr.h
	gcc tinystr.cpp -c ${OPT} -o ${tmpDir}/tinystr.o

architecture.o: architecture.cpp architecture.h
	gcc architecture.cpp -c ${OPT} -o ${tmpDir}/architecture.o

${tmpDir}/tinyxml.o: tinyxml.cpp tinyxml.h
	gcc tinyxml.cpp -c ${OPT} -o ${tmpDir}/tinyxml.o

${tmpDir}/tinyxmlerror.o: tinyxmlerror.cpp
	gcc tinyxmlerror.cpp -c ${OPT} -o ${tmpDir}/tinyxmlerror.o

${tmpDir}/tinyxmlparser.o: tinyxmlparser.cpp
	gcc tinyxmlparser.cpp -c ${OPT} -o ${tmpDir}/tinyxmlparser.o
