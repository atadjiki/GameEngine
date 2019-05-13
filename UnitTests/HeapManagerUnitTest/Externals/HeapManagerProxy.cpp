#include "HeapManagerProxy.h"

HeapManager * HeapManagerProxy::CreateHeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
{
	return nullptr;
}

void HeapManagerProxy::Destroy(HeapManager * i_pManager)
{
}

void * HeapManagerProxy::alloc(HeapManager * i_pManager, size_t i_size)
{
	return nullptr;
}

void * HeapManagerProxy::alloc(HeapManager * i_pManager, size_t i_size, unsigned int i_alignment)
{
	return nullptr;
}

bool HeapManagerProxy::free(HeapManager * i_pManager, void * i_ptr)
{
	return false;
}
