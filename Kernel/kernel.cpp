#include "../Include/system.h"

extern "C" void main() {
    
    installIdt();
    installIsr();

    return;
}
