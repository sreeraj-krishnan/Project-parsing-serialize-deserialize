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
        Fields(ulong& _seq, string& _date, string& _sym, double& _value, int& _code);
    };
	

    Fields m_fields;
    
    SignalNewRecord(const SignalNewRecord::Fields& _f);

    void serialize(std::ostream& _out) const;

};

#endif