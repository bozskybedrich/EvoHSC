# Jméno preloženého programu
outDir=out
tmpDir=temp
program=libframework.a

# Seznam objektových souboru použijeme na dvou místech.
OBJ=${tmpDir}/framework.o ${tmpDir}/individual.o

# Míra optimalizace prekladace gcc
#OPT=-I ../tinyxml -I ../simulator -ggdb3 -m32
OPT=-I ../tinyxml -I ../simulator -m32 -O2


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
	gzip c17.tar

${program}: ${OBJ}
	ar cr ${outDir}/${program} ${OBJ}

${tmpDir}/framework.o: framework.cpp framework.h
	gcc framework.cpp -c ${OPT} -o ${tmpDir}/framework.o

${tmpDir}/individual.o: individual.cpp individual.h
	gcc individual.cpp -c ${OPT} -o ${tmpDir}/individual.o
