APP := huebiboot 

CXXLIBS :=-lc  $(shell pkg-config --libs libudev mount)
CXXLIBS += $(shell fltk-config --ldflags --use-images)

CXXLIBS_RELEASE :=-Wl,-s

CXXFLAGS := -IUI -IUI/icons
CXXFLAGS += $(shell pkg-config --cflags libudev mount)
CXXFLAGS += $(shell fltk-config --cxxflags)

CXXFLAGS_RELEASE:= -O3
CXXFLAGS_DEBUG:= -O0 -ggdb -DDEBUG -Wall -Wno-unused-function

CXX := g++

SOURCE := $(wildcard *.cxx *.c UI/*.cxx)

OBJ := $(patsubst %.cxx %.c,%.o,$(SOURCE))

programa: release
release: CXXFLAGS += $(CXXFLAGS_RELEASE)
release: CXXLIBS += $(CXXLIBS_RELEASE)
release: $(APP)

depurar: CXXFLAGS += $(CXXFLAGS_DEBUG)
depurar: $(APP)

#~ dependencias = sgdisk mkfs udisksctl blockdev
#~ T := $(foreach exec, $(dependencias), \
#~ 		$(if $(shell which $(exec)),Ok,$(error "No se encontró $(exec)))) \
			 

$(APP): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(CXXLIBS) -o $(APP)



.o:
	$(CXX) -c $<

limpiar:
	rm  huebiboot

