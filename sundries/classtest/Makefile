cc = g++
CFLAG = -g
OBJ = test.o Interp2D.o Table2D.o
LIB = -llin
test:$(OBJ)
	$(cc) $(CFLAG) -o test $(OBJ) $(LIB)
$(OBJ):%.o:%.cc
	$(cc) $(CFLAG) -c $< -o $@
clean:
	rm *o
