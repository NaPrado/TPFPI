COMPILER = gcc
OUTPUT_FILE = bikeSharing.out
FRONT = bikeSharing.c
CFLAGS = -Wall -pedantic -std=c99 -fsanitize=address -g
FORMATFLAG = -DFORMATMON\=1

all: programa clean

programa: $(FRONT) stationADT.o read.o htmlTable.o treeADT.o querysADT.o writeCSV.o
	$(COMPILER) stationADT.o read.o treeADT.o querysADT.o htmlTable.o writeCSV.o $(FRONT) -o $(OUTPUT_FILE) $(CFLAGS)

stationADT.o: stationADT.c
	$(COMPILER) $(CFLAGS) -c stationADT.c

read.o: read.c
	$(COMPILER) $(FORMATFLAG) $(CFLAGS) -c read.c

treeADT.o: treeADT.c
	$(COMPILER) $(CFLAGS) -c treeADT.c

querysADT.o: querysADT.c
	$(COMPILER) $(CFLAGS) -c querysADT.c

htmlTable.o: htmlTable.c
	$(COMPILER) $(CFLAGS) -c htmlTable.c

writeCSV.o: writeCSV.c
	$(COMPILER) $(CFLAGS) -c writeCSV.c

clean:
	rm -f *.o
