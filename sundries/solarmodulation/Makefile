LIST = spectrum sym_solar_mod ffd_solar_mod solar_mod RK interp gfunction
HLIST = oformat

CFLAG = -std=c++0x -Wall -g -O2

OBJ = $(foreach n,$(LIST),$(n).o)

all:libsolar
libsolar:$(OBJ)
	ar crs libsolar.a $(OBJ)
$(OBJ):%.o:%.cc
	g++ $(CFLAG) $(INC_PATH) -c -fPIC $(notdir $<) -o $(notdir $@)
clean:
	rm $(OBJ) libsolar.a
