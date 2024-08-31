LIBRARIES = -lwsock32 -lwinhttp -lwininet -lgdi32 -lcomctl32 -Ilibvlc\\include -Llibvlc -lvlc
BUILDFLAGS = $(LIBRARIES) -municode --static -Og
RELEASE_BUILD_FLAGS = $(LIBRARIES) -municode -mwindows --static -Og
DEBUG_BUILD_FLAGS = $(LIBRARIES) -municode --static -Og
UNIT_TEST_FLAGS = -DUNIT_TEST $(LIBRARIES) -municode -mconsole --static -Og

debug: main.o connections.o file.o json.o provider.o video.o wmcreate.o
	gcc main.o connections.o file.o json.o provider.o video.o wmcreate.o resources.o -o debug_dango.exe $(DEBUG_BUILD_FLAGS)

release: main.o connections.o file.o json.o provider.o video.o wmcreate.o 
	gcc main.o connections.o file.o json.o provider.o video.o wmcreate.o resources.o -o dango.exe $(RELEASE_BUILD_FLAGS) 

test: test.o main.o connections.o file.o json.o provider.o video.o wmcreate.o
	gcc test.o main.o connections.o file.o json.o provider.o video.o wmcreate.o -o test.exe $(UNIT_TEST_FLAGS) 

main.o: main.c
	gcc $(BUILDFLAGS) -c main.c

connections.o: connections.c
	gcc $(BUILDFLAGS) -c connections.c

file.o: file.c
	gcc $(BUILDFLAGS) -c file.c

json.o: json.c
	gcc $(BUILDFLAGS) -c json.c

provider.o: provider.c
	gcc $(BUILDFLAGS) -c provider.c

video.o: video.c
	gcc $(BUILDFLAGS) -c video.c

wmcreate.o: wmcreate.c
	gcc $(BUILDFLAGS) -c wmcreate.c

test.o: wmcreate.c
	gcc $(BUILDFLAGS) -c test.c

clean: 
	@echo "Removing all .o files"
	del *.o 