include $(MAKEINC)

# object files
OBJS := crtend.o kmain.o crtbegin.o

# global constructor/destructor call
OBJ_LINK_LIST := crt0.o $(OBJS)

# linker flags
LDFLAGS := -Bstatic -T linker.ld

crt0.o:
	$(AS) crt0.S -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(OBJ_LINK_LIST)
	$(LD) $(LDFLAGS) $(OBJ_LINK_LIST) -o $(FATBIN)

.PHONY: clean

clean:
	@rm -f *.o