#include <iostream>
#include <windows.h>

const size_t cNullIndexCA = -1;

class CoalesceAllocator {
public:
    CoalesceAllocator() 
    {
#ifdef _DEBUG
        isInitialized = false;
        isDeinitialized = false;
        numAlloc = 0;
        numFree = 0;

        std::cout << "\tCA: Constructed\n";
#endif // _DEBUG
    }

    ~CoalesceAllocator() 
    {
#ifdef _DEBUG
        assert(isDeinitialized && "CA: ERROR\n\tNot destroed!");
        std::cout << "\tCA: Destructed\n";
#endif // _DEBUG
    }

    void init(size_t bufferSize) 
    {
#ifdef _DEBUG
        assert(!isDeinitialized && "CA: ERROR\n\tNot deinitialized!");
        isInitialized = true;
        isDeinitialized = false;
        std::cout << "\tCA: Init " << _bufferSize << " block size\n";
#endif // _DEBUG
     
        _bufferSize = bufferSize;
        allocBuffer(_buffer);
    }

    void destroy() {
#ifdef _DEBUG
        assert(isInitialized && "CA: ERROR\n\tNot initialized!");
        isInitialized = false;
        isDeinitialized = true;
        std::cout << "\tCA: Destroy pages with " << _bufferSize << " block size\n";
#endif // _DEBUG

        destroyBuffer(_buffer);
    }

    void* alloc(size_t size) 
    {
#ifdef _DEBUG
        assert(isInitialized && "CA: ERROR\n\tNot initialized!");
        std::cout << "\tCA: Alloc " << _bufferSize << " block size\n";
        numAlloc++;
#endif // _DEBUG
        
        Buffer* curBuffer = _buffer;
  
        while (true) 
        {
            if (curBuffer->fh != cNullIndexCA) 
            {
                Block* curBlock = (Block*)(void*)(((char*)(curBuffer->blocks)) + curBuffer->fh);
                Block* prevBlock = nullptr;
                size_t freeIndex;
                while (true) 
                {
                    freeIndex = *(size_t*)(curBlock->data);
                    if (curBlock->size >= size) 
                    {
                        if (curBlock->size == size) 
                        {
                            if (prevBlock == nullptr) 
                            {
                                curBuffer->fh = freeIndex;
                            }
                            else 
                            {
                                *(size_t*)(prevBlock->data) = cNullIndexCA;
                            }
                            curBlock->isFree = false;
                            return curBlock->data;
                        }

                        size_t tempSize = curBlock->size;
                        curBlock->size = size;
                        Block* newBlock = (Block*)(void*)(((char*)(curBlock->data)) + size);
                        newBlock->data = (char*)(void*)(newBlock) + sizeof(Block);
                        newBlock->size = tempSize - size - sizeof(Block);
                        newBlock->isFree = true;
                        *(size_t*)(newBlock->data) = cNullIndexCA;

                        if (prevBlock == nullptr) 
                        {
                            curBuffer->fh = (char*)(void*)(newBlock) - (char*)(curBuffer->blocks);
                        }
                        else 
                        {
                            *(size_t*)(prevBlock->data) = (char*)(void*)(newBlock) - (char*)(curBuffer->blocks);
                        }

                        curBlock->isFree = false;
                        return curBlock->data;
                    }

                    if (freeIndex == cNullIndexCA)
                    {
                        break;
                    }

                    prevBlock = curBlock;
                    curBlock = (Block*)(void*)((char*)(curBuffer->blocks) + freeIndex);
                }
            }

            if (curBuffer->nextBuffer == nullptr) 
            {
                allocBuffer(curBuffer->nextBuffer);
            }
            curBuffer = curBuffer->nextBuffer;
        }
    }


