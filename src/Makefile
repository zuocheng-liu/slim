CXX=g++
BASE_DIR= ..
BASE_INC= ${BASE_DIR}/include

POCO=${HOME}/local/poco
INC= -I${POCO}/include -I${BASE_INC}
LIBDIR=-L${POCO}/lib
LIB=-lPocoNet -lPocoUtil -lPocoFoundation
FLAG= ${INC} ${LIB} ${LIBDIR}

slim: main.o 
	${CXX} ${FLAG} -o $@ $^

main.o: main.cpp
	${CXX} ${FLAG} -c $<

clean :
	rm -f *.o slim

.PHONY: clean

