#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef int size_t;

// kernel.cpp 
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

// screen.cpp
extern void moveCursor(unsigned int x, unsigned int y);
extern void print(const char *string);
extern void clearScreen();
extern void printChar(char c);
extern void initVideo(void);
extern void setTextColor(unsigned int foreground, unsigned int background);

// idt.cpp
extern void setIdtGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void installIdt();
extern void installIsr();

//isrs.cpp
/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

#endif