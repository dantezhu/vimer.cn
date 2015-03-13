CXX = g++

TARGET = py

C_FLAGS += -ldl -g -Wall -pthread

INC = -I /usr/local/include/python2.7/
LIB = -L /usr/local/lib/ -lpython2.7 -L/usr/lib/ -lssl -lcrypto 

all: $(TARGET)

py:  py.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp

clean:
	-rm -f *.o $(TARGET)
