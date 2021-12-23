// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

void findOccurences(std::string s)
{
	// Vector of map
	std::vector<std::map<char, int> > mp(s.length());

	// Traverse the string s
	for (int i = 0; i < s.length(); i++) {

		// Update the frequency
		for (int j = 0; j <= i; j++) {
			mp[i][s[j]]++;
		}
	}

	// Print the vector of map
	for (int i = 0; i < s.length(); i++) {

		std::cout << "Frequency upto "
			<< "position " << i + 1
			<< std::endl;

		// Traverse the map
		for (auto x : mp[i])
			std::cout << x.first << "-"
			<< x.second << std::endl;
	}
}


std::string testSwitch(int burstType)
{
	std::string messageType = "unknown";

	if (burstType % 10 != 0)
	{
		std::string burstTypeString = std::to_string(burstType);
		char backNumber = burstTypeString.back();

		switch (backNumber)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
			messageType = "voice burst";
			break;
		default:
			break;
		}
	}

	if (burstType % 10 == 0)
	{
		switch (burstType)
		{
		case 210:
		case 230:
		case 250:
		case 260: // "RC Burst"
			messageType = "data burst";
			break;
		default:
			break;
		}
	}
	return messageType;
}

int main()
{
    std::cout << "Hello World!\n";
	std::vector<std::map<std::string, double>> burstInfo;
	burstInfo.push_back(
		{
			{"FrequencyOffset", 1},
			{"BurstType", 2},
			{"StartSample", 3},
			{"EndSample", 4},
			{"TxNumber", 5},
			{"SlotIndex", 6},
			{"SlotNumber", 7},
			{"IsDownlink", 8}
		});
	
	auto test = burstInfo.front().begin();


	std::vector<std::map<std::string, double>> testCollection;

	std::map<std::string, int> mapOfWords;
	// Inserting data in std::map
	mapOfWords.insert(std::make_pair("earth", 1));
	mapOfWords.insert(std::make_pair("moon", 2));
	mapOfWords["sun"] = 3;
	// Will replace the value of already added key i.e. earth
	mapOfWords["earth"] = 4;
	// Iterate through all elements in std::map
	std::map<std::string, int>::iterator it = mapOfWords.begin();
	while (it != mapOfWords.end())
	{
		std::cout << it->first << " :: " << it->second << std::endl;
		it++;
	}
	// Check if insertion is successful or not
	if (mapOfWords.insert(std::make_pair("earth", 1)).second == false)
	{
		std::cout << "Element with key 'earth' not inserted because already existed" << std::endl;
	}
	// Searching element in std::map by key.
	if (mapOfWords.find("sun") != mapOfWords.end())
		std::cout << "word 'sun' found" << std::endl;
	if (mapOfWords.find("mars") == mapOfWords.end())
		std::cout << "word 'mars' not found" << std::endl;


	// vector of maps example geeks for geeks:
	// Input string S
	std::string S = "geeks";

	// Function Call
	findOccurences(S);
	
	int burstType = 230;
	std::string messageType = testSwitch(burstType);
	std::string messageType2 = testSwitch(220);
	std::string messageType3 = testSwitch(221);
	std::string messageType4 = testSwitch(110);
	std::string messageType5 = testSwitch(101);
	std::string messageType6 = testSwitch(260);
	std::string messageType7 = testSwitch(201);

	int test1 = 230;
	int test2 = 241;
	int test3 = 246;
	auto s = std::to_string(230);
	auto s2 = std::to_string(test1);
	std::stringstream ss;
	std::string s3;
	ss << test1;
	ss >> s3;
	auto it1 = s3.end();
	auto it2 = s3.crend();
	auto back = s3.back();
	return 0;
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
