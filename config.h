
#ifndef CONFIG_H
#define CONFIG_H 





#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>

#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>

#include "./src/ciscoctrl.h"
#include "./src/codelog.h"
#include "./src/ini.h"


#ifdef POSIX
#include <termios.h>
#endif


#define TELNET_PORT "23"

// just for test
#define CTRL_USER "admin\r"
#define CTRL_PASSWD "sonic\r"
#define CTRL_LOGOUT "logout\r"
#define CTRL_SHOW_ROGUE_CLIENT_SUMMARY "show rogue client summary\r"
#define CTRL_SHOW_ROGUE_CLIENT_DETAILED "show rogue client detailed  \r"

struct ConfigFile
{
    int version;
    const char* name;
    const char* email;
};

extern class CodeLog *pLog;


//using namespace boost;





#endif