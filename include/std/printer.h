#ifndef PRINTER_H
#define PRINTER_H

class Printer
{
 public:
    virtual void print(const char * s) = 0;
    virtual void error(void) = 0;
};

namespace _setup {
extern Printer * printer;
}  // namespace _setup

#endif  // PRINTER_H