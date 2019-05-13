
#include "HeapManagerProxy.h"

#include <assert.h>
#include <stdio.h>

#include "HeapManager.h"

namespace HeapManagerProxy
{

HeapManager * CreateHeapManager( void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors )
{
	return HeapManager::create( i_pMemory, i_sizeMemory, i_numDescriptors );
}

void Destroy( HeapManager * i_pManager )
{
	assert( i_pManager );

	i_pManager->destroy();
}

void * alloc( HeapManager * i_pManager, size_t i_size )
{
	assert( i_pManager );
	return i_pManager->_alloc( i_size );
}

void * alloc( HeapManager * i_pManager, size_t i_size, unsigned int i_alignment )
{
	assert( i_pManager );

	return i_pManager->_alloc( i_size, i_alignment );
}

bool free( HeapManager * i_pManager, void * i_ptr )
{
	assert( i_pManager );

	return i_pManager->_free( i_ptr );
}

void Collect( HeapManager * i_pManager )
{
	assert( i_pManager );

	i_pManager->collect();
}


bool Contains( const HeapManager * i_pManager, void * i_ptr )
{
	assert( i_pManager );

	return i_pManager->Contains( i_ptr );
}

bool IsAllocated( const HeapManager * i_pManager, void * i_ptr )
{
	assert( i_pManager );

	return i_pManager->IsAllocated( i_ptr );
}

size_t GetLargestFreeBlock( const HeapManager * i_pManager )
{
	assert( i_pManager );

	return i_pManager->getLargestFreeBlock();
}

size_t GetTotalFreeMemory( const HeapManager * i_pManager )
{
	assert( i_pManager );

	return i_pManager->getTotalFreeMemory();
}

void ShowFreeBlocks( const HeapManager * i_pManager )
{
	assert( i_pManager );

	i_pManager->ShowFreeBlocks();
}

void ShowOutstandingAllocations( const HeapManager * i_pManager )
{
	assert( i_pManager );

#ifdef __TRACK_ALLOCATIONS
	i_pManager->ShowOutstandingAllocations();
#else
	printf( "HeapManager compiled without __TRACK_ALLOCATIONS defined.\n" );
#endif

}

} // namespace HeapManagerProxy