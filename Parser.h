#ifndef __PARSER__H
#define __PARSER__H

#include <string>
#include<vector>

using namespace std;

class Parser
{
    string m_filename;
    enum ParserType { CSV = 0 };
    public:
        Parser( const string _filename );
        
        void parse_file();
        void parse_records(const vector<string>& _lines,  unsigned long& sequence);
        void parse_record(const string& line, const unsigned long& _sequence);
        const vector<string> get_tokens_by_delim(const string& input, const string& delim);
        
    static const string ORIG_DELIM;
};

#endif
