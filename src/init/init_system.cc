#include <architecture/cpu.h>
#include <machine.h>
#include <system.h>

namespace qlib {

class Init_System
{
 public:
    Init_System() {
        db<Init>() << "Init_System()"
                   << "\n";

        db<Init>() << "Initializing the CPU ...\n";
        CPU::init();
        db<Init>() << "done\n";

        // Initialize machine hardware mediators.
        db<Init>() << "Initializing the Machine ...\n";
        Machine::init();
        db<Init>() << "done\n";

        // Initialize system abstractions.
        db<Init>() << "Initializing System abstractions ...\n";
        System::init();
        db<Init>() << "done\n";
    }
};

Init_System init_system;

};  // namespace qlib