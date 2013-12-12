
#ifndef CONFIG_H
#define CONFIG_H 


#include "./src/ciscoctrl.h"
#include "./src/codelog.h"
#include "./src/ini.h"



#include <stdlib.h>
#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>

#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#ifdef POSIX
#include <termios.h>
#endif


#define TELNET_PORT "23"

// just for test
#define CTRL_USER "admin\r"
#define CTRL_PASSWD "sonic\r"
#define CTRL_LOGOUT "logout\r"

struct ConfigFile
{
    int version;
    const char* name;
    const char* email;
};

extern class CodeLog *pLog;







#endif