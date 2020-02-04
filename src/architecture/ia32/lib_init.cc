// #include <architecture/cpu.h>
// #include <machine/display.h>
// #include <machine/ic.h>
// #include <thread.h>

// int other_entry_point() {
//     using namespace qlib;
//     using namespace qlib::mediator;

//     // while (1) {
//     cout << "Other\n";
//     //     Thread::yield();
//     // }
//     return 1;
// }

// // wrong but should work
// static char _stack_space[1 << 16];
// static char * stack1 = &_stack_space[1 << 14];
// static char * stack2 = &_stack_space[1 << 15];

// extern "C" {

// extern int main(void);

// // @TODO: placement new
// qlib::Thread main_thread;
// qlib::Thread other_thread;

// void _lib_init(void) {
//     using namespace qlib;
//     using namespace qlib::mediator;

//     // initialize hardware mediators
//     CPU::init();
//     IC::init();
//     Display::init();

//     // setup entry points (int still disabled)
//     main_thread = Thread(stack1, 0);
//     other_thread = Thread(stack2, main);
//     main_thread.next = &other_thread;
//     other_thread.next = &main_thread;
//     Thread::running_thread = &main_thread;

//     // while (1)
//     cout << "Main\n";
//     Thread::yield();
//     // }
//     cout << "Back at Main!\n";

//     while (1)
//         ;

//     // main();
// }
// }
