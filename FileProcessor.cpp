#include "FileProcessor.h"
#include "NewRecord.h"
#include "QuoteNewRecord.h"
#include "SignalNewRecord.h"
#include "TradeNewRecord.h"

#include<ctime>
#include<string.h>
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>


using namespace std;

FileMap FileProcessor::filemap;
RecordLengthMap FileProcessor::recordlengthmap;
SymbolFileList FileProcessor::symlist;
SaveWriteBlocks FileProcessor::save_write_blocks;
QueuePerSymbol FileProcessor::queue_per_sym;
QueueMap FileProcessor::queue_map;

unsigned long FileProcessor::beg, FileProcessor::end;
//MemBlock FileProcessor::memblock; 
static unsigned int maxblocksize(0); // = FileProcessor::FLUSH_LIMIT * (QuoteNewRecord::Fields::record_length+1) ;
char* FileProcessor::maxblock(0); // = new char [ maxblocksize ];

int FileProcessor::FLUSH_LIMIT = 1024;


FileProcessor::Data::Data(const char& _record, const unsigned int& _len, const char* _data):
		record(_record),
		len(_len),
		data(0)
{
		data = new char[len];
		memcpy(data, _data, len);
}

FileProcessor::Data::~Data()
{
	if( data )
	{
		delete[] data;
	}
}

FileProcessor::FileProcessor( const string& _filename ) :
	m_symbol(_filename),
	total_data(0),
	m_records(0)
{
		
}

FileProcessor::~FileProcessor()
{
	flush_data_queue();	
}

void FileProcessor::flush_data_queue()
{
	
	deque<FileProcessor::Data*>::iterator itr = chunks.begin();
	for( int i(0) ; itr != chunks.end(); itr++ )
	{
			delete (*itr);
	}
	chunks.clear();
	m_records = 0;
	total_data = 0;
	
}

void FileProcessor::AddDataToFile(const string& _symbol, const char& _record, const char* _data, const unsigned int& _data_length )
{
	if (!_data)
	{
		cout << "data is null \n";
		return;
	}
	// save record length for unserialize
	if( FileProcessor::recordlengthmap.count(_record) == 0 )
	{
		FileProcessor::recordlengthmap[ _record ] = (unsigned int)( _data_length );
	}

	if( FileProcessor::filemap.count( _symbol ) == 0 )
	{
		FileProcessor* file = new FileProcessor( _symbol );
		FileProcessor::filemap[ _symbol ] = file;
		FileProcessor::symlist.push_back( _symbol );
	}
	
	FileProcessor * file = FileProcessor::filemap[ _symbol ];
	file->AddData( _record, _data, _data_length );

	if( !FileProcessor::maxblock )
	{
		maxblocksize = FileProcessor::FLUSH_LIMIT * (QuoteNewRecord::Fields::record_length+1) ;
		FileProcessor::maxblock = new char [ maxblocksize ];
	}
	if( file->get_records() >= FileProcessor::FLUSH_LIMIT )
	{
		WriteToFile( _symbol );
	}

}

void FileProcessor::AddData(const char& _record, const char* _data, const unsigned int& _data_length )
{
	m_records++;
	Data* d = new Data( _record, _data_length, _data);
	chunks.push_back( d );
	total_data += _data_length+ sizeof(char); // +1 for record type as a char
	
}

void FileProcessor::WriteToFile(const string& _sym)
{
	
	FileProcessor * file = 0;
	if( !FileProcessor::filemap.count( _sym ) )
	{
		cout << "file not present \n";
		return;
	}
	file = FileProcessor::filemap[_sym ];

	if( !file )
	{
		std::cout << "file " << _sym << " not present\n";
		return;
	} 

	if ( FileProcessor::save_write_blocks.count( _sym ) == 0 )
	{
		WriteChunks* write_blocks = new WriteChunks();
		FileProcessor::save_write_blocks[ _sym ] = write_blocks;
	}
	WriteChunks* write_blocks = FileProcessor::save_write_blocks[ _sym ];
	
	//cout << "write blocks for " << _sym << " : " << write_blocks->size() << "\n";
	
	// copy all records to memory and write at one shot
	
	deque<FileProcessor::Data*>& chunks = file->get_chunks();
	
	const unsigned int& total_data( file->get_total_data() );
	
	char *write_data=0,*save_data=0;
	if( FileProcessor::maxblock && total_data <= maxblocksize )
	{
		save_data = write_data = FileProcessor::maxblock;
		//memset(write_data, 0 , total_data);
	}
	else
	{
		cout << "MEM ALLOC AGAIN : " << total_data - maxblocksize << "\n";
	}
	
	deque<FileProcessor::Data*>::iterator itr = chunks.begin();
	unsigned int save_write_blocks(0);
	for( int i(0) ; itr != chunks.end() && i < total_data; ++itr )
	{
		FileProcessor::Data* d = *itr;
		
		memcpy(write_data, reinterpret_cast<char*>(&(d->record)), sizeof(char) );
		write_data += sizeof(char);
	
		memcpy(write_data, d->data, (size_t)d->len );
		write_data += d->len;
		
		save_write_blocks += d->len + sizeof(char);
	}
	if( write_blocks->size() > 0 )
	{
		const pair< int,  int >& last_access = write_blocks->back();
		// 0,100 : 100,100 : 200,100 : 300,50 
		write_blocks->push( make_pair< int, int > ( last_access.first+last_access.second , save_write_blocks ) ); 
	}
	else
	{
		write_blocks->push( make_pair< int, int >(0,save_write_blocks));
	}
	const string filename = string(_sym) + string(".binary");
	
	ofstream outfile( filename.c_str(), ios::binary|ios::app|ios::ate|ios::out);
	if ( outfile.is_open() )
	{
		outfile.write( save_data, total_data );
	}
	else
	{
		cout << "unable to open file error \n";
		// do as needed
	};
	outfile.close();
	
	file->flush_data_queue();
}

