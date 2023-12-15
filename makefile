COMPILER = gcc
OUTPUT_FILE_NYC = bikeSharingNYC
OUTPUT_FILE_MON = bikeSharingMON
FRONT = bikeSharing.c
CFLAGS = -Wall -pedantic -std=c99 -fsanitize=address -g
FORMATMON = -DFORMATMON\=1
FORMATNYC = -DFORMATNYC\=1

all: programaNYC programaMON clean

programaNYC: $(FRONT) stationADT.o readNYC.o htmlTable.o treeADT.o querysADT.o writeCSV.o
	$(COMPILER) stationADT.o readNYC.o treeADT.o querysADT.o htmlTable.o writeCSV.o $(FRONT) -o $(OUTPUT_FILE_NYC) $(CFLAGS)

programaMON: $(FRONT) stationADT.o readMON.o htmlTable.o treeADT.o querysADT.o writeCSV.o
	$(COMPILER) stationADT.o readMON.o treeADT.o querysADT.o htmlTable.o writeCSV.o $(FRONT) -o $(OUTPUT_FILE_MON) $(CFLAGS)

stationADT.o: stationADT.c
	$(COMPILER) $(CFLAGS) -c stationADT.c

readMON.o: read.c
	$(COMPILER) $(FORMATMON) $(CFLAGS) -c read.c -o readMON.o

readNYC.o: read.c
	$(COMPILER) $(FORMATNYC) $(CFLAGS) -c read.c -o readNYC.o

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
