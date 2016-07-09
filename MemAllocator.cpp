#include "MemAllocator.h"


char* createMemoryBlock( const unsigned long& records, const unsigned short recsize )
{
	char* mem = new char[ records * recsize ];
	m_memblocks.push_back( mem );
	return mem;
}

void MemAllocator::releaseMemoryBlock( const char* mem )
{
	vector<char*>::iterator itr = m_memblocks.find( mem );
	if( itr != m_memblocks.end())
	{
		delete[] (*itr);
	}

}
