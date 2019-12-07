#ifndef PRINTER_H
#define PRINTER_H

//@TODO: move away!
class Printer
{
 public:
    virtual void print(const char * s) = 0;
    virtual void error(void) = 0;
};

#endif  // PRINTER_H