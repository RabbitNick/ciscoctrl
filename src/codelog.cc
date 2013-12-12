
#include <stdarg.h>
#include "codelog.h"
#include <time.h>

CodeLog::CodeLog(char* filename)
{
  m_stream.open(filename);
}

void CodeLog::Write(const char* logline, ...)
{
  va_list argList;
  char cbuffer[1024];
  va_start(argList, logline);
  vsnprintf(cbuffer, 1024, logline, argList);
  va_end(argList);

  char buff[100];
  time_t now = time(0);
  struct tm *sTm;
  sTm = localtime(&now);//gmtime(&now);
  strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);
  
  m_stream << buff;
  m_stream << ": ";
  m_stream << cbuffer << endl;
}

CodeLog::~CodeLog()
{
  m_stream.close();
}


