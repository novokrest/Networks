
CC            = gcc
CXX           = g++
CFLAGS        = -m64 -pipe -g -Wall -W -fPIE 
CXXFLAGS      = -m64 -pipe -g -Wall -W -fPIE
LINK          = g++
MOVE          = mv -f
MKDIR         = mkdir -p

OBJECTS_DIR   = ./bin

SOURCES       = main.c 
OBJECTS       = main.o
TARGET        = icmp


all: $(TARGET)
	

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	${MKDIR} ${OBJECTS_DIR}
	${MOVE} ${OBJECTS} ${OBJECTS_DIR}

main.o: main.c 
	$(CC) -c $(CFLAGS) -o main.o main.c
	
clean:
	rm -rf ${OBJECTS_DIR} icmp

