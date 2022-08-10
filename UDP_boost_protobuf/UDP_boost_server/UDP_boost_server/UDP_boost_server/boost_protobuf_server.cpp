#include <iostream>
#include <boost/array.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio.hpp>
#include <google/protobuf/util/time_util.h>
#include <string>

#include "radio.pb.h"


//https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio/tutorial/tutdaytime5.html

using boost::asio::ip::udp;

using google::protobuf::util::TimeUtil;

// This function fills in a Person message based on user input.
void AddPhoneData(madera::Phone* phone, int id, std::string name, madera::Phone::PhoneNetwork phoneNet,
	bool activeStatus, madera::Phone::PhoneType phoneType) {

	// int id = 1111;
	phone->set_id(id);
	std::cout << "Set phone ID: " << phone->id() << std::endl;

	phone->set_name(name); // it will be initialized to an empty string automatically if not set
	std::cout << "Set phone name: " << phone->name() << std::endl;

	phone->set_network(phoneNet);
	std::cout << "Set phone network: " << phone->network() << std::endl;

	phone->set_active(activeStatus);
	std::cout << "Set phone active status: " << phone->active() << std::endl;

	phone->set_type(phoneType);
	std::cout << "Set phone type: " << phone->type() << std::endl;

	*phone->mutable_created() = TimeUtil::SecondsToTimestamp(time(NULL));
}

int main(int argc, char* argv[])
{
	try
	{
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		boost::asio::io_context io_context;

		udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

		for (;;)
		{
			boost::array<char, 1> recv_buf;
			udp::endpoint remote_endpoint;
			socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

			// Protobuf part..........
			/*........................................................*/
			madera::PhoneList phoneList;

			// Add an address.
			std::cout << "Adding phone data" << std::endl;
			AddPhoneData(phoneList.add_phones(), 0001, "Fresno", madera::Phone::EE, true, madera::Phone::ANDROID);
			AddPhoneData(phoneList.add_phones(), 0002, "Fresno2", madera::Phone::VODAPHONE, true, madera::Phone::APPLE);

			boost::asio::streambuf stream_buffer;
			std::ostream output_stream(&stream_buffer);
			
			// Write the new address book back to disk.
			std::cout << "Serialize phone data To Ostream" << std::endl;
			if (!phoneList.SerializeToOstream(&output_stream)) {
				std::cerr << "Failed to write phone message." << std::endl;
				return -1;
			}
   			/*........................................................*/
			std::streamsize size = output_stream.tellp();
			std::cout << "Size of stream: " << std::to_string(size) << std::endl;
			boost::system::error_code ignored_error;
			std::cout << "Send data to socket" << std::endl;
			socket.send_to(stream_buffer.data(), remote_endpoint, 0, ignored_error);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}