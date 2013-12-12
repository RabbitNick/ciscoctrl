
// this work is for passive localization using cisco 4400 controller

#include "config.h"



class CodeLog *pLog = new class CodeLog((char *)"errors.log");



static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    struct ConfigFile* pconfig = (struct ConfigFile*)user;

    #define MATCH(s, n) strcasecmp(section, s) == 0 && strcasecmp(name, n) == 0
    if (MATCH("protocol", "version")) {
        pconfig->version = atoi(value);
    } else if (MATCH("user", "name")) {
        pconfig->name = strdup(value);
    } else if (MATCH("user", "email")) {
        pconfig->email = strdup(value);
    }
}

static int print_config(const struct ConfigFile config)
{
	std::cout<< config.version << std::endl;
	std::cout<< config.name << std::endl;
	std::cout<< config.email << std::endl;
}

int main(int argc, char* argv[])
{

	struct ConfigFile config_v;


 	pLog->Write("main() start......");


	if (ini_parse("ciscoctrl.ini", handler, &config_v) < 0) 
	{
        printf("Can't load 'test.ini'\n");
        return 1;
    }
    print_config(config_v);


	namespace po = boost::program_options;
	po::options_description desc("Allowed options");

	
	// on Unix POXIS based systems, turn off line buffering of input, so cin.get() returns after every keypress
// On other systems, you'll need to look for an equivalent
#ifdef POSIX
	termios stored_settings;
	tcgetattr(0, &stored_settings);
	termios new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= (~ISIG); // don't automatically handle control-C
	tcsetattr(0, TCSANOW, &new_settings);
#endif
	try
	{
		/*
		if (argc != 3)
		{
			cerr << "Usage: telnet <host> <port>\n";
			return 1;
		}*/
		boost::asio::io_service io_service;
		// resolve the host name and port number to an iterator that can be used to connect to the server
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1], TELNET_PORT);
		tcp::resolver::iterator iterator = resolver.resolve(query);




		// define an instance of the main class of this program
		telnet_client c(io_service, iterator);
		class ciscoctrl ciscoctrl_v(c);
		c._callback(ciscoctrl_v);

		// run the IO service as a separate thread, so the main thread can block on standard input
		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

		struct DetectAP test = {1,1,1,1,"1","12:23:23:23:45"};
		ciscoctrl_v.rogue_client.rogue_client_map["rogue_mac"] = test;
		ciscoctrl_v.rogue_client.rogue_client_map.insert(std::map<string, struct DetectAP> ::value_type("rogue_mac", test));

		PRINTMAP(rogue_client, ciscoctrl_v.rogue_client.rogue_client_map);

		while (1)
		{
			char ch;
			cin.get(ch); // blocking wait for standard input
			if (ch == 3) // ctrl-C to end program
				break;  
			ciscoctrl_v.login(CTRL_USER, CTRL_PASSWD);
			ciscoctrl_v.write(ch);
		//	c.write(ch);
		}
		c.close(); // close the telnet client connection
		t.join(); // wait for the IO service thread to close
	}
	catch (exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
	}
#ifdef POSIX // restore default buffering of standard input
	tcsetattr(0, TCSANOW, &stored_settings);
#endif
	return 0;
}


