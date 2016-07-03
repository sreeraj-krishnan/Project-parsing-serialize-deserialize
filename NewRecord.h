#ifndef __NewRecord__H
#define __NewRecord__H

#include<string>
#include<map>
#include<vector>
#include<fstream>
#include<iostream>

using namespace std;

class NewRecord;
typedef unsigned long ulong;

typedef vector<NewRecord*> SymList;
typedef map< const string, SymList*  > SymbolNewRecords;
typedef vector< string > SymbolFiles;

class NewRecord
{
    
 public:
    enum Type { QUOTE , TRADE , SIGNAL };
    
    NewRecord(Type _type);
    virtual ~NewRecord();
    
    virtual void serialize(std::ostream& _out) const = 0;
    static NewRecord* deserialize(std::ifstream& in);
   
    Type get_type() const { return m_type; }
   
 private:
    Type m_type;
};


#endif
