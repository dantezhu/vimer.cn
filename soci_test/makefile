CC = gcc
CXX = g++

MYFLAGS = -MMD -g -Wall -pthread

INC+=-I /usr/local/include/soci/ -I /usr/local/mysql/include/mysql/ -I/usr/local/include
LIB+= /usr/local/lib/libsoci_mysql.a /usr/local/lib/libsoci_core.a /usr/local/mysql/lib/mysql/libmysqlclient.a -lgtest_main -lgtest -ldl -lz

TAR_DIR = ./
SRC_DIR = ./
OBJ_DIR = ./

TARGET = $(TAR_DIR)test

OBJS = \
	   $(OBJ_DIR)test.o \


OBJ_SUB_DIRS = $(sort $(dir $(OBJS)))

DEPENDS = $(patsubst %.o, %.d, $(OBJS))

.PHONY: all clean init

all: init $(TARGET)
	@echo -e "\033[1;32m\nSuccess!\033[0m"

$(TARGET): $(OBJS)
	@echo -e "\033[1;33m\nLinking $< ==> $@\033[0m"
	$(CXX) -o $@ $^ $(LIB) $(MYFLAGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CC) $(MYFLAGS) -c -o $@ $(INC) $<

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CXX) $(MYFLAGS) -c -o $@ $(INC) $<

clean:
	rm -f $(OBJS) $(TARGET) $(DEPENDS)

init:
	mkdir -p $(sort ./ $(TAR_DIR) $(SRC_DIR) $(OBJ_DIR) $(OBJ_SUB_DIRS))

-include $(DEPENDS)
