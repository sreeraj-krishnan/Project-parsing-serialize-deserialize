#ifndef __MEM_ALLOC__H
#define __MEM_ALLOC__H

#include<vector>
using namespace std;

class MemAllocator
{
	public:
		
		static char* createMemoryBlock( const unsigned long& records, const unsigned short recsize );
	
		static void releaseMemoryBlock( const char* );

	private:
		MemAllocator();
		
		vector<char*> m_memblocks;
};

#endif
