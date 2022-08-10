#include <iostream>
#include <boost/array.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
//https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio/tutorial/tutdaytime5.html

using boost::asio::ip::udp;

int main()
{
	try
	{
		boost::asio::io_context io_context;

		udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

		for (;;)
		{
			boost::array<char, 1> recv_buf;
			udp::endpoint remote_endpoint;
			socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

			std::string message = "Hello World"; // make_daytime_string();

			boost::system::error_code ignored_error;
			socket.send_to(boost::asio::buffer(message),
				remote_endpoint, 0, ignored_error);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}