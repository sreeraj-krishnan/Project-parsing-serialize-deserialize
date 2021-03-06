#include "QuoteNewRecord.h"
#include "NewRecord.h"
#include "FileProcessor.h"

#include<stdio.h>
#include<string.h>

const unsigned short QuoteNewRecord::Fields::date_len      = 24;
const unsigned short QuoteNewRecord::Fields::symbol_len    = 5;
const unsigned short QuoteNewRecord::Fields::record_length = sizeof(unsigned long) +QuoteNewRecord::Fields::date_len + QuoteNewRecord::Fields::symbol_len
															 + sizeof(double)*2 + sizeof(int)*2 ; // 24 = datetime , 5 = symbol

QuoteNewRecord::Fields::Fields(): bid(0.0), ask(0.0),bsize(0),asize(0)
{
	datetime.reserve(24);
	symbol.reserve(5);
}

QuoteNewRecord::Fields::Fields(const ulong _seq, string _d, string _sy, double _bid, double _ask, int _bsize, int _asize):
		sequence_id(_seq),
		datetime(_d),
		symbol(_sy),
		bid(_bid),
		ask(_ask),
		bsize(_bsize),
		asize(_asize)
{
}

QuoteNewRecord::QuoteNewRecord(const QuoteNewRecord::Fields& _f) : NewRecord( NewRecord::QUOTE ), m_fields(_f)
{
		//NewRecord::qrec++;
		m_seq_id=m_fields.sequence_id;
}


void QuoteNewRecord::serialize() const
{
	static char save[ Fields::record_length ];
	char* data=save;
	
	memcpy(data, m_fields.datetime.c_str(), QuoteNewRecord::Fields::date_len );
	data += QuoteNewRecord::Fields::date_len;
	
	memcpy(data, m_fields.symbol.c_str(), QuoteNewRecord::Fields::symbol_len );
	data += QuoteNewRecord::Fields::symbol_len;
	
	memcpy(data,reinterpret_cast<const double*>(&m_fields.bid), sizeof(double));
	data += sizeof(double);
	
	memcpy(data,reinterpret_cast<const double*>(&m_fields.ask), sizeof(double));
	data += sizeof(double);
	
	memcpy(data,reinterpret_cast<const int*>(&m_fields.bsize), sizeof(int));
	data += sizeof(int);
	
	memcpy(data,reinterpret_cast<const int*>(&m_fields.asize), sizeof(int));
	data += sizeof(int);
	
	memcpy(data,reinterpret_cast<const unsigned long*>(&m_fields.sequence_id), sizeof(unsigned long));
	
	FileProcessor::AddDataToFile( m_fields.symbol, 'Q', save, QuoteNewRecord::Fields::record_length );
	
}

QuoteNewRecord* QuoteNewRecord::deserialize(char* data, const unsigned int len)
{
	if( Fields::record_length != len )
	{
		cout << "QuoteNewRecord length does not match";
	}
	static char datetime[ QuoteNewRecord::Fields::date_len ];
	static char symbol[   QuoteNewRecord::Fields::symbol_len ];
	static double bid(0.0), ask(0.0);
	static int bsize(0),asize(0);
	static unsigned long sequence_id(0);
	
	memcpy(datetime,data, QuoteNewRecord::Fields::date_len);
	data += QuoteNewRecord::Fields::date_len;
	
	memcpy(symbol,data,QuoteNewRecord::Fields::symbol_len);
	data += QuoteNewRecord::Fields::symbol_len;
	
	memcpy(reinterpret_cast<void*>(&bid), data, sizeof(double));
	data += sizeof(double);
	
	memcpy(reinterpret_cast<void*>(&ask), data, sizeof(double));
	data += sizeof(double);
	
	memcpy(reinterpret_cast<void*>(&bsize), data, sizeof(int));
	data += sizeof(int);
	
	memcpy(reinterpret_cast<void*>(&asize), data, sizeof(int));
	data += sizeof(int);
	
	memcpy(reinterpret_cast<void*>(&sequence_id), data, sizeof(unsigned long));


	return new QuoteNewRecord(QuoteNewRecord::Fields( sequence_id, datetime, symbol, bid, ask, bsize, asize ));
    			
}

std::stringstream& operator<<(std::stringstream& out, const QuoteNewRecord::Fields& f)
{
	out.precision(8);
	out << f.datetime << "," << f.symbol << "," << f.bid << "," <<f.ask << "," << f.bsize << "," << f.asize<<std::endl;
	
	return out;
}


std::stringstream& operator<<(std::stringstream& out, const QuoteNewRecord& f)
{
	out << f.m_fields;
	return out;
}

void QuoteNewRecord::print(ofstream& out) const
{
	out << *this;
}
void QuoteNewRecord::print(stringstream& out) const
{
	out << *this;
}

std::ostream& operator<<(std::ostream& out, const QuoteNewRecord::Fields& f)
{
	out.precision(8);
	out << f.datetime << "," << f.symbol << "," << f.bid << "," <<f.ask << "," << f.bsize << "," << f.asize<<"\n";
	
	return out;
}


std::ostream& operator<<(std::ostream& out, const QuoteNewRecord& f)
{
	out << f.m_fields;
	return out;
}
