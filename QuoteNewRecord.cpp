#include "QuoteNewRecord.h"
#include "NewRecord.h"


QuoteNewRecord::Fields::Fields(): bid(0.0), ask(0.0),bsize(0),asize(0)
{
	datetime.reserve(24);
	symbol.reserve(5);
}

QuoteNewRecord::Fields::Fields(ulong& _seq, string& _d, string& _sy, double& _bid, double& _ask, int& _bsize, int& _asize):
		sequence_id(_seq),
		datetime(_d),
		symbol(_sy),
		bid(_bid),
		ask(_ask),
		bsize(_bsize),
		asize(_asize)
{
	datetime.reserve(24);
	symbol.reserve(5);
}

QuoteNewRecord::QuoteNewRecord(const QuoteNewRecord::Fields& _f) : NewRecord( NewRecord::QUOTE ), m_fields(_f)
{
		// register for serialization
}

void QuoteNewRecord::serialize(std::ostream& _out) const
{
	
}