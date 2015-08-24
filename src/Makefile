CXX=g++
BASE_DIR= ..
BASE_INC= ${BASE_DIR}/include

POCO=${HOME}/local/poco
INC= -I${POCO}/include -I${BASE_INC}
LIBDIR=-L${POCO}/lib
LIB=-lPocoNet -lPocoUtil -lPocoFoundation
FLAG= ${INC} ${LIB} ${LIBDIR}

TARGET=slim
SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o) 

$(TARGET):$(OBJS)
	${CXX} ${FLAG} -o $@ $^

%.o:%.c
	${CXX} ${FLAG} -o $@ -c $<

clean :
	rm -f *.o slim

.PHONY: clean

