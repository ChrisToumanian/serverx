#ifndef USER_H 
#define USER_H 

#include <string>

class User
{
public:
	User();

	int id = 0;
	std::string uid = "";
	std::string username = "user";
	bool op = false;

	void receive(std::string message);
};

#endif
