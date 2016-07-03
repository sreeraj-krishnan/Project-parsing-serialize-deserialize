#include "TradeNewRecord.h"
#include "NewRecord.h"

// include serialization processor

TradeNewRecord::Fields::Fields(): sequence_id(0), price(0.0), condition(0)
{
	datetime.reserve(24);
	symbol.reserve(5);
}


TradeNewRecord::Fields::Fields(unsigned long& _seq, string& _d, string& _sy, double& _price, char& _condition ):
		sequence_id(_seq),
		datetime(_d),
		symbol(_sy),
		price(_price),
		condition(_condition)
{
	datetime.reserve(24);
	symbol.reserve(5);
}

TradeNewRecord::TradeNewRecord(const TradeNewRecord::Fields& _f) : NewRecord( NewRecord::TRADE ), m_fields(_f)
{
		// register for serialization
}

void TradeNewRecord::serialize(std::ostream& _out) const
{
	
}