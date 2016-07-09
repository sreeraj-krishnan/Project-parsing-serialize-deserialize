#include "Parser.h"
#include "NewRecord.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <locale>

using namespace std;

const string Parser::ORIG_DELIM(",");
const int MAX_TOKENS = 8;
int Parser::FLUSH_LIMIT = 128000;
unsigned int Parser::FILE_READ_BLOCK = 16 * 1024 * 1024; // 16 MB

Parser::Parser(const string _filename): m_filename(_filename )
{
    start = clock();
}


const vector<string> Parser::get_tokens_by_delim(const string& input, const string& delim)
{
    string::const_iterator it=input.begin();
    string tokn;
    vector<string> tokens;
    tokens.reserve( MAX_TOKENS );
    for( ; it != input.end(); it++ )
    {
        if( int(*it) == 13 ) // CR
        {
                continue;
        }
        if ( *it == delim[0] )
        {
            //cout << tokn << " ";
            tokens.push_back( tokn );
            tokn.clear();
        }
        else
        {
            tokn += *it;
        }
    }
    if( ! tokn.empty() )
    {
        tokens.push_back(tokn);
    }
    return tokens;
}

void Parser::parse_records(const vector<string>& _lines, unsigned long& sequence)
{
    vector<string>::const_iterator itr = _lines.begin();
	
    for( static unsigned long rec_count=1; itr != _lines.end(); ++itr )
    {
        parse_record( *itr ,rec_count );
        rec_count++;
    }
    
}

void Parser::parse_record(const string& line, const unsigned long& sequence)
{
    const vector<string> tokens = get_tokens_by_delim( line, ORIG_DELIM);
	NewRecord::CreateRecords( tokens, sequence  );
}

void Parser::parse_record(const string& line )
{
	static unsigned long seq=1;
    const vector<string> tokens = get_tokens_by_delim( line, ORIG_DELIM);
	NewRecord::CreateRecords( tokens, seq++ );
}

void Parser::parse_file()
{
   ifstream file( m_filename );
   string line;        
   vector < string > lines; 
   lines.reserve(Parser::FLUSH_LIMIT);     
   unsigned long linecount(0);
   char* block = new char[ FILE_READ_BLOCK + 100 ];
   short index;
   unsigned long blocklen(0);
   char* linebegin(0);
   
   while( !file.eof() )
   {
	   file.read( block, FILE_READ_BLOCK);
	   index=0;
	   if( file )
	   {
			char c;
			while( !file.eof() )
			{
				file.get(c);
				block[FILE_READ_BLOCK + index++] = c;
				if( c =='\n')
					break;
				
			}
			blocklen = FILE_READ_BLOCK + index;
	   }
	   else
	   {
		   blocklen = file.gcount();
	   }
	   int j;
	   for( int i(0); i < blocklen; i++)
	   {
		   j = i;
		   linebegin = block + j;
		   
		   for( ; block[j] != '\n' && j < blocklen; j++ );
		   
		   lines.push_back( string(linebegin, j-i ) );
		   
		   if( linecount % Parser::FLUSH_LIMIT == 0 )
		   {
			   	parse_records(lines, linecount);
				lines.clear();
				NewRecord::dispatchForSerialization();
		   }
		   i = j;
	   }
	}
   delete[] block;
   NewRecord::dispatchForSerialization();
   file.close();
   cout << "Parsing complete, time taken : " << (clock()-start)/double(CLOCKS_PER_SEC) << " seconds.\n";
}
