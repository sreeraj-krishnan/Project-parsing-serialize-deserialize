#ifndef __NewRecord__H
#define __NewRecord__H

#include<string>
#include<map>
#include<vector>
#include<fstream>
#include<iostream>
#include<list>
#include<sstream>

using namespace std;

class NewRecord;
typedef unsigned long ulong;

typedef vector<NewRecord*> SerializeList;



class NewRecord
{
    
 public:
    enum Type { QUOTE , TRADE , SIGNAL };
    Type get_type() const { return m_type; }
   
    NewRecord(Type _type,const unsigned long _seq=0);
    virtual ~NewRecord();

	unsigned long get_seq_id(); 
    
    virtual void serialize() const = 0;
    //virtual void print(ofstream& out) const = 0 ;
    virtual void print(stringstream& out) const = 0 ;
	
    static NewRecord* deserialize();
    static void CreateRecords(const vector<string>& tokens , const unsigned long& seq);
	static void registerForSerialization(NewRecord* _rec);
	static void dispatchForSerialization();
	
	static SerializeList serial_list;

	unsigned long m_seq_id;
	bool operator<(const NewRecord* i) { m_seq_id < i->m_seq_id ; }
	
 private:
    Type m_type;
	
};


#endif
