#ifndef __HEAP_MANAGER_PROXY_H
#define __HEAP_MANAGER_PROXY_H

#include <stdint.h>
#include <stdio.h>

class HeapManager {

public:

	struct BlockDescriptor
	{
	public:
		bool isFree = true;
		void * m_pBlockStartAddr = nullptr;
		size_t m_sizeBlock = 0;
		BlockDescriptor *next = NULL;
	};

	const void * pMyHeap;
	size_t sizeMyHeap;
	unsigned int descriptorCount;
	unsigned int count = 0;
	int freed = 0;
	int allocd = 0;
	int collects = 0;
	BlockDescriptor blocks;
	size_t blockListSize;

	HeapManager(const void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors) {

		descriptorCount = i_numDescriptors;
		blockListSize = (sizeof BlockDescriptor)*descriptorCount; //figure out size of block descriptor list
		pMyHeap = i_pMemory; //pointer to bottom of heap
		sizeMyHeap = i_sizeMemory - blockListSize; //size of entire heap, including descriptor list
		int temp = sizeof(BlockDescriptor);
		//set beginning of block list to the bottom of the heap
		_alloc(sizeMyHeap);
	}

	void destroy() {

		_free(pMyHeap);
		blocks = {};
	}

	void collect() {

		//walk through list
		BlockDescriptor * current = &blocks;

		while (true) {

			//if the current block is free
			if (current != nullptr && current->isFree && current->next != nullptr && current->next->isFree) {

				//if the next block is free
				current->isFree = true;
				current->m_sizeBlock = current->m_sizeBlock + current->next->m_sizeBlock;
				current->next = current->next->next;
				
				count--;

				collects++;
				break;
				
			}
			else if (current == nullptr || current->m_pBlockStartAddr == NULL) {
				break;;
			}
			else if (current->next != NULL) {
				current = current->next;
			}
			else if (current->next == NULL) {
				return;
			}
		}

		collect();
	}

	const bool _free(const void * ptr) {

		if (Contains(ptr)) {

			//if found, remove from linked list and moved to free list of blocks
			Remove(ptr);
			freed++;
			collect();
			return true;
		}

		return false;
	}

	void Remove(const void * ptr) {

		//walk through free list and see if it contains the pointer
		BlockDescriptor * current = &blocks;
		BlockDescriptor * previous = nullptr;

		while (true) {

			//find the block descriptor to remove
			if (current->m_pBlockStartAddr == ptr) {

				current->isFree = true;
				break;

			}
			else if (current->next != NULL) {

				previous = current;
				current = current->next;

			}
			else {
				break;
			}
		}
	}

	void * _alloc(size_t size) {

		//if descriptor count is max reject
		if (count > descriptorCount)
			return NULL;

		BlockDescriptor * current = &blocks;

		if (blocks.m_pBlockStartAddr == nullptr) {
			blocks = *(BlockDescriptor*)pMyHeap;
			blocks.isFree = true;
			blocks.m_pBlockStartAddr = &pMyHeap + blockListSize;
			blocks.m_sizeBlock = sizeMyHeap;
			blocks.next = NULL;
			allocd++;
			count++;
			return blocks.m_pBlockStartAddr;
		}
		if(current->next != NULL){
			current = current->next; //advance past heap

			while (true) {

				if (current->m_sizeBlock >= size && current->isFree) {

					//if an unused block of the same size is available, take it
					if (current->m_sizeBlock == size) {
						current->isFree = false;
						return current->m_pBlockStartAddr;
					}

					//if the block is only slightly bigger give it anyway
					else if (current->m_sizeBlock > size && current->m_sizeBlock <= size * 2) {
						current->isFree = false;
						return current->m_pBlockStartAddr;
					}

					//if the block is more than twice the desired size, subdivide it
					else if (current->m_sizeBlock >= size) {
						return subDivide(*current, size).m_pBlockStartAddr;
					}
					else {

						current->isFree = false;
						return current->m_pBlockStartAddr;
					}
				}
				else if (current->next != NULL) {
					current = current->next;
				}
				else {
					break;
				}
			}
	}

		
		//try the heap if nothing else works
		current = &blocks;

		if (current->m_sizeBlock >= size && current->isFree) {

			//if the block is more than twice the desired size, subdivide it
			if (current->m_sizeBlock > size) {

				return subDivide(*current, size).m_pBlockStartAddr;
			}
			else {
				current->isFree = false;
				return current->m_pBlockStartAddr;
			}
		}
		else {
			return NULL;
		}
	}

	void * _alloc(size_t size, unsigned int alignment) {

		BlockDescriptor * current = &blocks;

		//figre out aligned size
		while (true) {

			//if the size is aligned, dont need to add any more padding
			if (size % alignment == 0) {
				break;
			}
			else { //if still remainder, add
				size++;
			}
		}

		return _alloc(size);
	}

