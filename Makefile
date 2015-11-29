POCO=${HOME}/local/poco
CXX=g++
CXXLD=${CXX} -fPIC -shared -o 
BASE_DIR= .
BASE_INC= ${BASE_DIR}/include

INC= -I${BASE_INC} \
	 -I${POCO}/include

LIBDIR= -L${POCO}/lib

LIB= -lPocoUtil \
	 -lPocoNet \
	 -lPocoFoundation

FLAG= ${LIBDIR} ${LIB} ${INC}


TARGET=lib/libSlim.so
SRCS=$(wildcard src/*.cpp)
SRCSNOTDIR=$(notdir $(SRCS))
OBJS=$(patsubst %.cpp, compile/%.o, $(SRCSNOTDIR)) 

.PHONY: clean test

$(TARGET):$(OBJS)
	${CXXLD} $@ $^ ${FLAG} 

compile/%.o:src/%.cpp
	${CXX} -fPIC -o $@ -c $< ${FLAG} 

clean :
	rm -rf $(TARGET) $(OBJS) 

#include testsuite/Makefile
