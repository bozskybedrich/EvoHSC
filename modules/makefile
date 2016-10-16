# Jméno preloženého programu
outDir=out
tmpDir=temp
program=libmodules.a

# Seznam objektových souboru použijeme na dvou místech.
OBJ=${tmpDir}/adder.o ${tmpDir}/bit_shift.o ${tmpDir}/ComparatorModule.o ${tmpDir}/ConstantModule.o ${tmpDir}/MaxInputModule.o ${tmpDir}/MedianModule.o ${tmpDir}/module.o ${tmpDir}/Multiplexer.o ${tmpDir}/multiplier.o ${tmpDir}/PixelEvaluator.o ${tmpDir}/PixelInput.o

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

${tmpDir}/adder.o ${tmpDir}/bit_shift.o ${tmpDir}/ComparatorModule.o ${tmpDir}/ConstantModule.o ${tmpDir}/MaxInputModule.o ${tmpDir}/MedianModule.o ${tmpDir}/module.o ${tmpDir}/Multiplexer.o ${tmpDir}/multiplier.o ${tmpDir}/PixelEvaluator.o ${tmpDir}/PixelInput.o

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