void FileProcessor::FlushAllFiles()
{
	FileMap::iterator itr = FileProcessor::filemap.begin();
	
	for( ; itr != FileProcessor::filemap.end(); ++itr )
	{
		FileProcessor::WriteToFile( (*itr).first );
		delete (*itr).second;
	}
	
	FileProcessor::filemap.erase( FileProcessor::filemap.begin(), FileProcessor::filemap.end() );
	cout << "Serialization complete, time taken : " << (clock() - FileProcessor::beg)/double(CLOCKS_PER_SEC) << " seconds.\n";
	//cout << "FLUSHED ALL FILES \n";
}

void FileProcessor::set_total_data(const unsigned int total)
{
	total_data = total;
}

void FileProcessor::set_total_records(const int records)
{
	m_records = records;
}

void FileProcessor::deserialize()
{
		SymbolFileList::iterator itr = FileProcessor::symlist.begin();
		
		for( ; itr != FileProcessor::symlist.end(); itr++ )
		{
			FileProcessor::fetch_records( *itr );
			
		}
		unsigned long c_beg = clock();
		cout << "Write original CSV record\n";
		ofstream out("output.csv", ios::out );
		FileProcessor::order_records( out );
		out.close();
		cout << "Write original CSV record complete, time taken : " << (clock()-c_beg)/double(CLOCKS_PER_SEC) << " seconds.\n";
		delete[] FileProcessor::maxblock;
}

void FileProcessor::write_csv_records(vector< NewRecord* >& final_list,ofstream& out)
{
	vector< NewRecord* >::iterator itr = final_list.begin();
	for( ; itr != final_list.end() ; itr++ )
	{
		(*itr)->print( out );
		delete (*itr);
	}
	final_list.clear();
	
}
void PrintQueueFront()
{
	QueuePerSymbol::iterator itr = FileProcessor::queue_per_sym.begin();
	for( ; itr < FileProcessor::queue_per_sym.end(); itr++  )
	{
		if(*itr)
		{
			Queue* q = *itr;
			if (q ) 
			{
				cout << "qsize : " << q->size() << "\n";
				cout << "sequence : " << q->front()->get_seq_id() << "\n";
				cout << "type : " << q->front()->get_type() <<"\n"; 
			}
			
		}
		else
		{
			cout << "record null\n";
		}
		
	}
}
void FileProcessor::order_records(ofstream& out)
{
	QueuePerSymbol::iterator itr;
	unsigned long next_record=1;
	vector< NewRecord* > csv_list;
	for( QueueMap::iterator mitr = FileProcessor::queue_map.begin(); mitr != FileProcessor::queue_map.end(); mitr++ )
	{
		FileProcessor::queue_per_sym.push_back( mitr->second );
	}
	while( FileProcessor::queue_per_sym.size() )
	{
		itr = FileProcessor::queue_per_sym.begin();
		unsigned int last_record = next_record;
		for( ; itr < FileProcessor::queue_per_sym.end();  )
		{
			Queue* current_queue = *itr;
		
			if( !current_queue )
			{
				cout << "QUEUE EMPTY\n";
				itr = FileProcessor::queue_per_sym.erase(itr);
				continue;
			}
			if( !current_queue->size() )
			{
				fetch_more_records();
				if( !current_queue->size() )
				{
					FileProcessor::queue_per_sym.erase(itr);
				}
				break;
			}
			do
			{
				if( next_record == current_queue->front()->get_seq_id() )
				{
					next_record++;
					csv_list.push_back( current_queue->front() );
					current_queue->pop();
				}
				else
				{
					break;
				}
			}while( current_queue->size() );
		
			if ( csv_list.size() >= FileProcessor::FLUSH_LIMIT )
			{
				FileProcessor::write_csv_records( csv_list, out );
			}
			itr++;
		}
		if( last_record == next_record )
		{
			if( FileProcessor::queue_per_sym.size() )
			{
				continue;
				cout << "DESERIALIZATION ERROR\n";
				//PrintQueueFront();
			}
			
			break;
		}
		
	}
	FileProcessor::write_csv_records( csv_list, out );
}

