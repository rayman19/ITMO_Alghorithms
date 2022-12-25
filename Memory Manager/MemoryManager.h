#include <windows.h>
#include <vector>
#include "FSA.h"
#include "CoalesceAllocator.h"


class MemoryAllocator {
public:
    MemoryAllocator() 
    {
#ifdef _DEBUG
        isInitialized = false;
        isDeinitialized = false;
        numAlloc = 0;
        numFree = 0;
        std::cout << "Memory Manager: Constructed\n";
#endif // _DEBUG
    }

    ~MemoryAllocator() 
    {
#ifdef _DEBUG
        std::cout << "\nMemory Manager: Destructed\n";
#endif // _DEBUG
    }
    
    void init() 
    {
#ifdef _DEBUG
        std::cout << "\nMemory Manager: Init\n";
        isInitialized = true;
        isDeinitialized = false;
#endif // _DEBUG        
        
        fsa16.init(16, 512);
        fsa32.init(32, 256);
        fsa64.init(64, 128);
        fsa128.init(128, 64);
        fsa256.init(256, 32);
        fsa512.init(512, 16);
        ca.init(4096);
    }

    void destroy() 
    {
#ifdef _DEBUG
        std::cout << "\nMemory Manager: Destroy\n";
        isInitialized = false;
        isDeinitialized = true;
#endif // _DEBUG       
        
        fsa16.destroy();
        fsa32.destroy();
        fsa64.destroy();
        fsa128.destroy();
        fsa256.destroy();
        fsa512.destroy();
        ca.destroy();
        
        for (size_t i = 0; i < osBlocks.size(); i++) 
        {
            VirtualFree(osBlocks[i].data, 0, MEM_RELEASE);
        }
    }
    
    void *alloc(size_t size) 
    {
#ifdef _DEBUG
        std::cout << "\nMemory Manager: Alloc\n";
#endif // _DEBUG

        if (size < 16) return fsa16.alloc(size);
        else if (size < 32) return fsa32.alloc(size);
        else if (size < 64) return fsa64.alloc(size);
        else if (size < 128) return fsa128.alloc(size);
        else if (size < 256) return fsa256.alloc(size);
        else if (size < 512) return fsa512.alloc(size);
        else if (size < 4096) return ca.alloc(size);

        void* p = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        Block block;
        block.data = p;
        block.size = size;
        osBlocks.push_back(block);

        return p;
    }

    void free(void* p) 
    {
#ifdef _DEBUG
        std::cout << "\nMemory Manager: Free\n";
#endif // _DEBUG

        if (fsa16.free(p)) return;
        else if (fsa32.free(p)) return;
        else if (fsa64.free(p)) return;
        else if (fsa128.free(p)) return;
        else if (fsa256.free(p)) return;
        else if (fsa512.free(p)) return;
        else if (ca.free(p)) return;
        
        bool res = VirtualFree(p, 0, MEM_RELEASE);
        if (res) 
        {
            for (auto it = osBlocks.begin(); it < osBlocks.end(); it++) 
            {
                if (static_cast<Block>(*it).data == p) 
                {
                    osBlocks.erase(it);
                    break;
                }
            }
        }
    }

#ifdef _DEBUG
    void dumpStat() const
    {
        std::cout << "\nMemory Manager: Dump Stat\n";
        fsa16.dumpStat();
        fsa32.dumpStat();
        fsa64.dumpStat();
        fsa128.dumpStat();
        fsa256.dumpStat();
        fsa512.dumpStat();
        ca.dumpStat();

        std::cout << "----------------------------------------\n";
        std::cout << "\tOS Blocks:" << std::endl;
        for (size_t i = 0; i < osBlocks.size(); i++) 
        {
            std::cout << "\t\tBlock " << i << " Adress: " << osBlocks[i].data << " Size: " << osBlocks[i].size << std::endl;
        }
        std::cout << "----------------------------------------\n";
    }

    void dumpBlocks() const
    {
        std::cout << "\nMemory Manager: Dump Blocks\n";
        fsa16.dumpBlocks();
        fsa32.dumpBlocks();
        fsa64.dumpBlocks();
        fsa128.dumpBlocks();
        fsa256.dumpBlocks();
        fsa512.dumpBlocks();
        ca.dumpBlocks();

        std::cout << "----------------------------------------\n";
        std::cout << "\tOS Blocks:" << std::endl;
        for (size_t i = 0; i < osBlocks.size(); i++) {
            std::cout << "\t\tBlock " << i << " Adress: " << osBlocks[i].data << " Size: " << osBlocks[i].size << std::endl;
        }		
        std::cout << "----------------------------------------\n";
    }

#endif // _DEBUG
    struct Block {

        size_t size;
        void* data;
    };

    FSA fsa16, fsa32, fsa64, fsa128, fsa256, fsa512;
    CoalesceAllocator ca;
    std::vector<Block> osBlocks;

#ifdef _DEBUG
    bool isInitialized;
    bool isDeinitialized;

    size_t numAlloc;
    size_t numFree;
#endif  // _DEBUG
};
