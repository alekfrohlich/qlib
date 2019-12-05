include $(MAKEINC)

.PHONY: all clean

CXX_SRC := display.cc $(wildcard $(LIB)/*.cc)

OBJS  	:= $(CXX_SRC:.cc=.o)

# object files
OBJ_LINK_LIST := crt0.o crtend.o $(OBJS) kmain.o crtbegin.o

# linker flags
LDFLAGS := -Bstatic -nostdlib -T linker.ld

crt0.o: crt0.S
	$(AS) crt0.S -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJ_LINK_LIST)
	$(LD) $(LDFLAGS) $(OBJ_LINK_LIST) -o $(FATBIN)

clean:
	@rm -f *.o
