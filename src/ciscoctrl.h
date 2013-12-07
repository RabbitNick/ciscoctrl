
#ifndef CISCOCTRL_H
#define CISCOCTRL_H 


#include <iostream>
#include <string>

#include <deque>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

extern void printhello(void);



using namespace std;
using boost::asio::ip::tcp;

class telnet_client
{
public:
	enum { max_read_length = 512 };
	
	telnet_client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator);
	
	void write(const char msg); // pass the write data to the do_write function via the io service in the other thread

	
	void close() ;// call the do_close function via the io service in the other thread

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