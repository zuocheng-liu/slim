POCO=${HOME}/local/poco
SLIM=${HOME}/local/slim
CXX=g++
CXXLD=${CXX} -fPIC -shared -o 
BASE_DIR= .
BASE_INC= ${BASE_DIR}/include

INC= -I${BASE_INC} \
	 -I${SLIM}/include \
	 -I${POCO}/include

LIBDIR= -L${SLIM}/lib \
		-L${POCO}/lib

LIB= -lPocoUtil \
	 -lPocoNet \
	 -lPocoFoundation \
	 -lSlim

FLAG= ${LIBDIR} ${LIB} ${INC}


TARGET=cgi

SRCS=$(wildcard src/*.cpp)
SRCSNOTDIR=$(notdir $(SRCS))
OBJS=$(patsubst %.cpp, compile/%.o, $(SRCSNOTDIR)) 

OBJS += main.o

$(TARGET):$(OBJS)
	${CXX} -o $@ $^ ${FLAG} 

compile/%.o:src/%.cpp
	${CXX} -o $@ -c $< ${FLAG} 

%.o:%.cpp
	${CXX} -o $@ -c $< ${FLAG} 

time: TimeServer.o
	${CXX} -o $@ $^ ${FLAG} 

clean :
	rm -rf $(TARGET) $(OBJS) 

.PHONY: clean test
