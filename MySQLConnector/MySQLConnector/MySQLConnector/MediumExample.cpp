#include <stdlib.h>
#include <iostream>
#include <mysql/jdbc.h>
using namespace std;
int main(void)
{
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::ResultSet* res;
	driver = get_driver_instance();
	con = driver->connect("host", "usert", "password");
	con->setSchema("testdb"); //select any pre-existing schema
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT * from x"); //select all values from table x in testdb
	while (res->next()) {
		cout << res->getString(1) << endl;
	}
	delete res;
	delete stmt;
	delete con;
}