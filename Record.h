#ifndef __RECORD__H
#define __RECORD__H

#include<string>
#include<map>
#include<vector>
#include<fstream>


using namespace std;

class Record;
typedef vector<Record *> SymList;
typedef map< const string, SymList*  > SymbolRecords;
typedef vector< string > SymbolFiles;

class Record
{
    
    public:
    enum Type { QUOTE , TRADE , SIGNAL };
    Type get_type() const { return m_type; }
    
	Record();
    Record(Type _type, const string& _symbol, const unsigned long& _seq, Record* _rec);
    virtual ~Record();
    
    virtual string serialize();
    virtual void deserialize();
    
    void set_symbol(const string& _sym);
    
    static SymbolRecords  m_records;
    static SymbolFiles s_files;
   
    static void flush(const string& sym);
    static void flush_all();
    
#if 1
    virtual void write( ofstream& file) = 0;
    Record* read( ifstream& file );
#endif
    private:
    Type m_type;
    string m_symbol;
    unsigned long m_seq;
    
};

class QuoteRecord : public Record
{
 public:
    struct Fields 
    {
        unsigned long sequence_id;
        string datetime;
        string symbol;
        double bid;
        double ask;
        int bsize;
        int asize;
        Fields(){}
        Fields(const vector<string>& _tokens, const unsigned long& sequence_id );
    };
    Fields m_fields;
    
    public:
    
    QuoteRecord(const vector<string>& tokens, const unsigned long& _seq) ;
    QuoteRecord(const QuoteRecord::Fields& _f=QuoteRecord::Fields());
    string serialize();
    void deserialize();
    
#if 1
    virtual void write(ofstream& file);
    QuoteRecord* read( ifstream& file );
#endif    
};
class TradeRecord : public Record
{
 public:
    struct Fields 
    {
        unsigned long sequence_id;
        string datetime;
        string symbol;
        double price;
        char condition;
        
        Fields(){}
        Fields(const vector<string>& _tokens, const unsigned long& sequence_id );
    };
    Fields m_fields;
    
    string serialize();
    void deserialize();
    
    TradeRecord(const vector<string>& tokens, const unsigned long& _seq) ;
    TradeRecord(const TradeRecord::Fields& _f=TradeRecord::Fields());
#if 1
    virtual void write(ofstream& file);
    TradeRecord* read( ifstream& file );
#endif

};


class SignalRecord : public Record
{
 public:
    struct Fields 
    {
        unsigned long sequence_id;
        string datetime;
        string symbol;
        double value;
        int code;
        
        Fields(){}
        Fields(const vector<string>& _tokens, const unsigned long& sequence_id );
    };
    Fields m_fields;
    
    public:
    string serialize();
    void deserialize();
    SignalRecord(const vector<string>& tokens, const unsigned long& _seq) ; 
    SignalRecord(const SignalRecord::Fields& _f=SignalRecord::Fields());
#if 1
    virtual void write(ofstream& file);
    SignalRecord* read( ifstream& file_name);
#endif
};




#endif