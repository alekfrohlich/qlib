include $(MAKEINC)

.PHONY: all clean

CXX_SRC := 	$(wildcard $(ARCH)/*.cc) \
			$(wildcard $(MACH)/*.cc) \
			$(wildcard $(MACH)/$(MACHNAME)/*.cc) \
			$(wildcard $(STD)/*.cc)  \
		    $(wildcard $(APP)/*.cc)

OBJS  	:= $(CXX_SRC:.cc=.o)

# object files
OBJ_LINK_LIST := crt0.o crtend.o $(OBJS) lib_init.o crtbegin.o

# linker flags
LDFLAGS  := -static -nostdlib \
			-L $(patsubst %/libgcc.a,%/,$(shell $(CXX) -print-libgcc-file-name)) \
			-T link.ld

crt0.o: crt0.S
	$(AS) crt0.S -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJ_LINK_LIST)
	$(LD) $(LDFLAGS) $(OBJ_LINK_LIST) -lgcc -o $(FATBIN)

clean:
	@rm -f *.o
