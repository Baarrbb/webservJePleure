
#include "Webserv.hpp"

Server::Server()
{
	test = 9;
	this->_locations = std::vector<Location*>();
	this->server_name = std::vector<std::string>();
	this->port = std::vector<std::string>();
	this->host = std::vector<std::string>();
	this->index = std::vector<std::string>();
	this->root = "";
	this->allow_methods = std::vector<std::string>();
	this->yipi = std::vector<bool>();
	this->alias = "";
	this->cgi_pass = "";
	this->client_body_buffer_size = "";
	this->error_page = "";
}
/*Server::Server(std::string filename)
{
	this->_filename = filename;
}*/
Server::~Server()
{
	for (size_t i = 0; i < _locations.size(); ++i)
		delete _locations[i];
}

Server::Server (const Server &copy)
{
	*this = copy;
}

Server & Server::operator = (const Server &copy)
{
	if (this != &copy)
	{
		for (size_t i = 0; i < this->_locations.size(); ++i)
			delete this->_locations[i];
		this->_locations.clear();
		for (size_t i = 0; i < copy._locations.size(); i++)
		{
			this->_locations.push_back(new Location(*copy._locations[i]));
		}

		this->server_name = copy.server_name;
		this->port = copy.port;
		this->host = copy.host;
		this->index = copy.index;
		this->root = copy.root;
		this->allow_methods = copy.allow_methods;
		this->yipi = copy.yipi;
		this->alias = copy.alias;
		this->cgi_pass = copy.cgi_pass;
		this->client_body_buffer_size = copy.client_body_buffer_size;
		this->error_page = copy.error_page;
		this->test = copy.test;
	}
	return *this;
}

std::vector<Location*>	Server::GetLocation() const
{
	return (this->_locations);
}

void	Server::SetLocation(std::vector<Location*> location)
{
	this->_locations = location;
}

void	Server::AddLocation(std::string path_name)
{
	Location *newserv = new Location(path_name);
	this->_locations.push_back(newserv); // Make sure the pointer is valid
}

void	Server::AddLocation(Location *location)
{
	this->_locations.push_back(location);
}

Location* &Server::GetLocation(unsigned int index)
{
	// if (index == 0)
	// 	throw Server::WrongExpression();
	// index--;
	if (index < static_cast<size_t>(_locations.size()))
		return _locations[index];
	throw Server::WrongExpression();
}

int	Server::Gettest() const
{
	return this->test;
}

void	Server::Settest(int test)
{
	this->test = test;
}

std::ostream	&operator<<(std::ostream &out, Server &to_write) // rajouter le const apr tt les tests
{
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
	return (out);
}