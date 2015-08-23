CXX=g++
POCO=${HOME}/local/poco
INC= -I${POCO}/include
LIBDIR=-L${POCO}/lib
LIB=-lPocoNet -lPocoUtil -lPocoFoundation
FLAG= ${INC} ${LIB} ${LIBDIR}

timeServer: main.o 
	${CXX} ${FLAG} -o $@ $^

main.o: main.cpp
	${CXX} ${FLAG} -c $<

clean :
	rm -f *.o 

.PHONY: clean

