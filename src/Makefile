CXX=g++
POCO=${HOME}/local/poco
INC= -I${POCO}/include
LIBDIR=-L${POCO}/lib
LIB=-lPocoNet -lPocoUtil -lPocoFoundation
FLAG= ${INC} ${LIB} ${LIBDIR}

timeServer: HTTPTimeServer.o 
	${CXX} ${FLAG} -o $@ $^

HTTPTimeServer.o: HTTPTimeServer.cpp
	${CXX} ${FLAG} -c $<

clean :
	rm -f *.o 

.PHONY: clean

