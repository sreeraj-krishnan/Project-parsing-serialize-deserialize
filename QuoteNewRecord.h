#ifndef __QUOTE_NEW_RECORD__H
#define __QUOTE_NEW_RECORD__H

#include "NewRecord.h"


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
		Fields(ulong& _seq, string& d, string& sy, double& bid, double& ask, int& bsize, int&asize);
    };

    
    Fields m_fields;
    
    public:
    
    QuoteNewRecord(const QuoteNewRecord::Fields& _f);
   
	void serialize(std::ostream& _out) const;
};

#endif
