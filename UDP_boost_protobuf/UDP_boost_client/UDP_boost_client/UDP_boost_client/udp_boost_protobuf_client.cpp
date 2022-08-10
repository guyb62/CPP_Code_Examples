#include <iostream>
#include <boost/array.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <google/protobuf/util/time_util.h>
#include "radio.pb.h"

//https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio/tutorial/tutdaytime4.html

using boost::asio::ip::udp;
using google::protobuf::util::TimeUtil;

// Iterates though all people in the AddressBook and prints info about them.
void ReadProtobufList(const madera::PhoneList& phoneList) {
	for (int i = 0; i < phoneList.phones_size(); i++) {
		const madera::Phone& phone = phoneList.phones(i);

		std::cout << "Phone ID: " << phone.id() << std::endl;
		std::cout << "Phone Name: " << phone.name() << std::endl;
		std::cout << "Phone Network: " << phone.network() << std::endl;
		std::cout << "Phone Active Status: " << phone.active() << std::endl;
		std::cout << "Phone Type: " << phone.type() << std::endl;
		std::cout << "Phone GetTypeName: " << phone.GetTypeName() << std::endl;
		auto test = phone.GetMetadata();

		if (phone.has_created()) {
			std::cout << " Created on: " << TimeUtil::ToString(phone.created()) << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	try
	{
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		if (argc != 2)
		{
			std::cerr << "Usage: client <host>" << std::endl;
			return 1;
		}
		std::cout << "/-------------------- UDP Client --------------------/" << std::endl;
		boost::asio::io_context io_context;

		udp::resolver resolver(io_context);
		udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), argv[1], "daytime").begin();

		udp::socket socket(io_context);
		socket.open(udp::v4());

		boost::array<char, 1> send_buf = { { 0 } };
		socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

		boost::array<char, 128> recv_buf;
		udp::endpoint sender_endpoint;
		std::cout << "Receive data from socket" << std::endl;
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

		madera::PhoneList phoneList;

		std::cout << "Parse protobuf data received from socket" << std::endl;
		phoneList.ParseFromArray(&recv_buf, 128);

		ReadProtobufList(phoneList);

		google::protobuf::ShutdownProtobufLibrary();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}