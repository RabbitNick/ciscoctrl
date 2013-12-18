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

//	memset(telnet_buf.read_msgs, 0, telnet_client_ptr->max_read_length * sizeof(char));
}

int ciscoctrl::show_rogue_client_detail(const char *_mac)
{

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




int ciscoctrl::record_rogue_client_summary(void)
{

	if(can_read == 0)
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
//		cout << "s1" << endl;
	}



	if(ctrl_state.flag == ctrl_state.s2)
	{


		static int t = 0;
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s3;
			t = 0;
//		cout << "s2:1" << endl;

		}
		else
		{

			if(t <= 10)
				{
			timeout++;
			t++;
			ctrl_state.flag = ctrl_state.s2;
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);	
				}
				else
				{
					ctrl_state.flag = ctrl_state.s3;
				}
	//			cout << "s2:2" << endl;
		}

	}

	if(ctrl_state.flag == ctrl_state.s3)
	{
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
//			cout << "s3:1" << endl;
		}
		else
		{


			
			if(strstr(this->telnet_buf.read_msgs, ":"))
			{
				ctrl_state.flag = ctrl_state.s2;
	//			cout << "s3:2" << endl;
			}
	
			if (strstr(this->telnet_buf.read_msgs, CISCO4400_CTRL))
			{
//											cout << "s3:4" << endl;
							ctrl_state.flag = ctrl_state.s4;
			ctrl_state.record_mac_start = 1;

			}

		}		
		//			cout << "s3" << endl;
	}	


	if(ctrl_state.flag == ctrl_state.s4)
	{
		ctrl_state.flag = ctrl_state.s0;
	//	ctrl_state.ID = "";
		timeout = 0;
	}

	can_read = 0;
}

int ciscoctrl::record_rogue_client_detail(const char *_mac)
{
	//timeout = 0;
	string s = CTRL_SHOW_ROGUE_CLIENT_DETAILED;

	if(ctrl_state.flag == ctrl_state.s1)
	{
		if (strstr(this->telnet_buf.read_msgs, CISCO4400_CTRL))
		{
			s.insert(27, _mac);
			telnet_buf.write_msgs = s;
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s2;
			std::cout << "s1   "<< endl;
		}
		else
		{
			timeout++;
						std::cout << "s1   timeout"<< endl;

		}	
	//	while(1);
	}

		if(strstr(telnet_buf.read_msgs, "show rogue client"))
			exit(1);

	if(ctrl_state.flag == ctrl_state.s2)
	{

		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
			ctrl_state.flag = ctrl_state.s3;
						std::cout << "s2 :2  "<< endl;
		}
		else
		{
			timeout++;
			std::cout << "s2 :3 timeout "<< timeout << endl;

		}

		
		memset(this->telnet_buf.read_msgs, 0, 512*4);
	}

	if(ctrl_state.flag == ctrl_state.s3)
	{
		if (strstr(this->telnet_buf.read_msgs, "--More-- or (q)uit"))
		{
			telnet_buf.write_msgs = "\r";
			telnet_write(telnet_buf);
			std::cout << "s3  :1 "<< endl;

		}
		else
		{

			exit(1);
			ctrl_state.flag = ctrl_state.s4;
		//				std::cout << "s3  timeout "<< endl;

		}		
	}	


	if(timeout >= RUN_TIMEOUT)
	{
		ctrl_state.flag = ctrl_state.s4;
//		std::cout << "timeout   " << timeout<< endl;

	}

	if(ctrl_state.flag == ctrl_state.s4)
	{
		ctrl_state.flag = ctrl_state.s0;
		ctrl_state.record_mac_detail_start = 1;

	//	ctrl_state.ID = "";
		timeout = 0;
	}	
	return 0;
}

int ciscoctrl::record_rogue_client(struct rogue_client_record &v)
{


}



int ciscoctrl::record_rogue_client(void)
{
	if(ctrl_state.ID != "rrc")
	{
		return -1;
	}

	if(ctrl_state.record_mac_start == 0)
	{
		while(1)
		{
			record_rogue_client_summary();

			if(ctrl_state.flag == ctrl_state.s0)
				break;
		}
	}

	if(ctrl_state.record_mac_start == 1)
	{

		handle_rogue_client();

		ctrl_state.record_mac_start = 2;
		ctrl_state.flag = ctrl_state.s1;
	}


	if(ctrl_state.record_mac_start == 2)
	{
		static int i = 0;
		while(1)
		{
		record_rogue_client_detail(rogue_client.client_mac[i].c_str());

		if(ctrl_state.flag == ctrl_state.s0)
		{
			exit(1);
			i++;
			if(i >= rogue_client.client_mac.size())
			{
				ctrl_state.ID.clear();
				i = 0;
				break;
			}
			ctrl_state.flag = 1;
			std::cout<< rogue_client.client_mac.size()<<  "   " << i << endl;
		}
		}
	//	exit(1);

		//record_buffer_ptr->clear();
	}

	if(ctrl_state.record_mac_detail_start == 1)
	{
		;
	}

}


int ciscoctrl::handle_rogue_client_record(void)
{
	if(telnet_buf.read_msgs == 0)
	{
		return -1;
	}
	record_buffer_ptr->append(telnet_buf.read_msgs);

}

int ciscoctrl::handle_rogue_client(void)
{

	if(telnet_buf.read_msgs == 0)
	{
		return -1;
	}

	int r = 0;
	regex_mac = MAC_REGEX;
	string::const_iterator start = record_buffer_ptr->begin();
	string::const_iterator end = record_buffer_ptr->end();

	while(r = boost::regex_search(start, end, regex_what, regex_mac))
	{
		rogue_client.client_mac.push_back(regex_what.str());

		rogue_client.rogue_client_map[rogue_client.client_mac.back()] = new (struct each_rogue_client_property); // add client mac to a map

		start = regex_what[0].second;	//the end of the sub matched string	
	}

/*
		 	ctrlfile.open("mac.txt" , ios::app);
		 ctrlfile << *record_buffer_ptr;
		 ctrlfile.close();

	 static int i = 0;
	 ctrlfile.open("mac1.txt" , ios::app);
   for(; i<rogue_client.client_mac.size(); i++)
   {
     ctrlfile << rogue_client.client_mac[i];
     ctrlfile << '\n';
   }
  		 ctrlfile.close();*/
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




//----------------------------------------------------------------------------
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
	ciscoctrl_ptr->record_buffer_ptr =  new string;
	ciscoctrl_ptr->can_read = 0;
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

		// ciscoctrl_ptr->ctrlfile.open("mac.txt" , ios::app);
		// ciscoctrl_ptr->ctrlfile.write(read_msg_, bytes_transferred);
		// ciscoctrl_ptr->ctrlfile << std::endl;
		// ciscoctrl_ptr->ctrlfile.close();

		mtx_.lock();
		memcpy(ciscoctrl_ptr->telnet_buf.read_msgs, read_msg_,  sizeof(read_msg_));
		ciscoctrl_ptr->can_read = 1;
		mtx_.unlock();


		if(ciscoctrl_ptr->ctrl_state.ID == "rrc")
		{
			mtx_.lock();
			ciscoctrl_ptr->	handle_rogue_client_record();// this function have to place here because we need to copy the read_msg_(privte value) to ciscoctrl class.
			mtx_.unlock();

		}


		memset(read_msg_, 0, sizeof(read_msg_));
		//cout << "\n";
		read_start(); // start waiting for another asynchronous read again
	}
	else
	{
		do_close();
	}
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