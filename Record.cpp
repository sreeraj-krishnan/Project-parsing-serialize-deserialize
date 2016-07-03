#include "Record.h"
#include<iostream>
#include<fstream>
#include<string>
#include<typeinfo>

using namespace std;

SymbolRecords Record::m_records;
SymbolFiles Record::s_files;



const int FLUSH_LIMIT = 32;

Record::Record(){}

Record::Record(Type _type,  const string& _symbol,const unsigned long& _seq, Record* _derived):
    m_type( _type ),
    m_symbol( _symbol ),
    m_seq( _seq)
{
  
    if( m_records.count(m_symbol) == 0 )
    {
        SymList *list = new SymList();
        //cout << " this : " << typeid(*_derived).name() << "\n";
        list->push_back( _derived );
        m_records[ m_symbol ] = list;
        s_files.push_back( string(m_symbol + ".binary") );
    }
    else
    {
        SymList* list = m_records[ m_symbol ];
        list->push_back(_derived);
        m_records[ m_symbol ] = list;
        if( list->size() >= FLUSH_LIMIT )
        {
            Record::flush(m_symbol);
        }
    }
}


void Record::flush(const string& sym)
{
    //cout << "in flush \n";
    SymList* list = Record::m_records[sym];
    SymList::iterator itr = list->begin();
    string outbuf;
    string folder="SYMBOL/";
    ofstream file(sym + ".binary" , ios::binary);
    if ( !file.is_open() )
    {
        cout << "error opening file : " << sym + ".txt" << "\n";
        return;
    }

    for( ; itr != list->end(); itr++ )
    {
        (*itr)->write( file );
        
        //delete (*itr);
    }
    //cout << outbuf;
    list->clear();
    Record::m_records[sym] = list;
    file.close();
}

void Record::flush_all()
{
    for( SymbolRecords::iterator itr = Record::m_records.begin(); itr != Record::m_records.end(); itr++)
    {
        Record::flush( (*itr).first );
    }
}

Record::~Record()
{
   
}

QuoteRecord::QuoteRecord(const vector<string>& tokens, const unsigned long& seq ) :
        Record( Type::QUOTE, tokens[1], seq, this ), 
        m_fields( tokens, seq)
{   
    //cout << "constructor \n";
    //cout << " ser : " << this->serialize() << "\n";
}
TradeRecord::TradeRecord(const vector<string>& tokens, const unsigned long& seq ) :
        Record( Type::TRADE, tokens[1], seq , this),
        m_fields( tokens, seq)
{
   //  cout << " ser : " << this->serialize() << "\n";
}
SignalRecord::SignalRecord(const vector<string>& tokens, const unsigned long& seq ) : 
        Record( Type::SIGNAL, tokens[1], seq , this) , 
        m_fields( tokens, seq)
{
  // cout << " ser : " << this->serialize() << "\n";
}



#if 1
QuoteRecord::Fields::Fields(const vector<string>& _t, const unsigned long& seq):
        sequence_id(seq),
        datetime(_t[0]),
        symbol(_t[1]),
        bid( std::stod (_t[2]) ),
        ask( std::stod (_t[3]) )
        
{
    try
    {
        bsize =  std::stoi(_t[4]) ;
        asize = std::stoi(_t[5]);
    }
    catch(const std::exception& ex)
    {
        cout << "err : " << ex.what() << "\n";
    }
}
 

TradeRecord::Fields::Fields(const vector<string>& _t , const unsigned long& seq):
        sequence_id(seq),
        datetime(_t[0]),
        symbol(_t[1]),
        price( std::stod (_t[2]) ),
        condition(0)
        
{
   if(_t.size() == 4)
   {
       condition = _t[3].at(0);
   }
}


SignalRecord::Fields::Fields(const vector<string>& _t , const unsigned long& seq):
        sequence_id(seq),
        datetime(_t[0]),
        symbol(_t[1]),
        value( std::stod( _t[2] ) ),
        code(0)
       
