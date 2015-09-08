POCO=${HOME}/local/poco
CXX=g++
BASE_DIR= ..
BASE_INC= ${BASE_DIR}/include

INC= -I${POCO}/include -I${BASE_INC}
LIBDIR=-L${POCO}/lib
LIB=-lPocoUtil -lPocoNet -lPocoFoundation
FLAG= ${LIBDIR} ${LIB} ${INC}

TARGET=slim
SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o) 

$(TARGET):$(OBJS)
	${CXX} -o $@ $^ ${FLAG}

fastcgi : fastcgi.o
	${CXX} -o $@ $^ ${FLAG}

%.o:%.cpp
	${CXX} -o $@ -c $< ${FLAG}

clean :
	rm -rf $(TARGET) $(OBJS)

.PHONY: clean

