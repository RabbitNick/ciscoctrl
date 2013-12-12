
#ifndef CISCOCTRL_H
#define CISCOCTRL_H 


#include <iostream>
#include <string>
#include <map>


#include <deque>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>



#include "../config.h"


extern void printhello(void);



using namespace std;
using boost::asio::ip::tcp;


#define CISCO4400_USER "User"
#define CISCO4400_PASSWD "Password"
#define CISCO4400_CTRL "Cisco Controller"






#define PRINTMAP(mapPrefix, mapName) \
{\
	std::map<string, struct DetectAP>::iterator mapPrefix##iter = mapName.begin(); \
	for(; mapPrefix##iter != mapName.end(); mapPrefix##iter++) \
	{\
		std::cout << mapPrefix##iter->first << std::endl; \
	    std::cout <<  mapPrefix##iter->second.mac << std::endl;\
   	    std::cout <<  mapPrefix##iter->second.name << std::endl;\
	    std::cout <<  mapPrefix##iter->second.rssi << std::endl;\
	    std::cout <<  mapPrefix##iter->second.snr << std::endl;\
	    std::cout <<  mapPrefix##iter->second.report_time << std::endl;\
	    std::cout <<  mapPrefix##iter->second.channel << std::endl;\
   	    std::cout <<  "---------------------------------" << std::endl;\
	}\
}





struct DetectAP
{
	/* data */
	int rssi;
	int channel;
	int report_time;
	int snr;
	string name;
	string mac;
};

struct rogue_client_property
{
	/* data */
	string mac;
	int detect_AP_Num;
	struct DetectAP *detect_AP_point;
	std::map<string, struct DetectAP> rogue_client_map;
};


struct rogue_client_record 
{

};

class ciscoctrl
{
public:
	ciscoctrl();
	ciscoctrl(class telnet_client &v)
	{
		telnet_client_ptr = &v;
	}
	~ciscoctrl()
	{
		;
	}
     
    int login(const char *user, const char *passwd);
    int logout(void);
    int show_rogue_client_summary(void);
    int show_rogue_client_detail(const char &mac);
    int record_rogue_client(struct rogue_client_record &v);
    int delete_rogue_client(struct rogue_client_record &v);

    int write(const char msg);

    /* data */
    struct telnet_wr
    {
    	/* data */
    	int len;
    	char *read_msgs;
    	string write_msgs;
    };
    struct telnet_wr telnet_buf;
    struct rogue_client_property rogue_client;

    class telnet_client *telnet_client_ptr;
private:
	int telnet_write(struct telnet_wr &v);
};


class telnet_client
{
public:
	enum { max_read_length = 512 };
	
	telnet_client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator);
	~telnet_client(void);
	
	void write(const char msg); // pass the write data to the do_write function via the io service in the other thread

	
	void close() ;// call the do_close function via the io service in the other thread

	int _callback(class ciscoctrl &v);

	class ciscoctrl *ciscoctrl_ptr;

private:

	void connect_start(tcp::resolver::iterator endpoint_iterator);

	
	void connect_complete(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator);


	void read_start(void);

	
	void read_complete(const boost::system::error_code& error, size_t bytes_transferred);

	void do_write(const char msg);

	
	void write_start(void);

	
	void write_complete(const boost::system::error_code& error);

	
	void do_close();


private:
	boost::asio::io_service& io_service_; // the main IO service that runs this connection
	tcp::socket socket_; // the socket this instance is connected to
	char read_msg_[max_read_length]; // data read from the socket
	deque<char> write_msgs_; // buffered write data
};



#endif