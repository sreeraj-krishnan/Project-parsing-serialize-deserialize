#ifndef __SIGNAL_NEW_RECORD__H
#define __SIGNAL_NEW_RECORD__H

#include "NewRecord.h"


class SignalNewRecord : public NewRecord
{
 public:
    struct Fields 
    {
        unsigned long sequence_id;
        string datetime;
        string symbol;
        double value;
        int code;
        
        Fields();
        Fields(const ulong _seq, string _date, string _sym, double _value, int _code);
		
		static const unsigned short date_len;
		static const unsigned short symbol_len;
		static const unsigned short record_length;
		
		friend std::ostream& operator<<(std::ostream& out, const Fields& f);
    };
	

    Fields m_fields;
    
    SignalNewRecord(const SignalNewRecord::Fields& _f);

    void serialize() const;
	//unsigned long get_seq_id() ;
	void print(ofstream& out) const;
	
	static SignalNewRecord* deserialize(char* data, const unsigned int len);
	friend std::ostream& operator<<(std::ostream& out, const SignalNewRecord& f);

};

#endif