	BlockDescriptor subDivide(BlockDescriptor &currentFree, size_t &size) {


		//decrement the size of the free block being subdivided from
		currentFree.m_sizeBlock -= size;

		//find a new address to place the new block
		void * newAddress = &currentFree.m_pBlockStartAddr + currentFree.m_sizeBlock;

		//create a new block descriptor 
		BlockDescriptor * newBlock = (BlockDescriptor*)pMyHeap + (count);

		newBlock->isFree = false;
		newBlock->m_pBlockStartAddr = newAddress;
		newBlock->m_sizeBlock = size;
		newBlock->next = NULL;

		count++;
		allocd++;

		BlockDescriptor * current = &blocks;

		while (true) {

			if (current->next != NULL) {
				current = current->next;
			}
			else if (current->m_pBlockStartAddr == nullptr) {
				*current = *newBlock;

				break;
			}
			else {
				current->next = newBlock;
				break;
			}
		}

		return *newBlock;

	}

	static void ShowOutstandingAllocations(const HeapManager heapManager) {

		//walk through used block list and print 
		printf("Used Blocks: \n");
		const BlockDescriptor * current = &heapManager.blocks;

		while (true) {

			if (current != nullptr && !current->isFree) {
				printf("Block Addr: %p - Size: %Iu\n", current->m_pBlockStartAddr, current->m_sizeBlock);
				current = current->next;
			}
			else if (current == nullptr || current->m_pBlockStartAddr == NULL) {
				break;
			}
			else if (current->next != NULL) {
				current = current->next;
			}
			else if (current->next == NULL) {
				break;
			}
		}
	}

	static void ShowFreeBlocks(const HeapManager heapManager) {

		//walk through free block list and print 
		printf("Free Blocks: \n");

		const BlockDescriptor * current = &heapManager.blocks;

		while (true) {

			if (current != nullptr && current->isFree) {
				printf("Block Addr: %p - Size: %Iu\n", current->m_pBlockStartAddr, current->m_sizeBlock);
				current = current->next;
			}
			else if (current == nullptr || current->m_pBlockStartAddr == NULL) {
				break;
			}
			else if (current->next != NULL) {
				current = current->next;
			}
			else if (current->next == NULL) {
				break;
			}
		}
	}

	bool Contains(const void * ptr) const {

		//walk through list and see if it contains the pointer
		const BlockDescriptor * current = &blocks;
		bool contains = false;

		while (true) {

			if (current->m_pBlockStartAddr == ptr && !current->isFree) {
				return true;
			}
			else if (current->next != NULL) {

				current = current->next;
			}
			else {
				break;
			}
		}

		return contains;


	}

	bool IsAllocated(void * ptr) const {

		//walk through list and see if it contains the pointer
		const BlockDescriptor * current = &blocks;
		bool contains = false;

		while (true) {

			if (current->m_pBlockStartAddr == ptr && !current->isFree) {
				return true;
			}
			else if (current->next != NULL) {

				current = current->next;
			}
			else {
				break;
			}
		}

		return contains;

	}

	size_t getLargestFreeBlock() const {

		size_t largestSize = 0;

		//walk through list and find the largest size of free block
		const BlockDescriptor * current = &blocks;

		//skip past the heap
		if (current->next != NULL) {
			current = current->next;
		}
		else {
			return current->m_sizeBlock;
		}

		while (true) {

			if (current->isFree) {
				if (current->m_sizeBlock > largestSize)
					largestSize = current->m_sizeBlock;
			}
			else if (current->next != NULL) {

				current = current->next;
			}
			else {
				break;
			}
		}

		return largestSize;

	}

	size_t getTotalFreeMemory() const {

		size_t freeMemory = 0;

		//walk through list and find the largest size of free block
		const BlockDescriptor * current = &blocks;

		while (true) {

			if (current->isFree) {
				freeMemory += current->m_sizeBlock;
			}
			else if (current->next != NULL) {

				current = current->next;
			}
			else {
				break;
			}
		}

		return freeMemory;

	}
};

namespace HeapManagerProxy
{

	HeapManager *	CreateHeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	void			Destroy(HeapManager * i_pManager);

	void *			alloc(HeapManager * i_pManager, size_t i_size);
	void *			alloc(HeapManager * i_pManager, size_t i_size, unsigned int i_alignment);
	bool			free(HeapManager * i_pManager, void * i_ptr);

	void			Collect(HeapManager * i_pManager);

	bool			Contains(const HeapManager * i_pManager, void * i_ptr);
	bool			IsAllocated(const HeapManager * i_pManager, void * i_ptr);

	size_t			GetLargestFreeBlock(const HeapManager * i_pManager);
	size_t			GetTotalFreeMemory(const HeapManager * i_pManager);

	void			ShowFreeBlocks(const HeapManager * i_pManager);
	void			ShowOutstandingAllocations(const HeapManager * i_pManager);

} // namespace HeapManagerProxy

#endif // __HEAP_MANAGER_PROXY_H