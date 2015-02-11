objects=example.o RK.o
ccflag=-Wall -g
cc=g++
Example:$(objects)
	$(cc) $(ccflag) $(objects) -o exam
$(objects):%.o:%.cc
	$(cc) $(ccflag) -c $< -o $@
clean:
	rm *o
