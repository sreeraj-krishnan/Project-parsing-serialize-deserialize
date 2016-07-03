#ifndef __TRADE_NEW_RECORD__H
#define __TRADE_NEW_RECORD__H

#include "NewRecord.h"


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
        Fields(unsigned long& _seq, string& d, string& sy, double& _price, char& _condition );
    };
        

	
	Fields m_fields;
	
    TradeNewRecord(const TradeNewRecord::Fields& _f);

	void serialize(std::ostream& _out) const;

};

#endif
