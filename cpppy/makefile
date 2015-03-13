CXX = g++

TARGET = py

C_FLAGS += -g -Wall

INC = -IC:/Python27/include/
LIB = -LC:/Python27/libs/ -lpython27

all: $(TARGET)

py:  py.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp

clean:
	-rm -f *.o $(TARGET)
