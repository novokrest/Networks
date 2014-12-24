CXX = g++
CXXFLAGS = -c -std=c++11 -Wall -W
LDFLAGS = 
INCPATH = -I. -I../common
SOURCES = s_main.cpp server.cpp ../common/request.cpp ../common/response.cpp ../common/logger.cpp ../common/utils.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = server

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -f ../common/*.o *.o server
