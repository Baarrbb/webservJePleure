
#include "Webserv.hpp"

const char *Server::WrongExpression::what(void) const throw()
{
	return ("Index too far");
}

Server::errors::errors(const std::string& message)
{
	this->_msg = message;
}
const char* Server::errors::what() const throw()
{
	return _msg.c_str();
}

Server::errors::~errors() throw() {}