.DELETE_ON_ERROR:

ifndef CLASTOOL
    $(error "Please set the variable CLASTOOL")
endif

ROOTCONFIG := root-config
ROOTCFLAGS := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS := $(shell $(ROOTCONFIG) --ldflags)
ROOTGLIBS := $(shell $(ROOTCONFIG) --glibs)

CXX := c++
CXXFLAGS := -O2 -Wall -fPIC $(ROOTCFLAGS)
LD := c++
LDFLAGS := -O2 $(ROOTLDFLAGS)

INCLUDES := -I/user/o/${LOGNAME}/Analyser/include\
               -I$(CLASTOOL)/include
LIBS := $(ROOTGLIBS) \
               -L$(CLASTOOL)/slib/${OS_NAME} -lClasTool -lClasBanks -lVirtualReader -lDSTReader -lMathMore -lMatrix\
               -L/user/o/${LOGNAME}/Analyser/slib/ -lTIdentificator -lSpectrum -lEG

FILES := get_simple_tuple pi0_eta_id pi0_eta_id_acc pi0_eta_id_acc_sim mb_sub get_mb_binned get_recsim_binned get_mb_binned_gsim get_recsim_binned_gsim get_gamma_binned get_onlygamma_binned eta_id get_recsim_binned_mod get_data_binned get_acceptance get_MRatio get_Nelectrons


.PHONY: all clean

all: $(FILES)

write_tree: write_tree.o


%: %.o
	@echo "Doing application " $@ 
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $^

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


clean:
	rm -f $(FILES:%=%.o) $(FILES)
