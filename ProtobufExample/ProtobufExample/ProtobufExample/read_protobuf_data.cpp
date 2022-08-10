// See README.txt for information and build instructions.

#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>

#include "radio.pb.h"

using namespace std;

using google::protobuf::util::TimeUtil;

// Iterates though all people in the AddressBook and prints info about them.
void ReadProtobufList(const madera::PhoneList& phoneList) {
	for (int i = 0; i < phoneList.phones_size(); i++) {
		const madera::Phone& phone = phoneList.phones(i);

		cout << "Phone ID: " << phone.id() << endl;
		cout << "Phone Name: " << phone.name() << endl;
		cout << "Phone Network: " << phone.network() << endl;
		cout << "Phone Active Status: " << phone.active() << endl;
		cout << "Phone Type: " << phone.type() << endl;
		cout << "Phone GetMetadata: " << *phone.GetMetadata().descriptor() << endl;
		cout << "Phone GetTypeName: " << phone.GetTypeName() << endl;
		
		if (phone.has_created()) {
			std::cout << " Created on: " << TimeUtil::ToString(phone.created()) << std::endl;
		}
	}
}

// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if (argc != 2) {
		cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
		return -1;
	}

	madera::Phone phone;
	madera::PhoneList phoneList;

	{
		// Read the existing address book.
		fstream input(argv[1], ios::in | ios::binary);
		if (!phoneList.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return -1;
		}
	}

	ReadProtobufList(phoneList);

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}