    bool free(void* p) {
#ifdef _DEBUG
        assert(isInitialized && "CA: ERROR\n\tNot initialized!");
        std::cout << "\tCA: Free " << _bufferSize << " block size\n";
        numFree++;
#endif // _DEBUG

        Buffer* curBuffer = _buffer;
        while (curBuffer != nullptr) 
        {
            if (curBuffer->blocks < p && (char*)curBuffer->blocks + _bufferSize + sizeof(Block) > p) 
            {
                Block* curBlock = (Block*)(void*)(char*)(curBuffer->blocks);
                Block* prevBlock = nullptr;
                while (curBlock->data < p) 
                {
                    prevBlock = curBlock;
                    curBlock = (Block*)(void*)((char*)(curBlock->data) + curBlock->size);
                }

                Block* nextBlock = (Block*)(void*)((char*)(curBlock->data) + curBlock->size);

                if ((void*)(nextBlock) >= (char*)curBuffer->blocks + _bufferSize + sizeof(Block)) 
                {
                    nextBlock = nullptr;
                }

                if (nextBlock != nullptr && nextBlock->isFree) {
                    size_t nextBlockIndex = curBuffer->fh;
                    size_t prevIndex = cNullIndexCA;
                    while ((Block*)(void*)((char*)(curBuffer->blocks) + nextBlockIndex) != nextBlock)
                    {
                        prevIndex = nextBlockIndex;
                        nextBlockIndex = *static_cast<size_t*>(static_cast<Block*>(static_cast<void*>(static_cast<char*>(curBuffer->blocks) + nextBlockIndex))->data);
                    }

                    if (prevIndex == cNullIndexCA) 
                    { 
                        curBuffer->fh = *(size_t*)(nextBlock->data);
                    }
                    else 
                    {
                        *static_cast<size_t*>(static_cast<Block*>(static_cast<void*>(static_cast<char*>(curBuffer->blocks) + prevIndex))->data) =
                            *static_cast<size_t*>(nextBlock->data);
                    }
                    curBlock->size += nextBlock->size + sizeof(Block);
                }

                if (prevBlock != nullptr && prevBlock->isFree) 
                {
                    prevBlock->size += curBlock->size + sizeof(Block);
                    return true;
                }

                curBlock->isFree = true;
                *(size_t*)(curBlock->data) = curBuffer->fh;
                curBuffer->fh = (char*)((void*)(curBlock)) - (char*)(curBuffer->blocks);

                return true;
            }
            curBuffer = curBuffer->nextBuffer;
        }

#ifdef _DEBUG
        numFree--;
#endif // _DEBUG
        return false;
    }

#ifdef _DEBUG
    void dumpStat() const 
    {
        assert(isInitialized && "CA: ERROR\n\tNot initialized!");
        std::cout << "----------------------------------------\n";
        std::cout << "CA: Dump Stat\n";
        std::cout << "\t\tAllocs: " << numAlloc << " Frees: " << numFree << std::endl;

        Buffer* curBuffer = _buffer;
        size_t numBusyBlocks = 0;
        size_t numFreeBlocks = 0;

        while (curBuffer != nullptr) 
        {
            Block* curBlock = static_cast<Block*>(curBuffer->blocks);
            while ((char*)((void*)(curBlock)) - (char*)(curBuffer->blocks) < _bufferSize + sizeof(Block)) 
            {
                if (curBlock->isFree) 
                {
                    numFreeBlocks++;
                }
                else 
                {
                    numBusyBlocks++;
                }

                curBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(curBlock->data) + curBlock->size));
            }
            curBuffer = curBuffer->nextBuffer;
        }

        std::cout << "\t\tBusy Blocks: " << numBusyBlocks << " Free Blocks: " << numFreeBlocks << std::endl;

        std::cout << "\t\tOS Buffers:" << std::endl;
        curBuffer = _buffer;
        size_t bufferIndex = 0;
        while (curBuffer != nullptr) {
            std::cout << "\t\t\tBuffer " << bufferIndex << " Adress: " << static_cast<void*>(curBuffer)
                << " Size: " << _bufferSize + sizeof(Buffer) + sizeof(Block) << std::endl;
            bufferIndex++;
            curBuffer = curBuffer->nextBuffer;
        }
        std::cout << "----------------------------------------\n";
    }


    void dumpBlocks() const 
    {
        assert(isInitialized && "CA: ERROR\n\tNot initialized!");
        std::cout << "----------------------------------------\n";
        std::cout << "CA: Dump Blocks\n";

        Buffer* curBuffer = _buffer;
        size_t bufferIndex = 0;

        while (curBuffer != nullptr) 
        {
            std::cout << "\t\tBuffer " << bufferIndex << std::endl;
            Block* curBlock = (Block*)(curBuffer->blocks);
            size_t blockIndex = 0;
            while (static_cast<char*>(static_cast<void*>(curBlock)) - static_cast<char*>(curBuffer->blocks) < _bufferSize + sizeof(Block)) 
            {
                std::cout << "\t\t\tBlock " << blockIndex;
                if (curBlock->isFree) 
                {
                    std::cout << " Free";
                }
                else 
                {
                    std::cout << " Busy";
                }

                blockIndex++;

                std::cout << " Adress: " << static_cast<void*>(curBlock) << " Size " << curBlock->size << std::endl;

                curBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(curBlock->data) + curBlock->size));
            }
            curBuffer = curBuffer->nextBuffer;
            bufferIndex++;
        }
        std::cout << "----------------------------------------\n";
    }
#endif

    struct Buffer {
        Buffer* nextBuffer;
        size_t fh;
        void* blocks;
    };

    struct Block {
        size_t size;
        void* data;
        bool isFree;
    };

    void allocBuffer(Buffer* &buffer)
    {
        void* pBuf = VirtualAlloc(NULL, _bufferSize + sizeof(Buffer) + sizeof(Block), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        buffer = (Buffer*)(pBuf);
        buffer->nextBuffer = nullptr;
        buffer->fh = 0;
        buffer->blocks = (char*)(pBuf) + sizeof(Buffer);
        Block* b = (Block*)(buffer->blocks);
        b->size = _bufferSize;
        b->data = (char*)(void*)(b) + sizeof(Block);
        b->isFree = true;
        *(size_t*)(b->data) = cNullIndexCA;
    }

    void destroyBuffer(Buffer*& buffer)
    {
        if (buffer == nullptr) { return; }

        destroyBuffer(buffer->nextBuffer);
        VirtualFree((void*)(buffer), 0, MEM_RELEASE);
    }

#ifdef _DEBUG
    bool isInitialized;
    bool isDeinitialized;
    size_t numAlloc;
    size_t numFree;
#endif  // _DEBUG

    size_t _bufferSize;
    Buffer* _buffer;
};