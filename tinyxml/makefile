# Jméno preloženého programu
outDir=out
tmpDir=temp
program=libtinyxml.a

# Seznam objektových souboru použijeme na dvou místech.
OBJ=${tmpDir}/ticpp.o ${tmpDir}/tinystr.o ${tmpDir}/tinyxml.o ${tmpDir}/tinyxmlerror.o ${tmpDir}/tinyxmlparser.o

# Míra optimalizace prekladace gcc
#OPT=-ggdb3 -m32
OPT=-O2 -m32

# Cílum build, install, uninstall, clean a distrib neodpovídá prímo žádný soubor

.PHONY: build
.PHONY: install
.PHONY: uninstall
.PHONY: clean
.PHONY: distrib

# První cíl je implicitní, není treba volat 'make build', stací 'make'.
# Cíl build nemá žádnou akci, jen závislost.

build: ${program}

# install závisí na preložení projektu, volat ho muže jen root
#install: build
#	cp ${program} /usr/bin

# uninstall má jenom akci a žádnou závislost, volat ho muže jen root
#uninstall:
#	rm -f /usr/bin/${program}

# clean smaže soubory po prekladu
clean:
	rm -f ${tmpDir}/*.o ${program}

# distrib vytvorí balícek s kompletním zdrojovým kódem

# akce na dva rádky se napí‘e pomocí zpetného lomítka

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
