# Jm�no prelo�en�ho programu
outDir=out
tmpDir=temp
program=sigmoid_plan

# Seznam objektov�ch souboru pou�ijeme na dvou m�stech.
OBJ=${tmpDir}/sigmoid_plan.o

# M�ra optimalizace prekladace gcc
#OPT=-I ../tinyxml -I ../simulator -I ../framework -ggdb3 -m32
OPT=-I ../tinyxml -I ../simulator -I ../framework -m32 -O2
OPT_LINKER=-L ../tinyxml/out -L ../simulator/out -L ../framework/out -L ../modules/out -lmodules -lframework -lsimulator -ltinyxml -lstdc++

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
	gcc ${OBJ} ${OPT} ${OPT_LINKER} -o ${outDir}/${program}
#	gcc ${OBJ} ${OPT} ${OPT_LINKER} -o ${outDir}/${program}

${tmpDir}/sigmoid_plan.o: sigmoid_plan.cpp
	gcc sigmoid_plan.cpp -c ${OPT} -o ${tmpDir}/sigmoid_plan.o
