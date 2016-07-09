#include "SignalNewRecord.h"
#include "NewRecord.h"
#include "FileProcessor.h"
#include<stdio.h>
#include<string.h>

// include processor for serialization

const unsigned short SignalNewRecord::Fields::date_len      = 24;
const unsigned short SignalNewRecord::Fields::symbol_len    = 5;
const unsigned short SignalNewRecord::Fields::record_length = sizeof(unsigned long) +SignalNewRecord::Fields::date_len + SignalNewRecord::Fields::symbol_len
															 + sizeof(double) + sizeof(int); // 24 = datetime , 5 = symbol


SignalNewRecord::Fields::Fields(): value(0.0), code(0)
	{
		datetime.reserve(24);
		symbol.reserve(5);
	}
SignalNewRecord::Fields::Fields(const ulong _seq, string _date, string _sym, double _value, int _code):
		sequence_id(_seq),
		datetime(_date),
		symbol(_sym),
		value(_value),
		code(_code)
{

}

SignalNewRecord::SignalNewRecord(const SignalNewRecord::Fields& _f) : NewRecord( NewRecord::SIGNAL ), m_fields(_f)
{
		m_seq_id=m_fields.sequence_id;
		//NewRecord::srec++;
}


void SignalNewRecord::serialize() const
{
	static char save[ Fields::record_length ];
	char* data=save;
	
	memcpy(data, m_fields.datetime.c_str(), SignalNewRecord::Fields::date_len );
	data += SignalNewRecord::Fields::date_len;
	
	memcpy(data, m_fields.symbol.c_str(), SignalNewRecord::Fields::symbol_len );
	data += SignalNewRecord::Fields::symbol_len;
	
	memcpy(data,reinterpret_cast<const double*>(&m_fields.value), sizeof(double));
	data += sizeof(double);
	
	memcpy(data,reinterpret_cast<const char*>(&m_fields.code), sizeof(int));
	data += sizeof(int);
		
	memcpy(data,reinterpret_cast<const unsigned long*>(&m_fields.sequence_id), sizeof(unsigned long));
	
	FileProcessor::AddDataToFile( m_fields.symbol, 'S', save, SignalNewRecord::Fields::record_length );
	
	//delete save;
}


SignalNewRecord* SignalNewRecord::deserialize(char* data, const unsigned int len)
{
	if( Fields::record_length != len )
	{
		cout << "SignalNewRecord length does not match";
	}
	char datetime[ SignalNewRecord::Fields::date_len ];
	char symbol[   SignalNewRecord::Fields::symbol_len ];
	double value(0.0);
	int code(0);
	unsigned long sequence_id(0);
	
	memcpy(datetime,data, SignalNewRecord::Fields::date_len);
	data += SignalNewRecord::Fields::date_len;
	
	memcpy(symbol,data,SignalNewRecord::Fields::symbol_len);
	data += SignalNewRecord::Fields::symbol_len;
	
	memcpy(reinterpret_cast<void*>(&value), data, sizeof(double));
	data += sizeof(double);
	
	memcpy(reinterpret_cast<void*>(&code), data, sizeof(int));
	data += sizeof(int);
	
	memcpy(reinterpret_cast<void*>(&sequence_id), data, sizeof(unsigned long));

	
	return new SignalNewRecord(SignalNewRecord::Fields( sequence_id, datetime, symbol, value, code));

}

std::stringstream& operator<<(std::stringstream& out, const SignalNewRecord::Fields& f)
{
	out.precision(8);
	//out << f.sequence_id << "," << f.datetime << "," << f.symbol << "," << f.value << "," <<f.code<<"\n";
	out << f.datetime << "," << f.symbol << "," << f.value << "," <<f.code << "\n";
	
	return out;
}
std::stringstream& operator<<(std::stringstream& out, const SignalNewRecord& f)
{
	out << f.m_fields;
	return out;
}


std::ostream& operator<<(std::ostream& out, const SignalNewRecord::Fields& f)
{
	out.precision(8);
	//out << f.sequence_id << "," << f.datetime << "," << f.symbol << "," << f.value << "," <<f.code<<"\n";
	out << f.datetime << "," << f.symbol << "," << f.value << "," <<f.code << std::endl;
	
	return out;
}
std::ostream& operator<<(std::ostream& out, const SignalNewRecord& f)
{
	out << f.m_fields;
	return out;
}

void SignalNewRecord::print(stringstream& out) const
{
	out << *this;
}

void SignalNewRecord::print(ofstream& out) const
{
	out << *this;
}
