objects=solar_test.o
ccflag=-Wall -g
lib=-llins
cc=g++
Example:$(objects)
	$(cc) $(ccflag) $(objects) $(lib) -o test
$(objects):%.o:%.cc
	$(cc) $(ccflag) -c $< -o $@
clean:
	rm *o
