#include "NewRecord.h"
#include "QuoteNewRecord.h"
#include "TradeNewRecord.h"
#include "SignalNewRecord.h"
#include "FileProcessor.h"

#include<vector>
#include<iostream>
#include<locale>

SerializeList NewRecord::serial_list;



NewRecord::NewRecord(Type _type,const unsigned long _seq): m_type(_type), m_seq_id(_seq)
{
}

NewRecord::~NewRecord()
{
	// virtual destructor
}

unsigned long NewRecord::get_seq_id()
{
		return m_seq_id;
}

void NewRecord::CreateRecords(const vector<string>& tokens , const unsigned long& seq)
{
	std::locale loc;
    // identify record type
	// pass the record and then serialize and destruct
  try {
    if( tokens.size() == 6 )
    {
		QuoteNewRecord::Fields f(seq,tokens[0],tokens[1],std::stod(tokens[2]),std::stod(tokens[3]),std::stoi(tokens[4]),std::stoi(tokens[5]));
		registerForSerialization ( new QuoteNewRecord(f) );
		
	}
    else if( tokens.size() == 3 )
    {
		TradeNewRecord::Fields f( seq,tokens[0],tokens[1],std::stod(tokens[2]),0);
		registerForSerialization ( new TradeNewRecord(f) );
	}
    else if ( tokens.size() == 4 )
    {
        string item4 = tokens[3];
        if ( std::isalpha(item4[0],loc) )
        {
			TradeNewRecord::Fields f( seq,tokens[0],tokens[1],std::stod(tokens[2]),tokens[3].at(0));
			registerForSerialization ( new TradeNewRecord(f) );
        }
        else
        {
			SignalNewRecord::Fields f(seq,tokens[0],tokens[1],std::stod(tokens[2]),std::stoi(tokens[3]));
			registerForSerialization ( new SignalNewRecord(f) );
        }
        
    }
  }
  catch(const std::exception& ex)
  {
	cout << "exception caught : " << ex.what() <<  " " <<  seq <<"\n";
#if 0
	for( int i=0; i < tokens.size(); i++ )
		cout << tokens[i] << " ";
	cout << "\n";
#endif
  }
}

void NewRecord::registerForSerialization( NewRecord* record )
{
	NewRecord::serial_list.push_back( record );
}

void NewRecord::dispatchForSerialization()
{
	if( !FileProcessor::beg )
	{
		FileProcessor::beg = clock();
		cout << "Serialization begin\n";
	}
	SerializeList::iterator itr = NewRecord::serial_list.begin();
	for( ; itr != NewRecord::serial_list.end(); ++itr )
	{
		(*itr)->serialize();
		delete (*itr);
	}
	NewRecord::serial_list.erase( NewRecord::serial_list.begin(), NewRecord::serial_list.end() );
}
