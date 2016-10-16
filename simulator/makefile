# Jméno preloženého programu
outDir=out
tmpDir=temp
program=libsimulator.a

# Seznam objektových souboru použijeme na dvou místech.
OBJ=${tmpDir}/adder.o ${tmpDir}/architecture.o ${tmpDir}/environment.o ${tmpDir}/instrdecoder.o ${tmpDir}/module.o ${tmpDir}/multiplier.o

# Míra optimalizace prekladace gcc
#OPT=-I ../tinyxml -ggdb3 -m32
OPT=-I ../tinyxml -m32 -O2

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

${tmpDir}/adder.o: adder.cpp adder.h
	gcc adder.cpp -c ${OPT} -o ${tmpDir}/adder.o

${tmpDir}/architecture.o: architecture.cpp architecture.h
	gcc architecture.cpp -c ${OPT} -o ${tmpDir}/architecture.o

${tmpDir}/environment.o: environment.cpp environment.h
	gcc environment.cpp -c ${OPT} -o ${tmpDir}/environment.o

${tmpDir}/instrdecoder.o: instrdecoder.cpp instrdecoder.h
	gcc instrdecoder.cpp -c ${OPT} -o ${tmpDir}/instrdecoder.o

${tmpDir}/main.o: main.cpp
	gcc main.cpp -c ${OPT} -o ${tmpDir}/main.o

${tmpDir}/module.o: module.cpp module.h
	gcc module.cpp -c ${OPT} -o ${tmpDir}/module.o

${tmpDir}/multiplier.o: multiplier.cpp multiplier.h
	gcc multiplier.cpp -c ${OPT} -o ${tmpDir}/multiplier.o

${tmpDir}/simulator.o: simulator.cpp simulator.h
	gcc simulator.cpp -c ${OPT} -o ${tmpDir}/simulator.o
