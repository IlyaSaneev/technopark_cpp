.PHONY: all clean install uninstall

CXX = g++
INCLUDES= -I./3rdparty/cppunit/include
CXXFLAGS=-ggdb3 $(INCLUDES) -std=c++11 -Wall
LINKFLAGS= -lcppunit
SRC_STUDY_LIB = DArray.cpp LinkedList.cpp Queue.cpp \
      	Stack.cpp Heap.cpp

all: prog tests_cppunit
	echo "all target called"

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp

# Full compilation
#prog: $(SRC_STUDY_LIB)
#	$(CXX) $(CXXFLAGS) main.cpp $(SRC_STUDY_LIB) -o prog $(LINKFLAGS)

# Incremental compilation
prog: main.o $(SRC_STUDY_LIB)
	$(CXX) $(CXXFLAGS) main.o $(SRC_STUDY_LIB) -o prog $(LINKFLAGS)

# Incremental compilation phony target
project: $(SRC_STUDY_LIB) main.o
	$(CXX) $(CXXFLAGS) main.o $(SRC_STUDY_LIB) -o prog $(LINKFLAGS)

tests_cppunit:
	$(CXX) $(CXXFLAGS) tests_cppunit.cpp $(SRC_STUDY_LIB) -o tests_cppunit $(LINKFLAGS)

clean:
	rm -rf *.o build/prog build/tests_cppunit

# $make
# $make install

install:
	sudo cp ./build/prog /usr/local/bin

uninstall:
	sudo rm /usr/local/bin/prog




# <цель - что собрать>: <реквизиты - что необходимо> или <зависимости - другие цели>
#	<операция 1>
#	...
#	<операция n>