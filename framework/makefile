# Jm�no prelo�en�ho programu
outDir=out
tmpDir=temp
program=libframework.a

# Seznam objektov�ch souboru pou�ijeme na dvou m�stech.
OBJ=${tmpDir}/framework.o ${tmpDir}/individual.o

# M�ra optimalizace prekladace gcc
#OPT=-I ../tinyxml -I ../simulator -ggdb3 -m32
OPT=-I ../tinyxml -I ../simulator -m32 -O2


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

${tmpDir}/framework.o: framework.cpp framework.h
	gcc framework.cpp -c ${OPT} -o ${tmpDir}/framework.o

${tmpDir}/individual.o: individual.cpp individual.h
	gcc individual.cpp -c ${OPT} -o ${tmpDir}/individual.o
