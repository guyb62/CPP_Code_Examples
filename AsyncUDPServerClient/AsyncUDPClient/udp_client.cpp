#include <cstdlib>
#include <iostream>
#include <deque>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
// #include "asio.hpp"
//#include "messages.hpp"
#include <string>
#include <boost/asio/error.hpp>
#include <boost/system.hpp>


std::string show_hex(const char* data, size_t size)
{
	std::string hex;
	char buf[16];
	const char* p = data;
	for (size_t i = 0; i < size; ++i) {
		std::sprintf(buf, "%02X ", static_cast<unsigned>(p[i]) & 0xFF);
		hex += buf;
	}
	return hex;
}
std::string show_str(const char* data, size_t size)
{
	std::string str;
	char buf[16];
	const char* p = data;
	for (size_t i = 0; i < size; ++i) {
		std::sprintf(buf, "%c", p[i]);
		str += buf;
	}
	return str;
}

class message
{
public:
	enum { header_length = 22 };
	enum { max_body_length = 1024 };

	message()
		: body_length_(0)
	{
	}

	const char* data() const
	{
		return data_;
	}

	char* data()
	{
		return data_;
	}
	size_t max_length()
	{
		return header_length + max_body_length;
	}
	size_t length() const
	{
		return header_length + body_length_;
	}

	const char* body() const
	{
		return data_ + header_length;
	}

	char* body()
	{
		return data_ + header_length;
	}

	size_t body_length() const
	{
		return body_length_;
	}

	void body_length(size_t length)
	{
		body_length_ = length;
		if (body_length_ > max_body_length)
			body_length_ = max_body_length;
	}

	bool decode_header()// ? 4???????body??
	{
		using namespace std; // For strncat and atoi.
		char header[header_length + 1] = "";
		strncat(header, data_, header_length);
		body_length_ = atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		using namespace std; // For sprintf and memcpy.
		char header[header_length + 1] = "";
		sprintf(header, "%4d", body_length_);
		memcpy(data_, header, header_length);
	}

private:
	char data_[header_length + max_body_length];
	size_t body_length_;
};

using boost::asio::ip::udp;

class udp_client {
public:
	udp_client(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
		:io_service_(io_service), socket_(io_service, udp::endpoint(udp::v4(), 0)) {
		udp::resolver resolver(io_service_);
		udp::resolver::query query(udp::v4(), host, port);
		udp::resolver::iterator itr = resolver.resolve(query);

		sender_endpoint_ = *itr;
	}

	~udp_client() {
		socket_.close();
	}
	void send_message(const message& msg) {
		io_service_.post(boost::bind(&udp_client::do_write, this, msg));
	}

	void do_write(const message& msg) {
		send_msg_queue_.push_back(msg);

		if (!send_msg_queue_.empty()) {
			socket_.async_send_to(
				//boost::asio::buffer(send_msg_queue_.front().data(), send_msg_queue_.front().length()), 
				boost::asio::buffer(msg.data(), msg.length()), 
				sender_endpoint_,
				boost::bind(&udp_client::handle_send_to, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

			std::cout << "send data(str):" << show_str(send_msg_queue_.front().data(), send_msg_queue_.front().length()) << std::endl;
		}
	}



	void handle_send_to(const boost::system::error_code& error, size_t s/*bytes_sent*/) {
		std::cout << "bytes_sent = " << s << "." << std::endl;

		if (!error) {
			// send success, remove from the queue
			send_msg_queue_.pop_front();

			// recv response after send a message
			recv_message();

			if (!send_msg_queue_.empty()) {
				socket_.async_send_to(
					boost::asio::buffer(send_msg_queue_.front().data(), send_msg_queue_.front().length()),
					sender_endpoint_,
					boost::bind(&udp_client::handle_send_to, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}
		else {
			std::cerr << "error in handle_send_to:" << error << std::endl;
			socket_.close();
		}

	}

	void recv_message() {
		socket_.async_receive_from(
			boost::asio::buffer(data_.data(), data_.max_length()), sender_endpoint_,
			boost::bind(&udp_client::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
	{
		if (!error) {
			std::cout << "handle_receive_from " << std::endl;
			std::cout << "recv data(str):" << show_str(data_.data(), bytes_recvd) << std::endl;
			if (bytes_recvd > message::header_length)
				data_.body_length(bytes_recvd - message::header_length);
			//std::cout<<"message.lenght()="<<data_.length()<<"bytes_recvd="<<bytes_recvd<<std::endl;
		}
		else {
			std::cerr << "error in handle_receive_from:" << error << std::endl;
			socket_.close();
		}
	}
private:
	//enum {max_length = 1024};
	//char data_[max_length];
	message data_;
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint sender_endpoint_;
	std::deque<message> send_msg_queue_;
};

int main(int argc, char* argv[]) {

	boost::asio::io_service is;

	udp_client c1(is, "localhost", "8886");
	udp_client c2(is, "localhost", "8887");
	udp_client c3(is, "localhost", "8888");
	std::string msg1 = "This is Request seq1 from c1.";
	std::string msg2 = "This is Request seq1 from c2.";
	std::string msg3 = "This is Request seq1 from c3.";

	message msg;
	msg.body_length(msg1.length());
	memcpy(msg.data(), "1234567890123456789021", message::header_length);
	memcpy(msg.body(), "This is Request seq1 from c1.", msg.body_length());

	c1.send_message(msg);

	msg.body_length(msg2.length());
	memcpy(msg.data(), "1234567890123456789022", message::header_length);
	memcpy(msg.body(), "This is Request seq1 from c2.", msg.body_length());
	c2.send_message(msg);

	msg.body_length(msg3.length());
	memcpy(msg.data(), "1234567890123456789023", message::header_length);
	memcpy(msg.body(), "This is Request seq1 from c3.", msg.body_length());
	c3.send_message(msg);

	is.run();

	return 0;
}