# Jm�no prelo�en�ho programu
outDir=out
tmpDir=temp
program=libsimulator.a

# Seznam objektov�ch souboru pou�ijeme na dvou m�stech.
OBJ=${tmpDir}/adder.o ${tmpDir}/architecture.o ${tmpDir}/environment.o ${tmpDir}/instrdecoder.o ${tmpDir}/module.o ${tmpDir}/multiplier.o

# M�ra optimalizace prekladace gcc
#OPT=-I ../tinyxml -ggdb3 -m32
OPT=-I ../tinyxml -m32 -O2

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
	gzip c17.tar

${program}: ${OBJ}
	ar cr ${outDir}/${program} ${OBJ}

${tmpDir}/adder.o: ../modules/adder.cpp ../modules/adder.h
	gcc ../modules/adder.cpp -c ${OPT} -o ${tmpDir}/adder.o

${tmpDir}/architecture.o: architecture.cpp architecture.h
	gcc architecture.cpp -c ${OPT} -o ${tmpDir}/architecture.o

${tmpDir}/environment.o: environment.cpp environment.h
	gcc environment.cpp -c ${OPT} -o ${tmpDir}/environment.o

${tmpDir}/instrdecoder.o: instrdecoder.cpp instrdecoder.h
	gcc instrdecoder.cpp -c ${OPT} -o ${tmpDir}/instrdecoder.o

${tmpDir}/main.o: main.cpp
	gcc main.cpp -c ${OPT} -o ${tmpDir}/main.o

${tmpDir}/module.o: ../modules/module.cpp ../modules/module.h
	gcc ../modules/module.cpp -c ${OPT} -o ${tmpDir}/module.o

${tmpDir}/multiplier.o: ../modules/multiplier.cpp ../modules/multiplier.h
	gcc ../modules/multiplier.cpp -c ${OPT} -o ${tmpDir}/multiplier.o

${tmpDir}/simulator.o: simulator.cpp simulator.h
	gcc simulator.cpp -c ${OPT} -o ${tmpDir}/simulator.o
