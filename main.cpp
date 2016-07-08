#include <iostream>
#include <ctime>
#include "Parser.h"
#include "NewRecord.h"
#include "FileProcessor.h"

using namespace std;

int main(int argc, char** argv)
{
	unsigned long start=clock(), d_beg(0), d_end(0);
	string _filename;
	if( argc == 2 ) 
	{
		_filename = argv[1];
	}
	else if( argc == 3)
	{
		_filename = argv[1];
		FileProcessor::FLUSH_LIMIT = stoi(argv[2]);
		
	}
	else
	{
		cout << "usage : ./main.exe <filename> \n";
		return 0;
	}

	cout << "Parse begin\n";
    Parser file( _filename ) ;
    file.parse_file();
	
		
    FileProcessor::FlushAllFiles();
		
	cout << "Deserialization begin\n";
	d_beg = clock();
	FileProcessor::deserialize();
	d_end = clock();
	
	cout<< "Deserialization complete, time taken : " << (d_end-d_beg)/double(CLOCKS_PER_SEC) << " seconds.\n";
	unsigned long end = clock();
	
	cout << "Total run time : " << (clock()-start)/double(CLOCKS_PER_SEC)  << " seconds\n";
    return 0;
}

