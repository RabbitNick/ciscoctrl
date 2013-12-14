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
	timeout = 0;
	if(ctrl_state.ID != "login")
	{

		return -1;
	}

	if(ctrl_state.flag == ctrl_state.s1)
	{
		if (strstr(this->telnet_buf.read_msgs, "(Cisco Controller)"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);	
		}
		ctrl_state.flag = ctrl_state.s2;
		timeout++;
	}


	if(ctrl_state.flag == ctrl_state.s2)
	{
		if (strstr(this->telnet_buf.read_msgs,CISCO4400_USER))
		{ 
			telnet_buf.write_msgs = user;   
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s3;
		}
		else
		{
			timeout++;
		}
	}

	if(ctrl_state.flag == ctrl_state.s3)
	{
		if (strstr(this->telnet_buf.read_msgs, CISCO4400_PASSWD))
		{    
	    	telnet_buf.write_msgs = passwd;
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s4;
		}
		else
		{
			timeout++;
		}
	}

	if(timeout >= RUN_TIMEOUT)
	{
		ctrl_state.flag = ctrl_state.s4;
	}

	if(ctrl_state.flag == ctrl_state.s4)
	{
		ctrl_state.flag = ctrl_state.s0;
		ctrl_state.ID = "";
		timeout = 0;
	}


	return 0;
}

int ciscoctrl::logout(void)
{
	timeout = 0;
	if(ctrl_state.ID != "logout")
	{
		return -1;
	}

	if (strstr(this->telnet_buf.read_msgs, CISCO4400_CTRL))
	{    
	    telnet_buf.write_msgs = CTRL_LOGOUT;
		telnet_write(telnet_buf);
		exit(1);
	}

	return 0;
}

int ciscoctrl::show_rogue_client_summary(void)
{

	timeout = 0;
	if(ctrl_state.ID != "srcs")
	{
		return -1;
	}

	if(ctrl_state.flag == ctrl_state.s1)
	{
		if (strstr(this->telnet_buf.read_msgs, CISCO4400_CTRL))
		{
			telnet_buf.write_msgs = CTRL_SHOW_ROGUE_CLIENT_SUMMARY;
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s2;
		}
		else
		{
			timeout++;
		}	
	}


	if(ctrl_state.flag == ctrl_state.s2)
	{
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s3;
		}
		else
		{
			timeout++;
		}
	}

	if(ctrl_state.flag == ctrl_state.s3)
	{
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
		}
		else
		{
			ctrl_state.flag = ctrl_state.s4;
		}		
	}	


	if(timeout >= RUN_TIMEOUT)
	{
		ctrl_state.flag = ctrl_state.s4;
	}

	if(ctrl_state.flag == ctrl_state.s4)
	{
		ctrl_state.flag = ctrl_state.s0;
		ctrl_state.ID = "";
		timeout = 0;
	}
}

int ciscoctrl::show_rogue_client_detail(const char *_mac)
{
	timeout = 0;
	string s = CTRL_SHOW_ROGUE_CLIENT_DETAILED;

	if(ctrl_state.ID != "srcd")
	{
		return -1;
	}

	if(ctrl_state.flag == ctrl_state.s1)
	{
		if (strstr(this->telnet_buf.read_msgs, CISCO4400_CTRL))
		{
			s.insert(27, _mac);
			telnet_buf.write_msgs = s;
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s2;
		}
		else
		{
			timeout++;
		}	
	}

	if(ctrl_state.flag == ctrl_state.s2)
	{
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s3;
		}
		else
		{
			timeout++;
		}
	}

	if(ctrl_state.flag == ctrl_state.s3)
	{
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
		}
		else
		{
			ctrl_state.flag = ctrl_state.s4;
		}		
	}	


	if(timeout >= RUN_TIMEOUT)
	{
		ctrl_state.flag = ctrl_state.s4;
	}

	if(ctrl_state.flag == ctrl_state.s4)
	{
		ctrl_state.flag = ctrl_state.s0;
		ctrl_state.ID = "";
		timeout = 0;
	}	


	return 0;
}

int ciscoctrl::record_rogue_client(struct rogue_client_record &v)
{


}

int ciscoctrl::record_rogue_client(void)
{
	timeout = 0;
	string s = CTRL_SHOW_ROGUE_CLIENT_DETAILED;

	if(ctrl_state.ID != "rrc")
	{
		return -1;
	}

	if(ctrl_state.flag == ctrl_state.s1)
	{
		if (strstr(this->telnet_buf.read_msgs, CISCO4400_CTRL))
		{
			telnet_buf.write_msgs = CTRL_SHOW_ROGUE_CLIENT_SUMMARY;
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s2;
		}
		else
		{
			timeout++;
		}	
	}

	if(ctrl_state.flag == ctrl_state.s2)
	{

			// ctrlfile.open("mac.txt");
			// ctrlfile << this->telnet_buf.read_msgs << "\n";
			// ctrlfile.close();

		//std::cout<< endl<< "adfasdf " << telnet_buf.read_msgs << "sdfasdfg" << endl;
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s3;
		}
		else
		{
			timeout++;
		}
	}

	if(ctrl_state.flag == ctrl_state.s3)
	{
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
		}
		else
		{
			ctrl_state.flag = ctrl_state.s4;
		}		
	}	


	if(timeout >= RUN_TIMEOUT)
	{
		ctrl_state.flag = ctrl_state.s4;
	}

	if(ctrl_state.flag == ctrl_state.s4)
	{
		ctrl_state.flag = ctrl_state.s0;
		ctrl_state.ID = "";
		timeout = 0;
	}	




}



int ciscoctrl::delete_rogue_client(struct rogue_client_record &v)
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
		memset(read_msg_, 0, sizeof(read_msg_));
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
	ciscoctrl_ptr->telnet_buf.read_msgs = new char[sizeof(read_msg_)];
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
	//	char c;
	//	cin.get(c);
 		// char *p = strstr(read_msg_,"User");
 		// std::cout << std::endl;
 		// cout.write(p, strlen("User"));
			// while(1);

			ciscoctrl_ptr->ctrlfile.open("mac.txt" , ios::app);
			ciscoctrl_ptr->ctrlfile.write(read_msg_, bytes_transferred);
			ciscoctrl_ptr->ctrlfile.close();

	//	FILE *ifp;
	//	ifp = fopen("mac.txt", "a");
	//	fprintf(stderr, read_msg_ );
	//	fclose(ifp);

	//	ciscoctrl_ptr->telnet_buf.read_msgs = new char[sizeof(read_msg_)];
		memmove(ciscoctrl_ptr->telnet_buf.read_msgs, read_msg_,  sizeof(read_msg_));



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