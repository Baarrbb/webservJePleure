
#include "Webserv.hpp"

Config::Config()
{
	test = 1;
	// this->_filename = "filepardefaultjsplequel";
	// this->_servers = std::vector<Server*>();
}

Config::Config(std::string filename)
{
	this->test = 1;
	this->_filename = filename;
	// this->_servers = std::vector<Server*>();
}
Config::~Config()
{
	for (size_t i = 0; i < _servers.size(); ++i)
		delete _servers[i];
}

Config::Config (const Config &copy)
{
	*this = copy;
}

Config & Config::operator = (const Config &copy)
{
	if (this != &copy)
	{
		this->test = copy.test;
		this->_filename = copy._filename;
	}
	return *this;
}

void	Config::PrintAllServers()
{
	for (int i = 0; i < static_cast<int>(this->_servers.size()); i++)
	{
		std::cout << "server numero : " << i << std::endl;
		std::cout << (this->_servers)[i] << std::endl;
		std::cout << "fin du serveur numer : " << i << std::endl;
	}
}

std::string	Config::GetFilename() const
{
	return this->_filename;
}

void	Config::SetFilename(std::string filename)
{
	this->_filename = filename;
}

std::vector<Server*>	&Config::GetServer()
{
	return (this->_servers);
}

void	Config::SetServer(std::vector<Server*> server)
{
	this->_servers = server;
}

void	Config::AddServer(Server *server)
{
	this->_servers.push_back(server);
}

void	Config::AddServer()
{
	Server *newserv = new Server;
	this->_servers.push_back(newserv);
}

Server* &Config::GetServer(unsigned int index)
{
	if (index == 0)
		throw Config::WrongExpression();
	index--;
	if (index < static_cast<size_t>(_servers.size()))
		return _servers[index];
	throw Config::WrongExpression();
}

//exceptions

const char *Config::WrongExpression::what(void) const throw()
{
	return ("Index too far");
}

//template get/setter
int	Config::Gettest() const
{
	return this->test;
}

void	Config::Settest(int test)
{
	this->test = test;
}


std::ostream	&operator<<(std::ostream &out, Config &to_write) // rajouter le const apr tt les tests
{
	out << "The file name is " << to_write.GetFilename() << std::endl;
	for (size_t i = 0; i < to_write.GetServer().size(); i++)
	{
		out << "print server :" << std::endl << *(to_write.GetServer(i + 1)) << std::endl;
	}
	return (out);
}