#include "system.h"

int cursorX;  // X-cord of the cursor
int cursorY;  // Y-cord of the cursor

unsigned int attributeByte;  // The color of the text
unsigned short *videoMemory;

// Move the cursor to the cursorX and cursorY positions
void updateCursor() {
    unsigned int cursorLocation = (cursorY * 80) + cursorX;  // Use liniar offset to find the location of the cursor

    outportb(0x3D4, 0x0F);  // Tell VGA board we are setting the high byte
    outportb(0x3D5, (unsigned int) (cursorLocation & 0x0FF));  // Set the high byte
    outportb(0x3D4, 0x0E);  // Tell VGA board we are setting the low byte
    outportb(0x3D5, (unsigned int) ((cursorLocation >> 8) & 0xFF));  // Send the low byte
}

// Directly set new x and y positions and move the cursor
void moveCursor(unsigned int x, unsigned int y) {
    cursorX = x;
    cursorY = y;
    updateCursor();
}

/* Scroll the screen
void scroll() {
    unsigned blank;  // A character with a space and a color

    if (cursorY >= 25) {

        blank = 0x20 | (attributeByte << 8);

        // Move current text on the screen back in the buffer by a line
        unsigned temp = cursorY - 25 + 1;
        memcpy(videoMemory, videoMemory + temp * 80, (25 - temp) * 80 * 2);

        // Set chunk of memory at the last line to the blank character
        memsetw(videoMemory + (25 - temp) * 80, blank, 80);
        cursorY = 25 - 1;

    }
}
*/

// Clears the screen
void clearScreen() {
    int i;  // Iterator
    unsigned blank = 0x20 | (attributeByte << 8);  // A character with a space and a color

    for (i = 0; i < 25; i++) { memsetw(videoMemory + i * 80, blank, 80); }  // Set every character to blank
    moveCursor(0, 0);  // Move cursor
}

// Print a character to the screen
void printChar(char c) {

    unsigned attrib = attributeByte << 8;
    unsigned short *location;

    if (c == 0x08 && cursorX > 0) { cursorX--; }  // Handle a backspace
    else if (c == 0x09) { cursorX = (cursorX + 8) & ~(8-1); }  // Handle a tab
    else if (c == '\r') { cursorX = 0; }  // Handle a carrige return
    else if (c == '\n') { cursorX = 0; cursorY++; }  // Handle a newline

    // Handle every other character
    else if (c >= ' ') {
        location = videoMemory + (cursorY * 80 + cursorX);
        *location = c | attrib;
        cursorX++;
    }

    if (cursorX >= 80) {
        cursorX = 0;
        cursorY++;
    }

    updateCursor();
}

// Print string to screen
void print(const char *string) {
    int i = 0;
    while (string[i]) { printChar(string[i++]); }
}

void setTextColor(unsigned int foreground, unsigned int background) { attributeByte = (background << 4) | (foreground & 0x0F); }

void initVideo(void) {
    videoMemory = (unsigned short *)0xB8000;
    clearScreen();
} 