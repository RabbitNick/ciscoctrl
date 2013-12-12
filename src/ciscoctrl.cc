#include "ciscoctrl.h"

using namespace std;


// just for testing
void printhello(void)
{
	std::cout << "Hello, this is fudan's voice." << std::endl;
}





int ciscoctrl::write(const char msg)
{
	telnet_client_ptr->write(msg);
}

int ciscoctrl::login(const char *user, const char *passwd)
{


	if (strstr(this->telnet_buf.read_msgs,CISCO4400_USER))
	{ 
		telnet_buf.write_msgs = user;   
		telnet_write(telnet_buf);
	}

	if (strstr(this->telnet_buf.read_msgs, CISCO4400_PASSWD))
	{    
	    telnet_buf.write_msgs = passwd;
		telnet_write(telnet_buf);
	}
	return 0;
}

int ciscoctrl::logout(void)
{

}

int ciscoctrl::show_rogue_client_summary(void)
{

}

int ciscoctrl::show_rogue_client_detail(const char &mac)
{

}

int record_rogue_client(struct rogue_client_record &v)
{

}

int delete_rogue_client(struct rogue_client_record &v)
{

}


// private class function
int ciscoctrl::telnet_write(struct telnet_wr &v)
{
	for(int i = 0; i < v.write_msgs.length(); i++)
	{
		telnet_client_ptr->write(*(v.write_msgs.c_str() + i));		
	}
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

int telnet_client::_callback(class ciscoctrl &v)
{
	ciscoctrl_ptr = &v;
}

telnet_client::~telnet_client(void)
{
	delete ciscoctrl_ptr->telnet_buf.read_msgs;
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
	

 		// char *p = strstr(read_msg_,"User");
 		// std::cout << std::endl;
 		// cout.write(p, strlen("User"));
			// while(1);
		ciscoctrl_ptr->telnet_buf.read_msgs = new char[sizeof(read_msg_)];
		memmove(ciscoctrl_ptr->telnet_buf.read_msgs, read_msg_,  sizeof(read_msg_));

		// if (strstr(read_msg_,"User"))
		// {    
		// 	char tab[] = "admin\r";
		// 	for(int i = 0; i < strlen(tab); i++)
		// 	{
		// 		write(tab[i]);
		// 	}
		// }

		// if (strstr(read_msg_,"Password"))
		// {    
		// 	char tab[] = "sonic\r";
		// 	for(int i = 0; i < strlen(tab); i++)
		// 	{
		// 		write(tab[i]);
		// 	}
		// }

		// if (strstr(read_msg_,"Cisco Controller"))
		// {    
		// 	char tab[] = "show rogue client summary\r";
		// 	for(int i = 0; i < strlen(tab); i++)
		// 	{
		// 		write(tab[i]);
		// 	}
		// }

		memset(read_msg_, 0, sizeof(read_msg_));
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