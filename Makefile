WCLIST = chi2prop mcparas propagator create_source
BCLIST = 
HLIST = oformat tridiagmatrix debug enum_utils mydebug Bnode Btree
DLIST = 
CFLAG = -std=c++0x -Wall -g -O2
DEBUG = 
CLIST = $(BCLIST) $(WCLIST)
VPATH = $(foreach n,$(CLIST),./$(n))

UDLIST = $(filter-out $(DLIST),$(CLIST))
DOBJ = $(foreach n,$(DLIST),$(n)/$(n).o)
UDOBJ = $(foreach n,$(UDLIST),$(n)/$(n).o)
BOBJ = $(foreach n, $(BCLIST), $(n)/$(n).o)
WOBJ = $(foreach n, $(WCLIST), $(n)/$(n).o)
INC = $(foreach n,$(CLIST) $(HLIST),$(n)/$(n).h)
INC_PATH = $(foreach n,$(CLIST) $(HLIST),-I ../$(n)/)

all:libwork
libwork:$(WOBJ)
	ar crs libworks.a $(WOBJ)
$(DOBJ):%.o:%.cc
	cd $(dir $<); g++ $(CFLAG) $(DEBUG) -D DATDIR=\"$(HOME)/lib/$(basename $(notdir $<))_data\" $(INC_PATH) -c -fPIC $(notdir $<) -o $(notdir $@)
$(UDOBJ):%.o:%.cc
	cd $(dir $<); g++ $(CFLAG) -D DATDIR=\"$(HOME)/lib/$(basename $(notdir $<))_data\" $(INC_PATH) -c -fPIC $(notdir $<) -o $(notdir $@)
clean:
	rm */*.o libworks.a
install:
	install -D -m 664 $(INC) ~/include
	install -D libworks.a ~/lib
	mkdir -p ~/lib/load_dat_data
	install -D load_dat/load_dat_data/* ~/lib/load_dat_data
	mkdir -p ~/lib/anaspec_data
	install -D anaspec/anaspec_data/* ~/lib/anaspec_data
	mkdir -p ~/lib/anaspec_pppc_data
	install -D anaspec_pppc/anaspec_pppc_data/* ~/lib/anaspec_pppc_data
