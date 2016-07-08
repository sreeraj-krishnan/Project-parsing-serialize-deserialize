#ifndef __FILE_PROCESSOR__H
#define __FILE_PROCESSOR__H

#include<string>
#include<deque>
#include<map>
#include<vector>
#include<list>
#include<queue>
#include<utility>

using namespace std;

class FileProcessor;
class NewRecord;

typedef map < string , FileProcessor* > FileMap;
typedef map < char , unsigned int > RecordLengthMap;          // save length for different records, namely 3, Q, S , T

typedef queue< pair < int, int > > WriteChunks; // save begin and end 
typedef map < string , WriteChunks* > SaveWriteBlocks; // per file write chunk size, so that it can be read in same size back

typedef list<string> SymbolFileList;

typedef queue<NewRecord*> Queue;

typedef deque < Queue* > QueuePerSymbol;
//typedef deque<NewRecord*> Deque;
typedef map < string , Queue* > QueueMap;

typedef map < string, ifstream* > IfstreamMap;

class FileProcessor
{
	public:
	struct Data;
	deque<Data*> chunks;
	
	private:
		string m_symbol;
		unsigned int total_data;
		int m_records;
		
	public:
		
	FileProcessor( const string& _symbol  );
	~FileProcessor();
	
	string get_symbol() const { return m_symbol ; }
	void AddData(const char& _record, const char* _data, const unsigned int& _data_length );
	
	deque<Data*>& get_chunks() { return chunks; }
	unsigned int get_total_data() const { return total_data; }
	const int get_records() const { return m_records; }
	
	void set_total_records(const int records);
	void set_total_data(const unsigned int total);
	void flush_data_queue();
	
	static void deserialize();
	
	static FileMap filemap;
	static SymbolFileList symlist;
	static RecordLengthMap recordlengthmap;
	static SaveWriteBlocks save_write_blocks;
	static QueuePerSymbol queue_per_sym;
	static QueueMap queue_map;
	static char* maxblock;
	static IfstreamMap fmap;
	
	
	static int FLUSH_LIMIT;
	
	static void AddDataToFile(const string& _symbol, const char& _record, const char* _data, const unsigned int& _data_length );
	static void WriteToFile(const string& _sym);
	static void FlushAllFiles();
	static bool fetch_records( const string sym );        // from symbol file
	static void fetch_records( int len, char* read_data, const string& _sym ); // from memory per sym
	static void fetch_more_records();
	static void flushOpenFiles();
	
	static void write_csv_records(vector< NewRecord* >& final_list,ofstream& out);
	static void order_records(ofstream& out);
	
	static unsigned long beg,end;
		
};

struct FileProcessor::Data
{
			char record;
			unsigned int len;
			char* data;
			Data(const char& _record, const unsigned int& len, const char* _data);
			Data( Data const& _data) = delete;
			void operator=(Data const& _d) = delete;
			~Data();
};

#endif