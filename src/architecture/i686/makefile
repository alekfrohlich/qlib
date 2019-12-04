include $(MAKEINC)

.PHONY: all clean

# object files
OBJ_LINK_LIST := crt0.o crtend.o kmain.o crtbegin.o

# linker flags
LDFLAGS := -Bstatic -nostdlib -T linker.ld

crt0.o: crt0.S
	$(AS) crt0.S -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJ_LINK_LIST)
	$(LD) $(LDFLAGS) $(OBJ_LINK_LIST) -o $(FATBIN)

clean:
	@rm -f *.o
