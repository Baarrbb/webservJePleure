
#include "Webserv.hpp"

Location::Location()
{
	this->_locations = std::vector<Location*>();
	this->_file_path = "default";
	this->server_name = std::vector<std::string>();
	this->port = std::vector<std::string>();
	this->host = std::vector<std::string>();
	this->index = std::vector<std::string>();
	this->root = "";
	this->allow_methods = std::vector<std::string>();
	this->alias = "";
	this->cgi_pass = "";
	this->client_body_buffer_size = "";
	this->error_page = "";
	this->yipi = std::vector<bool>();
	this->autoindex = false;
}

Location::Location(std::string filename)
{
	this->_file_path = filename;
	this->_locations = std::vector<Location*>();
	this->server_name = std::vector<std::string>();
	this->port = std::vector<std::string>();
	this->host = std::vector<std::string>();
	this->index = std::vector<std::string>();;
	this->root = "";
	this->allow_methods = std::vector<std::string>();
	this->alias = "";
	this->cgi_pass = "";
	this->client_body_buffer_size = "";
	this->error_page = "";
	this->yipi = std::vector<bool>();
	this->autoindex = false;
}

Location::~Location()
{
	// for (size_t i = 0; i < _locations.size(); ++i)
	// 	delete _locations[i];
}

Location::Location (const Location &copy) : Server(copy)
{
	this->_file_path = copy._file_path;
}

Location & Location::operator = (const Location &copy)
{
	if (this != &copy)
	{
		Server::operator=(copy);
		this->test = copy.test;
		this->_file_path = copy._file_path;
	}
	return *this;
}

/*int	Location::Gettest() const
{
	return this->test;
}

void	Location::Settest(int test)
{
	this->test = test;
}*/

std::string	Location::getPathLoc( void )
{
	return this->_file_path;
}

void	Location::WriteFilePath() const
{
	std::cout << this->_file_path << std::endl;
}

std::ostream	&operator<<(std::ostream &out, Location &to_write) // rajouter le const apr tt les tests
{
	out << "path : ";to_write.WriteFilePath();
	out << "server_name : ";to_write.WriteServerName();
	out << "port : ";to_write.WritePort();
	out << "host : ";to_write.WriteHost();
	out << "root : ";to_write.WriteRoot();
	out << "index : ";to_write.WriteIndex();
	out << "errorpage : ";to_write.WriteErrorPage();
	out << "allow_methods : ";to_write.WriteAllowMethods();
	out << "alias : ";to_write.WriteAlias();
	out << "cgipass : ";to_write.WriteCgiPass();
	out << "clientbodysize : ";to_write.WriteClientBodyBufferSize();
	out << "location :"; to_write.WriteLocation();
	out << "autoindex :" << to_write.getAutoIndex() << std::endl;
	return (out);
}
