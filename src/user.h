#ifndef USER_H 
#define USER_H 

#include <string>

class User
{
public:
	User();

	int id = 0;
	bool op = false;
	std::string username = "user";

	void receive(std::string message);
};

#endif
