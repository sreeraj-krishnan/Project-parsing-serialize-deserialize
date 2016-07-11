#include "TradeNewRecord.h"
#include "NewRecord.h"
#include "FileProcessor.h"
#include<stdio.h>
#include<string.h>

// include serialization processor

const unsigned short TradeNewRecord::Fields::date_len      = 24;
const unsigned short TradeNewRecord::Fields::symbol_len    = 5;
const unsigned short TradeNewRecord::Fields::record_length = sizeof(unsigned long) +TradeNewRecord::Fields::date_len + TradeNewRecord::Fields::symbol_len
															 + sizeof(double) + sizeof(char); // 24 = datetime , 5 = symbol


TradeNewRecord::Fields::Fields(): sequence_id(0), price(0.0), condition(0)
{
	datetime.reserve(24);
	symbol.reserve(5);
}


TradeNewRecord::Fields::Fields(const unsigned long _seq, string _d, string _sy, double _price, char _condition ):
		sequence_id(_seq),
		datetime(_d),
		symbol(_sy),
		price(_price),
		condition(_condition)
{
}

TradeNewRecord::TradeNewRecord(const TradeNewRecord::Fields& _f) : NewRecord( NewRecord::TRADE ), m_fields(_f)
{
	m_seq_id=m_fields.sequence_id;
}



void TradeNewRecord::serialize() const
{
	static char save[ Fields::record_length ];
	char* data=save;
	
	memcpy(data, m_fields.datetime.c_str(), TradeNewRecord::Fields::date_len );
	data += TradeNewRecord::Fields::date_len;
	
	memcpy(data, m_fields.symbol.c_str(), TradeNewRecord::Fields::symbol_len );
	data += TradeNewRecord::Fields::symbol_len;
	
	memcpy(data,reinterpret_cast<const double*>(&m_fields.price), sizeof(double));
	data += sizeof(double);
	
	memcpy(data,reinterpret_cast<const char*>(&m_fields.condition), sizeof(char));
	data += sizeof(char);
		
	memcpy(data,reinterpret_cast<const unsigned long*>(&m_fields.sequence_id), sizeof(unsigned long));
	
	FileProcessor::AddDataToFile( m_fields.symbol, 'T', save, TradeNewRecord::Fields::record_length );
	
}


TradeNewRecord* TradeNewRecord::deserialize(char* data, const unsigned int len)
{
	if( Fields::record_length != len )
	{
		cout << "TradeNewRecord length does not match";
		
	}
	static char datetime[ TradeNewRecord::Fields::date_len ];
	static char symbol[   TradeNewRecord::Fields::symbol_len ];
	static double price(0.0);
	static char condition;
	static unsigned long sequence_id(0);
	
	memcpy(datetime,data, TradeNewRecord::Fields::date_len);
	data += TradeNewRecord::Fields::date_len;
	
	memcpy(symbol,data,TradeNewRecord::Fields::symbol_len);
	data += TradeNewRecord::Fields::symbol_len;
	
	memcpy(reinterpret_cast<double*>(&price), data, sizeof(double));
	data += sizeof(double);
	
	memcpy(reinterpret_cast<char*>(&condition), data, sizeof(char));
	data += sizeof(char);
	
	memcpy(reinterpret_cast<void*>(&sequence_id), data, sizeof(unsigned long));

	return new TradeNewRecord(TradeNewRecord::Fields(sequence_id,datetime, symbol, price,condition ));

}


std::stringstream& operator<<(std::stringstream& out, const TradeNewRecord::Fields& f)
{
	out.precision(8);
	out << f.datetime << "," << f.symbol << "," << f.price << ",";
	if( f.condition != 0 )
	{
		out<<f.condition;
	}
	out << "\n";
	
	return out;
}
std::stringstream& operator<<(std::stringstream& out, const TradeNewRecord& f)
{
	out << f.m_fields;
	return out;
}

std::ostream& operator<<(std::ostream& out, const TradeNewRecord::Fields& f)
{
	out.precision(8);
	out << f.datetime << "," << f.symbol << "," << f.price << ",";
	if( f.condition != 0 )
	{
		out<<f.condition;
	}
	out << std::endl;
	
	return out;
}
std::ostream& operator<<(std::ostream& out, const TradeNewRecord& f)
{
	out << f.m_fields;
	return out;
}


void TradeNewRecord::print(stringstream& out) const
{
	out << *this;
}

void TradeNewRecord::print(ofstream& out) const
{
	out << *this;
}