{
    if( _t.size() == 4  )
    {
        try {
            code = stoi( _t[3] );
        }
        catch (const std::exception& ia) {
                cout << " err : " << ia.what() << "\n";
              
        }
            
    }
    
}

#endif

void Record::set_symbol(const string& _sym)
{
    m_symbol = _sym; 
    
}
  
    
string QuoteRecord::serialize()
{
    string s;
    try {
        s += std::to_string( m_fields.sequence_id) + ",";
        s += m_fields.datetime + ",";
        s += m_fields.symbol + ",";
        s += std::to_string( m_fields.bid) + ",";
        s += std::to_string( m_fields.ask) + ",";
        s += std::to_string( m_fields.bsize) + ",";
        s += std::to_string( m_fields.asize);
        
        
    }
    catch(const std::exception& ex)
    {
        cout << "err : " << ex.what() << "\n";
    }
    return s + "\n";
}
    
void QuoteRecord::deserialize()
{
    
}
  
string SignalRecord::serialize()
{
    string s;
    try {
      s += std::to_string( m_fields.sequence_id) + ",";
      s += m_fields.datetime + ",";
      s +=  m_fields.symbol + ",";
       s += std::to_string( m_fields.value) + ",";
       s += std::to_string( m_fields.code );
    }
    catch(const std::exception& ex)
    {
        cout << "err : " << ex.what() << "\n";
    }
    return s + "\n";
}
    
void SignalRecord::deserialize()
{
    
}


string TradeRecord::serialize()
{
    string s;
    try {
      s += std::to_string( m_fields.sequence_id) + ",";
        s += m_fields.datetime + ",";
        s +=  m_fields.symbol + ",";
        s += std::to_string( m_fields.price) + ",";
        if( m_fields.condition )
        {
            s += m_fields.condition;
        }
    }
    catch(const std::exception& ex)
    {
        cout << "err : " << ex.what() << "\n";
    };

    return s + "\n";
}
    
void TradeRecord::deserialize()
{
    
}

string Record::serialize()
{
        return string();
}
    
void Record::deserialize()
{
    
}


#if 1



// Read from SYM file
Record* Record::read( ifstream& in )
{
  char rec;
  in >> rec;
  if( rec == 'S')
  {
      static SignalRecord s;
      return s.read(in);
  }
  else if ( rec == 'Q')
  {
     static QuoteRecord q;
     return q.read(in);
  }
  else if( rec == 'T')
  {
      static TradeRecord t;
      return t.read(in);
  }
  return 0;
  
}

SignalRecord* SignalRecord::read( ifstream& in )
{
    SignalRecord::Fields f;
    in.read(reinterpret_cast<char*>(&f), sizeof(SignalRecord::Fields));
    return new SignalRecord(f);
}
QuoteRecord* QuoteRecord::read( ifstream& in )
{
    QuoteRecord::Fields f;
    in.read(reinterpret_cast<char*>(&m_fields), sizeof(QuoteRecord::Fields));
    return new QuoteRecord(f);
}

TradeRecord* TradeRecord::read( ifstream& in)
{
    TradeRecord::Fields f;
  	in.read(reinterpret_cast<char*>(&f), sizeof(TradeRecord::Fields));
    return new TradeRecord(f);    
}


void TradeRecord::write(ofstream& out)
{
  	out << 'T';
	out.write(reinterpret_cast<char*>(&m_fields), sizeof(TradeRecord::Fields));
}


void QuoteRecord::write(ofstream& out)
{
  	out << 'Q';
	out.write(reinterpret_cast<char*>(&m_fields), sizeof(QuoteRecord::Fields));
}

void SignalRecord::write(ofstream& out)
{
  	out << 'S';
	out.write(reinterpret_cast<char*>(&m_fields), sizeof(SignalRecord::Fields));
}


#endif
    
SignalRecord::SignalRecord(const SignalRecord::Fields& _f):m_fields(_f)
{
}
TradeRecord::TradeRecord(const TradeRecord::Fields& _f):m_fields(_f)
{
}
QuoteRecord::QuoteRecord(const QuoteRecord::Fields& _f):m_fields(_f)
{
}



