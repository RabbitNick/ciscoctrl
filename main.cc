
// this work is for passive localization using cisco 4400 controller

#include "config.h"





int main(int argc, char* argv[])
{


 
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
		if (argc != 3)
		{
			cerr << "Usage: telnet <host> <port>\n";
			return 1;
		}
		boost::asio::io_service io_service;
		// resolve the host name and port number to an iterator that can be used to connect to the server
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1], TELNET_PORT);
		tcp::resolver::iterator iterator = resolver.resolve(query);
		// define an instance of the main class of this program
		telnet_client c(io_service, iterator);
		// run the IO service as a separate thread, so the main thread can block on standard input
		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
		while (1)
		{
			char ch;
			cin.get(ch); // blocking wait for standard input
			if (ch == 3) // ctrl-C to end program
				break;
			c.write(ch);
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


