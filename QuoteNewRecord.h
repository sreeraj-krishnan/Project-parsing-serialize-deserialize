#ifndef __QUOTE_NEW_RECORD__H
#define __QUOTE_NEW_RECORD__H

#include "NewRecord.h"
#include<fstream>

class QuoteNewRecord : public NewRecord
{
 public:
    struct Fields 
    {
        ulong sequence_id;
        string datetime;
        string symbol;
        double bid;
        double ask;
        int bsize;
        int asize;
        Fields();
		Fields(const ulong _seq, string d, string sy, double bid, double ask, int bsize, int asize);
		
		static const unsigned short date_len;
		static const unsigned short symbol_len;
		static const unsigned short record_length;
		
		friend std::ostream& operator<<(std::ostream& out, const Fields& f);
    };

    
    Fields m_fields;
    
    public:
    
    QuoteNewRecord(const QuoteNewRecord::Fields& _f);
   
	void serialize() const;
	//unsigned long get_seq_id();
	void print(ofstream& out) const;
	
	static QuoteNewRecord* deserialize(char* data, const unsigned int len);
	
	friend std::ostream& operator<<(std::ostream& out, const QuoteNewRecord& f);
	
};

#endif