void FileProcessor::fetch_more_records()
{
	for( QueueMap::iterator mitr = FileProcessor::queue_map.begin(); mitr != FileProcessor::queue_map.end();  )
	{
		Queue* q = mitr->second;
		if( q && q->size() == 0)
		{
			if( !FileProcessor::fetch_records( mitr->first) )
			{
				mitr = FileProcessor::queue_map.erase(mitr);
				continue;
			}
		}
		mitr++;
	}
}


void FileProcessor::fetch_records_for_queue(Queue* current_queue)
{
	
	if( current_queue )
	{
		NewRecord* r = current_queue->front();
		string sym;
		if( !r )
		{
			cout << "Queueu null record\n";
			return;
		}
		if( r->get_type() == NewRecord::QUOTE )
		{
			sym = (dynamic_cast<QuoteNewRecord*>(r))->m_fields.symbol;
		}
		else if( r->get_type() == NewRecord::TRADE )
		{
			sym = (dynamic_cast<TradeNewRecord*>(r))->m_fields.symbol;
		}
		else if( r->get_type() == NewRecord::SIGNAL )
		{
			sym = (dynamic_cast<SignalNewRecord*>(r))->m_fields.symbol;
		}
		else
		{
			cout << "INVALID record\n";
			return;
		}
	
		FileProcessor::fetch_records( sym );
	}
}

bool FileProcessor::fetch_records(const string _sym)
{
	if( FileProcessor::save_write_blocks.count( _sym )  )
	{
		WriteChunks* write_blocks = FileProcessor::save_write_blocks[ _sym ];
		if( !write_blocks  || ( write_blocks && !write_blocks->size() ) )
		{
			cout << "No records available for symbol : " << _sym << "\n";
			FileProcessor::save_write_blocks.erase( _sym );
			return false;
		}
		//cout << _sym << " : read block size : " << write_blocks->size() << "\n";
		
		const pair < int, int > fd = write_blocks->front();
		write_blocks->pop();
		if( write_blocks->size() == 0 )
		{
			delete write_blocks;
			FileProcessor::save_write_blocks.erase( _sym );
		}
		ifstream in(_sym +  ".binary", ios::in|ios::binary );
		in.seekg(fd.first,ios::beg);
		char *read_data(0);
		if( fd.second <= maxblocksize )
		{
			read_data = FileProcessor::maxblock;
			//memset(read_data, 0, fd.second );
		}
		
		if (! in.eof() )
		{
			in.read(read_data, fd.second);
		
			if( in ){}
			else
			{
				cout << "read only " << in.gcount() << "\n";
			}

			FileProcessor::fetch_records( fd.second, read_data, _sym );
		}
		in.close();
		//delete[] read_data;
		return true;
	}
	else
	{
		return false;
	}	
	
	return false;
}




void FileProcessor::fetch_records( int len, char* read_data, const string& _sym )
{
	if( FileProcessor::queue_map.count( _sym ) == 0 )
	{
		Queue* q = new Queue();
		FileProcessor::queue_map[ _sym ] = q;
	}
	Queue* q = FileProcessor::queue_map[ _sym ];
	char r;
	
	while( len > 0 )
	{
		r = *read_data;
		//cout << "RECORD : " << r << "\n";
		unsigned int record_len;
		--len;
		
		if ( FileProcessor::recordlengthmap.count(r) )
		{
			record_len = FileProcessor::recordlengthmap[r];
			
		}
		else
		{
			cout << "len : " << len << "\n";
			break;
		}
	
		if( len - record_len < 0 )
		{
			break;
		}
		read_data += sizeof(char);
		
		if( r == 'Q' )
		{
			q->push ( QuoteNewRecord::deserialize(read_data, record_len) );
		}
		else if( r == 'T' )
		{
			q->push( TradeNewRecord::deserialize(read_data, record_len) );
		}
		else if( r == 'S')
		{
			q->push( SignalNewRecord::deserialize( read_data, record_len) );
		}
		else
		{
			cout << "Invalid record in deserialize\n";
		}
		read_data += record_len;
		len -= record_len;
	}
	
}

void FileProcessor::fetchrecord( const string _file )
{
	ifstream in(_file + ".binary", ios::binary );
	char r;

	while( !in.eof() )
	{
		in.read(&r, sizeof(char));
		//cout << r<< " ";
	
		unsigned int record_len = FileProcessor::recordlengthmap[ r ];
	
		if( in.eof() )
		{
			break;
		}
		char *data = new char[record_len];
		in.read( data, record_len);
	
		if( r == 'Q' )
		{
			QuoteNewRecord::deserialize(data, record_len);
		}
		else if( r == 'T' )
		{
			TradeNewRecord::deserialize(data, record_len);
		}
		else if( r == 'S')
		{
			SignalNewRecord::deserialize( data, record_len);
		}
		else
		{
			cout << "Invalid record in deserialize\n";
		}
		delete data;
	}
	in.close();
}
