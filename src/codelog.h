#ifndef CODELOG_H
#define CODELOG_H

#include <fstream>

using namespace std;

class CodeLog
{
  public:
    CodeLog(char* filename);
    ~CodeLog();
    void Write(const char* logline, ...);
  private:
    ofstream m_stream;
};

#endif


