objects= iteration.o test.o sym_solar_mod.o
objs = iteration.o example.o
ccflag=-Wall -g
lib=-llins
cc=g++
All: Example
test:$(objects)
	$(cc) $(ccflag) $(objects) $(lib) -o test
Example:$(objs)
	$(cc) $(ccflag) $(objs) $(lib) -o exam
$(objects):%.o:%.cc
	$(cc) $(ccflag) -c $< -o $@
clean:
	rm *o
