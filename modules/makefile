# Jm�no prelo�en�ho programu
outDir=out
tmpDir=temp
program=libmodules.a

# Seznam objektov�ch souboru pou�ijeme na dvou m�stech.
OBJ=${tmpDir}/adder.o ${tmpDir}/bit_shift.o ${tmpDir}/ComparatorModule.o ${tmpDir}/ConstantModule.o ${tmpDir}/MaxInputModule.o ${tmpDir}/MedianModule.o ${tmpDir}/module.o ${tmpDir}/Multiplexer.o ${tmpDir}/multiplier.o ${tmpDir}/PixelEvaluator.o ${tmpDir}/PixelInput.o ${tmpDir}/AndOr.o ${tmpDir}/SigmoidInput.o

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

${tmpDir}/bit_shift.o: ../modules/bit_shift.cpp ../modules/bit_shift.h
	gcc ../modules/bit_shift.cpp -c ${OPT} -o ${tmpDir}/bit_shift.o

${tmpDir}/ComparatorModule.o: ../modules/ComparatorModule.cpp ../modules/ComparatorModule.h
	gcc ../modules/ComparatorModule.cpp -c ${OPT} -o ${tmpDir}/ComparatorModule.o

${tmpDir}/ConstantModule.o: ../modules/ConstantModule.cpp ../modules/ConstantModule.h
	gcc ../modules/ConstantModule.cpp -c ${OPT} -o ${tmpDir}/ConstantModule.o

${tmpDir}/MaxInputModule.o: ../modules/MaxInputModule.cpp ../modules/MaxInputModule.h
	gcc ../modules/MaxInputModule.cpp -c ${OPT} -o ${tmpDir}/MaxInputModule.o

${tmpDir}/MedianModule.o: ../modules/MedianModule.cpp ../modules/MedianModule.h
	gcc ../modules/MedianModule.cpp -c ${OPT} -o ${tmpDir}/MedianModule.o

${tmpDir}/module.o: ../modules/module.cpp ../modules/module.h
	gcc ../modules/module.cpp -c ${OPT} -o ${tmpDir}/module.o

${tmpDir}/Multiplexer.o: ../modules/Multiplexer.cpp ../modules/Multiplexer.h
	gcc ../modules/Multiplexer.cpp -c ${OPT} -o ${tmpDir}/Multiplexer.o
	
${tmpDir}/multiplier.o: ../modules/multiplier.cpp ../modules/multiplier.h
	gcc ../modules/multiplier.cpp -c ${OPT} -o ${tmpDir}/multiplier.o

${tmpDir}/PixelEvaluator.o: ../modules/PixelEvaluator.cpp ../modules/PixelEvaluator.h
	gcc ../modules/PixelEvaluator.cpp -c ${OPT} -o ${tmpDir}/PixelEvaluator.o

${tmpDir}/PixelInput.o: ../modules/PixelInput.cpp ../modules/PixelInput.h
	gcc ../modules/PixelInput.cpp -c ${OPT} -o ${tmpDir}/PixelInput.o

${tmpDir}/AndOr.o: ../modules/AndOr.cpp ../modules/AndOr.h
	gcc ../modules/AndOr.cpp -c ${OPT} -o ${tmpDir}/AndOr.o

${tmpDir}/SigmoidInput.o: ../modules/SigmoidInput.cpp ../modules/SigmoidInput.h
	gcc ../modules/SigmoidInput.cpp -c ${OPT} -o ${tmpDir}/SigmoidInput.o