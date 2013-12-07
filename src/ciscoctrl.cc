#include "ciscoctrl.h"

using namespace std;


// just for testing
void printhello(void)
{
	std::cout << "Hello, this is fudan's voice." << std::endl;
}


// telnet_client class function

// public function 
telnet_client::telnet_client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
		: io_service_(io_service), socket_(io_service)
	{
		connect_start(endpoint_iterator);
	}
	
void telnet_client::write(const char msg) // pass the write data to the do_write function via the io service in the other thread
{
	io_service_.post(boost::bind(&telnet_client::do_write, this, msg));
}
	
void telnet_client::close() // call the do_close function via the io service in the other thread
{
	io_service_.post(boost::bind(&telnet_client::do_close, this));
}





// private function
void telnet_client::connect_start(tcp::resolver::iterator endpoint_iterator)
{ // asynchronously connect a socket to the specified remote endpoint and call connect_complete when it completes or fails
	tcp::endpoint endpoint = *endpoint_iterator;
	socket_.async_connect(endpoint,
		boost::bind(&telnet_client::connect_complete,
		this,
		boost::asio::placeholders::error,
		++endpoint_iterator));
}


void telnet_client::connect_complete(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
{ // the connection to the server has now completed or failed and returned an error
	if (!error) // success, so start waiting for read data
		read_start();
	else if (endpoint_iterator != tcp::resolver::iterator())
	{ // failed, so wait for another connection event
		socket_.close();
		connect_start(endpoint_iterator);
	}
}

void telnet_client::read_start(void)
{ // Start an asynchronous read and call read_complete when it completes or fails
	socket_.async_read_some(boost::asio::buffer(read_msg_, max_read_length),
		boost::bind(&telnet_client::read_complete,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}
	
void telnet_client::read_complete(const boost::system::error_code& error, size_t bytes_transferred)
{ // the asynchronous read operation has now completed or failed and returned an error
	if (!error)
	{ // read completed, so process the data
		cout.write(read_msg_, bytes_transferred); // echo to standard output
		//cout << "\n";
		read_start(); // start waiting for another asynchronous read again
	}
	else
		do_close();
}
	
void telnet_client::do_write(const char msg)
{ // callback to handle write call from outside this class
	bool write_in_progress = !write_msgs_.empty(); // is there anything currently being written?
	write_msgs_.push_back(msg); // store in write buffer
	if (!write_in_progress) // if nothing is currently being written, then start
		write_start();
}
	
void telnet_client::write_start(void)
{ // Start an asynchronous write and call write_complete when it completes or fails
	boost::asio::async_write(socket_,
		boost::asio::buffer(&write_msgs_.front(), 1),
		boost::bind(&telnet_client::write_complete,
			this,
			boost::asio::placeholders::error));
}
	
void telnet_client::write_complete(const boost::system::error_code& error)
{ // the asynchronous read operation has now completed or failed and returned an error
	if (!error)
	{ // write completed, so send next write data
		write_msgs_.pop_front(); // remove the completed data
		if (!write_msgs_.empty()) // if there is anthing left to be written
			write_start(); // then start sending the next item in the buffer
	}
	else
		do_close();
}
	
void telnet_client::do_close()
{
	socket_.close();
}