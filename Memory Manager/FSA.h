#include <iostream>
#include <windows.h>
#include <cassert>

const size_t cNullIndex = -1;

class FSA {
public:
	FSA()
	{
	#ifdef _DEBUG
		isInitialized = false;
		isDeinitialized = false;
		numAlloc = 0;
		numFree = 0;

		std::cout << "\tFSA: Constructed\n";
	#endif // _DEBUG
	}
	
	~FSA() 
	{
	#ifdef _DEBUG
		assert(isDeinitialized && "FSA: ERROR\n\tNot initialized!");
		std::cout << "\tFSA: Destructed\n";
	#endif // _DEBUG
	}
	
	void init(size_t blockSize, size_t numBlocksInPage) 
	{
	#ifdef _DEBUG
		assert(!isDeinitialized && "FSA: ERROR\n\tNot deinitialized!");
		isInitialized = true;
		isDeinitialized = false;
		std::cout << "\tFSA: Init " << _blockSize << " block size\n";
	#endif // _DEBUG
	
		_blockSize = blockSize;
		_numBlocksInPage = numBlocksInPage;
	
		createPage(_page);
	}

	void destroy()
	{
#ifdef _DEBUG
		assert(isInitialized && "FSA: ERROR\n\tNot initialized!");
		isInitialized = false;
		isDeinitialized = true;

		std::cout << "\tFSA: Destroy pages with " << _blockSize << " block size\n";
#endif // _DEBUG

		destroyPage(_page);
		_page = nullptr;
	}

	void* alloc(size_t size) 
	{
#ifdef _DEBUG
		assert(isInitialized && "FSA: ERROR\n\tNot initialized!");
		std::cout << "\tFSA: Alloc " << _blockSize << " block size\n";
		numAlloc++;
#endif // _DEBUG

		Page *curPage = _page;

		while (curPage->fh == cNullIndex) 
		{
			if (curPage->numInitialized < _numBlocksInPage) 
			{
				curPage->numInitialized++;
				void *p = (char*)(curPage->data) + (_numBlocksInPage * curPage->numInitialized - 1) * _blockSize;
				return p;
			}

			if (curPage->nextPage == nullptr) 
			{
				createPage(curPage->nextPage);
			}
			
			curPage = curPage->nextPage;
		}

		void* p = (char*)(curPage->data) + curPage->fh * _blockSize;
		curPage->fh = *(size_t*)p;
		return p;
	}

	bool free(void* p) 
	{
#ifdef _DEBUG
		assert(isInitialized && "FSA: ERROR\n\tNot initialized!");
		std::cout << "\tFSA: Free " << _blockSize << " block size\n";
		numFree++;
#endif // _DEBUG

		Page *curPage = _page;

		while (curPage != nullptr) {
			if ((void*)(curPage->data) <= p && (void*)((char*)(curPage->data) + _numBlocksInPage * _blockSize) > p) {
				*(size_t*)(p) = curPage->fh;
				curPage->fh = (size_t)(((char*)(p) - (char*)(curPage->data))) / _blockSize;
				curPage->numInitialized--;
				return true;
			}
			curPage = curPage->nextPage;
		}

		return false;
	}
	
#ifdef _DEBUG
	void dumpStat() const 
	{
		assert(isInitialized && "FSA: ERROR\n\tNot initialized!");
		std::cout << "----------------------------------------\n";
		std::cout << "FSA: Dump Stat for " << _blockSize << " block size\n";

		Page* curPage = _page;
		size_t countPages = 0;
		size_t countFreeMemory = 0;
		size_t countOccupMemory = 0;

		std::cout << "\tOS Memory:\n";

		while (curPage != nullptr) {
			countOccupMemory += curPage->numInitialized;
			countFreeMemory += _numBlocksInPage - curPage->numInitialized;

			std::cout << "\t\tAdress: " << (void*)(curPage) << "\n\t\tSize: " <<
				_blockSize * _numBlocksInPage + sizeof(Page) << std::endl;

			curPage = curPage->nextPage;
			countPages++;
		}

		std::cout << "\tCount Pages: " << countPages << "\n";
		std::cout << "\tCount Free Memmory Blocks: " << countFreeMemory << "\n";
		std::cout << "\tCount Occup Memmory Blocks: " << countOccupMemory << "\n";
		std::cout << "----------------------------------------\n";
	}

	void dumpBlocks() const 
	{
		assert(isInitialized && "FSA: ERROR\n\tNot initialized!");
		std::cout << "----------------------------------------\n";
		std::cout << "FSA: Dump Blocks for " << _blockSize << " block size\n";

		Page* curPage = _page;
		size_t curIndexPage = 1;

		while (curPage != nullptr) 
		{
			std::cout << "\tPage: " << curIndexPage << "\n";
			for (size_t i = 0; i < curPage->numInitialized; i++) 
			{
				bool isFree = false;
				size_t curIndex = curPage->fh;

				while (curIndex != cNullIndex) 
				{
					if (i == curIndex) 
					{
						isFree = true;
						break;
					}
					curIndex = *(size_t*)(void*)((char*)(curPage->data) + curIndex * _blockSize);
				}

				std::cout << "\tAdress: " << (void*)((char*)(curPage->data) + i * _blockSize);

				std::cout << "\n\tBlock " << i + 1;
				if (!isFree) 
				{
					std::cout << " is Busy\n";
				}
				else 
				{
					std::cout << " is Free\n";
				}
			}

			curPage = curPage->nextPage;
			curIndexPage++;
		}

		std::cout << "----------------------------------------\n";
	}
#endif // _DEBUG

private:
	struct Page {
		Page *nextPage;
		size_t fh;
		size_t numInitialized = 0;
		char *data;
	};

	void createPage(Page* &page) 
	{
		void* pBuff = VirtualAlloc(NULL, sizeof(Page) + _blockSize * _numBlocksInPage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		page = (Page*)pBuff;
		page->nextPage = nullptr;
		page->fh = cNullIndex;
		page->numInitialized = 0;
		page->data = (char*)pBuff + sizeof(Page);

#ifdef _DEBUG
		std::cout << "\tFSA: Create page with " << _blockSize << " block size\n";
#endif // _DEBUG
	}

	void destroyPage(Page*& page) 
	{
		if (page == nullptr) { return; }

		destroyPage(page->nextPage);
		VirtualFree((void*)page, 0, MEM_RELEASE);
	}

	Page* _page;
	size_t _blockSize;
	size_t _numBlocksInPage;

#ifdef _DEBUG
	bool isInitialized;
	bool isDeinitialized;
	size_t numAlloc;
	size_t numFree;
#endif // _DEBUG
};
