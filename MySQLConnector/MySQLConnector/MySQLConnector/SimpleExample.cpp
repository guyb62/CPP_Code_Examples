#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "root" 
#define EXAMPLE_PASS "root" 
#define EXAMPLE_DB "world"


int main(int argc, const char **argv)
{
	std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	std::cout << "Connector/C++ tutorial framework..." << std::endl;
	std::cout << std::endl;

	try {
		sql::Driver *driver = get_driver_instance();
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::PreparedStatement *pstmt;
		con = driver->connect(url, user, pass);
		con->setSchema(database);

		if (con->isValid())
		{
			std::cout << "Connection is valid" << std::endl;
		}
		else
		{
			std::cout << "Connection is not valid" << std::endl;
			delete con;
			return EXIT_FAILURE;
		}


		// running simple queries and creating statements:
		std::string tableName = "test";
		stmt = con->createStatement();		
		stmt->execute("USE " EXAMPLE_DB);
		stmt->execute("DROP TABLE IF EXISTS " + tableName);
		stmt->execute("CREATE TABLE " + tableName + "(id INT, label CHAR(1))");
		stmt->execute("INSERT INTO " + tableName + "(id, label) VALUES (1, 'a')"); // using SQL commands
		stmt->execute("INSERT INTO " + tableName + "(id, label) VALUES (2, 'b')");

		// using prepared statements:
		sql::PreparedStatement *prep_stmt;
		prep_stmt = con->prepareStatement("INSERT INTO " + tableName + "(id, label) VALUES (?, ?)");
		std::vector<std::string> labels;
		labels = { "c", "d", "e", "f", "h", "i", "j", "k" };

		for (int i = 3; i <= 10; i++) {
			prep_stmt->setInt(1, i);
			prep_stmt->setString(2, labels[i - 3]);
			prep_stmt->executeUpdate();
		}
		

		// Fetching results
		res = stmt->executeQuery("SELECT id, label FROM " + tableName + " ORDER BY id ASC");
		while (res->next()) {
			// You can use either numeric offsets and strings...
			std::cout << "id = " << res->getInt("id"); // getInt(1) returns the first column as well
			std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
		}

		delete prep_stmt;
		delete res;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		/*
		  MySQL Connector/C++ throws three different exceptions:

		  - sql::MethodNotImplementedException (derived from sql::SQLException)
		  - sql::InvalidArgumentException (derived from sql::SQLException)
		  - sql::SQLException (derived from std::runtime_error)
		*/
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		/* what() (derived from std::runtime_error) fetches error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "Done." << std::endl;
	return EXIT_SUCCESS;
}
