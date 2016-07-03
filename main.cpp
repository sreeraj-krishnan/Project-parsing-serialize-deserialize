#include <iostream>
#include "Parser.h"
#include "Record.h"

using namespace std;

int main()
{
    Parser file( string("q.csv")) ;
    file.parse_file();
    Record::flush_all();
  
    return 0;
}

