POCO=${HOME}/local/poco
CXX=g++
BASE_DIR= ..
BASE_INC= ${BASE_DIR}/include

INC= -I${BASE_INC} \
	 -I${POCO}/include
LIBDIR= -L${POCO}/lib
LIB= -lPocoUtil \
	 -lPocoNet \
	 -lPocoFoundation
FLAG= ${LIBDIR} ${LIB} ${INC}

TARGET=slim
SRCS=main.cpp Exception.cpp Handler.cpp Message.cpp Request.cpp Response.cpp Stream.cpp

OBJS=$(SRCS:.cpp=.o) 

$(TARGET):$(OBJS)
	${CXX} -o $@ $^ ${FLAG} -g

fastcgi : fastcgi.o
	${CXX} -o $@ $^ ${FLAG}

%.o:%.cpp
	${CXX} -o $@ -c $< ${FLAG} -g

clean :
	rm -rf $(TARGET) $(OBJS)

.PHONY: clean
