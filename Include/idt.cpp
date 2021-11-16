#include "system.h"

// Defines an idt entry
struct idtEntry
{
    unsigned short baseLo;
    unsigned short sel;  // Our kernel segment goes here 
    unsigned char always0;  // This will ALWAYS be set to 0
    unsigned char flags;   
    unsigned short baseHi;
} __attribute__((packed));

// Defines a special pointer object
struct idtPtr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idtEntry idt[256];  // Declare an idt of 256 entries
struct idtPtr idtp;  // Create the special pointer object

extern "C" void loadIdt();  // Defined in kernel_entry.asm

// Set entry in the idt
void setIdtGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    // The interrupt routine's base address
    idt[num].baseLo = (base & 0xFFFF);
    idt[num].baseHi = (base >> 16) & 0xFFFF;

    // The segment or 'selector' that this IDT entry will use is set here, along with any access flags
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

// Install the idt
void installIdt() {
    idtp.limit = (sizeof (struct idtEntry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    memset(&idt, 0, sizeof(struct idtEntry) * 256);

    loadIdt();
}