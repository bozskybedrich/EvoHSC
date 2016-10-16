# Jméno preloženého programu
outDir=out
tmpDir=temp
program=maximum

# Seznam objektových souboru použijeme na dvou místech.
OBJ=${tmpDir}/maximum.o ${tmpDir}/MaxInputModule.o ${tmpDir}/ComparatorModule.o

# Míra optimalizace prekladace gcc
#OPT=-I ../tinyxml -I ../simulator -I ../framework -ggdb3 -m32
OPT=-I ../tinyxml -I ../simulator -I ../framework -m32 -O2
OPT_LINKER=-L ../tinyxml/out -L ../simulator/out -L ../framework/out -lframework -lsimulator -ltinyxml -lstdc++

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
	gcc maximum.cpp ${OPT} ${OPT_LINKER} -o ${outDir}/${program}
#	gcc ${OBJ} ${OPT} ${OPT_LINKER} -o ${outDir}/${program}

${tmpDir}/maximum.o: maximum.cpp
	gcc maximum.cpp -c ${OPT} -o ${tmpDir}/maximum.o

${tmpDir}/MaxInputModule.o: MaxInputModule.cpp
	gcc MaxInputModule.cpp -c ${OPT} -o ${tmpDir}/MaxInputModule.o

${tmpDir}/ComparatorModule.o: ComparatorModule.cpp
	gcc ComparatorModule.cpp -c ${OPT} -o ${tmpDir}/ComparatorModule.o
