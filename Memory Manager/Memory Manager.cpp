#include <iostream>
#include <Windows.h>
#include "MemoryManager.h"


int main()
{
    MemoryAllocator allocator;
    allocator.init();
    void* p = allocator.alloc(2048);
    void* s = allocator.alloc(3025);
    allocator.free(p);
    allocator.dumpStat();
    allocator.dumpBlocks();
    allocator.destroy();

    return 0;
}
