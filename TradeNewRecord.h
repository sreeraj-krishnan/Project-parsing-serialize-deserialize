#ifndef __TRADE_NEW_RECORD__H
#define __TRADE_NEW_RECORD__H

#include "NewRecord.h"
#include<fstream>

class TradeNewRecord : public NewRecord
{
 public:
    struct Fields 
    {
        unsigned long sequence_id;
        string datetime;
        string symbol;
        double price;
        char condition;
        
        Fields();
        Fields(const unsigned long _seq, string d, string sy, double _price, char _condition );
		
		static const unsigned short date_len;
		static const unsigned short symbol_len;
		static const unsigned short record_length;
		
		friend std::ostream& operator<<(std::ostream& out, const Fields& f);
		friend std::stringstream& operator<<(std::stringstream& out, const Fields& f);
    };
        

	
	Fields m_fields;
	
    TradeNewRecord(const TradeNewRecord::Fields& _f);

	void serialize() const;
	//unsigned long get_seq_id();
	void print(ofstream& out) const;
	void print(stringstream& out) const;
	
	static TradeNewRecord* deserialize(char* data, const unsigned int len);
	friend std::stringstream& operator<<(std::stringstream& out, const TradeNewRecord& f);
	friend std::ostream& operator<<(std::ostream& out, const TradeNewRecord& f);

};

#endif
