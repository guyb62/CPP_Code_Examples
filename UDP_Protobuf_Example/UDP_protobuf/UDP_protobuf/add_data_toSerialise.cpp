// See README.txt for information and build instructions.

#include <ctime>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>

#include "radio.pb.h"

using namespace std;

using google::protobuf::util::TimeUtil;

// This function fills in a Person message based on user input.
void AddPhoneData(madera::Phone* phone, int id, string name, madera::Phone::PhoneNetwork phoneNet,
	bool activeStatus, madera::Phone::PhoneType phoneType) {
	
	// int id = 1111;
	phone->set_id(id);
	std::cout << "Set phone ID: " << phone->id() << endl;

	phone->set_name(name); // it will be initialized to an empty string automatically if not set
	std::cout << "Set phone name: " << phone->name() << endl;

	phone->set_network(phoneNet);
	std::cout << "Set phone network: " << phone->network() << endl;

	phone->set_active(activeStatus);
	std::cout << "Set phone active status: " << phone->active() << endl;

	phone->set_type(phoneType);
	std::cout << "Set phone type: " << phone->type() << endl;

	// Create the google:protobuf::Timestamp from date string
	//struct tm created;
	//strptime(doc["created"].GetString(), "%Y-%m-%dT%H:%M:%SZ", &created);

	//google::protobuf::Timestamp *created_timestamp = new google::protobuf::Timestamp();
	//created_timestamp->set_seconds(mktime(&created));
	//created_timestamp->set_nanos(0);
	//*phone->set_allocated_created();

	*phone->mutable_created() = TimeUtil::SecondsToTimestamp(time(NULL));
}

madera::Phone CreatePhone(int id, string name, madera::Phone::PhoneNetwork phoneNet,
	bool activeStatus, madera::Phone::PhoneType phoneType) {

	madera::Phone *phone = new madera::Phone;

	// int id = 1111;
	phone->set_id(id);
	std::cout << "Set phone ID: " << phone->id() << endl;

	phone->set_name(name); // it will be initialized to an empty string automatically if not set
	std::cout << "Set phone name: " << phone->name() << endl;

	phone->set_network(phoneNet);
	std::cout << "Set phone network: " << phone->network() << endl;

	phone->set_active(activeStatus);
	std::cout << "Set phone active status: " << phone->active() << endl;

	phone->set_type(phoneType);
	std::cout << "Set phone type: " << phone->type() << endl;

	// Create the google:protobuf::Timestamp from date string
	//struct tm created;
	//strptime(doc["created"].GetString(), "%Y-%m-%dT%H:%M:%SZ", &created);

	//google::protobuf::Timestamp *created_timestamp = new google::protobuf::Timestamp();
	//created_timestamp->set_seconds(mktime(&created));
	//created_timestamp->set_nanos(0);
	//*phone->set_allocated_created();

	*phone->mutable_created() = TimeUtil::SecondsToTimestamp(time(NULL));

	return *phone;
}

bool AddToPhoneList(madera::PhoneList* phoneList, std::vector<madera::Phone> listOfPhones, string fName)
{
	for (auto phone : listOfPhones)
	{
		// Add an address.
		AddPhoneData(phoneList->add_phones(), phone.id(), phone.name(),
			phone.network(), phone.active(), phone.type());

		// Write the new address book back to disk.
		// phone.SerializeToArray 
		fstream output(fName, ios::out | ios::trunc | ios::binary);
		if (!phoneList->SerializeToOstream(&output)) {
			cerr << "Failed to write phone message." << endl;
			return false;
		}

	}
	
	return true;

}

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if (argc != 2) {
		cerr << "Usage:  " << argv[0] << " PHONE_BINARY_DB_FILE" << endl;
		return -1;
	}

	madera::Phone phone1;
	madera::Phone phone2;
	madera::PhoneList phoneList;
   
	// phone1 = CreatePhone(0001, "Fresno", madera::Phone::EE, true, madera::Phone::ANDROID);
	// phone2 = CreatePhone(0002, "Fresno2", madera::Phone::VODAPHONE, true, madera::Phone::APPLE);
	
	// Add an address.
	AddPhoneData(phoneList.add_phones(), 0001, "Fresno", madera::Phone::EE, true, madera::Phone::ANDROID);
	AddPhoneData(phoneList.add_phones(), 0002, "Fresno2", madera::Phone::VODAPHONE, true, madera::Phone::APPLE);
	{
		// Write the new address book back to disk.
		// phone.SerializeToArray 
		fstream output(argv[1], ios::out | ios::trunc | ios::binary);
		if (!phoneList.SerializeToOstream(&output)) {
			cerr << "Failed to write phone message." << endl;
			return -1;
		}

		//if (!phoneList.SerializeToArray(&output,)) {
		//	cerr << "Failed to write phone message." << endl;
		//	return -1;
		//}
	}

	// Add an address.
	//AddPhoneData(phoneList.add_phones(), 0002, "Fresno2", madera::Phone::VODAPHONE, true, madera::Phone::APPLE);


	//{
	//	// Write the new address book back to disk.
	//	// phone.SerializeToArray 
	//	fstream output(argv[1], ios::out | ios::trunc | ios::binary);
	//	if (!phoneList.SerializeToOstream(&output)) {
	//		cerr << "Failed to write phone message." << endl;
	//		return -1;
	//	}
	//}

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}
