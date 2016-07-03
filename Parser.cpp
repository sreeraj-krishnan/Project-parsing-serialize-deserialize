#include "Parser.h"
#include "Record.h"

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

Parser::Parser(const string _filename): m_filename(_filename )
{
    
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
  // if( tokens.size() == 3 ) cout << "yes size 3 \n";
    return tokens;
}

void Parser::parse_records(const vector<string>& _lines, unsigned long& sequence)
{
    vector<string>::const_iterator itr = _lines.begin();
    for( ; itr != _lines.end(); ++itr )
    {
        parse_record( *itr ,sequence );
        sequence++;
    }
    
}

void Parser::parse_record(const string& line, const unsigned long& sequence)
{
    
    const vector<string> tokens = get_tokens_by_delim( line, ORIG_DELIM);
    std::locale loc;
    // identify record type
    if( tokens.size() == 6 )
    {
        Record *quote_record = new QuoteRecord( tokens, sequence);
    }
    else if( tokens.size() == 3 )
    {
        Record *trade_record = new TradeRecord(tokens, sequence);
    }
    else if ( tokens.size() == 4 )
    {
        string item4 = tokens[3];
        if ( std::isalpha(item4[0],loc) )
        {
            Record *trade_record = new TradeRecord(tokens, sequence);
        }
        else
        {
            Record *signal_Record = new SignalRecord(tokens, sequence);
        }
        
    }
}

void Parser::parse_file()
{
   ifstream file( m_filename );
   string line;        
   vector < string > lines;
   lines.reserve(1024);
   unsigned long linecount(0);
   
   while( !file.eof() )
   {
       getline(file,line);
       //cout << line.size() << " ";       
       linecount++;
       lines.push_back(line);
       if( linecount % 1024 == 0 )
       {
           parse_records(lines, linecount);
           lines.clear();
       }
   }
   cout << "lines : " << linecount << "\n";
   file.close();
}
