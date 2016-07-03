#include "SignalNewRecord.h"
#include "NewRecord.h"

// include processor for serialization

SignalNewRecord::Fields::Fields(): value(0.0), code(0)
	{
		datetime.reserve(24);
		symbol.reserve(5);
	}
SignalNewRecord::Fields::Fields(ulong& _seq, string& _date, string& _sym, double& _value, int& _code):
		sequence_id(_seq),
		datetime(_date),
		symbol(_sym),
		value(_value),
		code(_code)
{
	datetime.reserve(24);
	symbol.reserve(5);
}

SignalNewRecord::SignalNewRecord(const SignalNewRecord::Fields& _f) : NewRecord( NewRecord::SIGNAL ), m_fields(_f)
{
		// register for serialization
}

void SignalNewRecord::serialize(std::ostream& _out) const
{
	
}