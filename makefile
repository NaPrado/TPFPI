COMPILER=gcc
OUTPUT_FILE=bikeSharingMON.out
FRONT=bikeSharingMON.c

all: programa clean

programa: $(FRONT) stationADT.o read.o htmlTable.o
	$(COMPILER) -o $(OUTPUT_FILE) $(FRONT) stationADT.o read.o htmlTable.o

stationADT.o: stationADT.c
	$(COMPILER) -c stationADT.c

read.o: read.c
	$(COMPILER) -c read.c

htmlTable.o: htmlTable.c
	$(COMPILER) -c htmlTable.c

clean:
	rm -f *.o
