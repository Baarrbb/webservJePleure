
#include "Webserv.hpp"

void Server::SetServerName(const std::string& value)
{
	this->server_name.push_back(value);
}

void Server::SetPort(const std::string& value)
{
	this->port.push_back(value);
}

void Server::SetHost(const std::string& value)
{
	this->host.push_back(value);
}

void Server::SetRoot(const std::string& value)
{
	this->root = value;
}

void Server::SetIndex(const std::string& value)
{
	this->index.push_back(value);
}

void Server::SetErrorPage(const std::string& value)
{
	this->error_page = value;
}

void Server::SetAllowMethods(const std::string& value)
{
	this->allow_methods.push_back(value);
}

void Server::SetAlias(const std::string& value)
{
	this->alias = value;
}

void Server::SetCgiPass(const std::string& value)
{
	this->cgi_pass = value;
}

void Server::SetClientBodyBufferSize(const std::string& value)
{
	this->client_body_limit_size = value;	
}

void	Server::SetSocket(bool info)
{
	this->yipi.push_back(info);
}

void	Server::setAutoIndex(std::string i)
{
	this->autoindex = i;
}

void	Server::setBodyLimit( long o )
{
	this->octet_body_size = o;
}

void	Server::setCodeError( std::vector<int> codes )
{
	this->code_error_pages = codes;
}


// Get methods

std::vector<std::string> Server::GetServerName() const
{
	return this->server_name;
}

std::vector<std::string> Server::GetPort() const
{
	return this->port;
}

std::vector<std::string> Server::GetHost() const
{
	return this->host;
}

std::vector<std::string> Server::GetIndex() const
{
	return this->index;
}

std::vector<std::string> Server::GetAllowMethods() const
{
	return this->allow_methods;
}

std::string Server::GetServerName(const int& index) const
{
	if (index >= 0 && index < static_cast<int>(this->server_name.size()))
		return this->server_name[index]; // Get the server name at the given index
	throw Server::WrongExpression();
}

std::string Server::GetPort(const int& index) const
{
	if (index >= 0 && index < static_cast<int>(this->port.size()))
		return this->port[index]; // Get the port at the given index
	throw Server::WrongExpression();
}

std::string Server::GetHost(const int& index) const
{
	if (index >= 0 && index < static_cast<int>(this->host.size()))
		return this->host[index]; // Get the host at the given index
	throw Server::WrongExpression();
}

std::string Server::GetIndex(const int& index) const
{
	if (index >= 0 && index < static_cast<int>(this->index.size()))
		return this->index[index]; // Get the index at the given index
	throw Server::WrongExpression();
}

std::string Server::GetAllowMethods(const int& index) const
{
	if (index >= 0 && index < static_cast<int>(this->allow_methods.size()))
		return this->allow_methods[index]; // Get the allowed method at the given index
	throw Server::WrongExpression();
}

std::string Server::GetRoot() const
{
	return this->root; // Get the root string
}

std::string Server::GetErrorPage() const
{
	return this->error_page; // Get the error page string
}

std::string Server::GetAlias() const
{
	return this->alias; // Get the alias string
}

std::string Server::GetCgiPass() const
{
	return this->cgi_pass; // Get the cgi_pass string
}

std::string Server::GetClientBodyBufferSize() const
{
	return this->client_body_limit_size; // Get the client body buffer size string
}

bool	Server::GetSocket(const int& index) const
{
	if (index >= 0 && index < static_cast<int>(this->yipi.size()))
		return this->yipi[index]; // Get the allowed method at the given index
	throw Server::WrongExpression();
}

std::string	Server::getAutoIndex( void ) const
{
	return this->autoindex;
}

long	Server::getBodyLimit( void ) const
{
	return this->octet_body_size;
}

std::vector<int>	Server::getCodeError( void ) const
{
	return this->code_error_pages;
}

// Write

void Server::WriteServerName() const
{
	int index = 0;
	std::vector<std::string>::const_iterator it = this->server_name.begin();
	while (it != server_name.end())
	{
		std::cout << "tab number " << index << " value is: " << *it << std::endl;
		it++;
		index++;
	}
}

void	Server::WritePort() const
{
	int index = 0;
	std::vector<std::string>::const_iterator it = this->port.begin();
	while (it != port.end())
	{
		std::cout << "tab number " << index << " value is: " << *it << std::endl;
		it++;
		index++;
	}
}

void	Server::WriteHost() const
{
	int index = 0;
	std::vector<std::string>::const_iterator it = this->host.begin();
	while (it != host.end())
	{
		std::cout << "tab number " << index << " value is: " << *it << std::endl;
		it++;
		index++;
	}
}

void	Server::WriteRoot() const
{
	std::cout << this->root << std::endl;
}

void	Server::WriteIndex() const
{
	int indexx = 0;
	std::vector<std::string>::const_iterator it = this->index.begin();
	while (it != index.end())
	{
		std::cout << "tab number " << indexx << " value is: " << *it << std::endl;
		it++;
		indexx++;
	}
}

void	Server::WriteErrorPage() const
{
	std::cout << this->error_page << std::endl;
}

void	Server::WriteAllowMethods() const
{
	int index = 0;
	std::vector<std::string>::const_iterator it = this->allow_methods.begin();
	while (it != allow_methods.end())
	{
		std::cout << "tab number " << index << " value is: " << *it << std::endl;
		it++;
		index++;
	}
}

void	Server::WriteAlias() const
{
	std::cout << this->alias << std::endl;
}

void	Server::WriteCgiPass() const
{
	std::cout << this->cgi_pass << std::endl;
}

void	Server::WriteClientBodyBufferSize() const
{
	std::cout << this->cgi_pass << std::endl;
}

void	Server::WriteLocation()
{
	if (_locations.size() > 0)
	{
		for (std::vector<Location*>::iterator it = _locations.begin(); it != _locations.end() ;it++)
		{
			std::cout << *it;
		}
		return ;
	}
	std::cout << "pas de location pr le moment" << std::endl;
}

void	Server::WriteSocket() const
{
	int index = 0;
	std::vector<bool>::const_iterator it = this->yipi.begin();
	while (it != yipi.end())
	{
		std::cout << "tab number " << index << " value is: " << *it << std::endl;
		it++;
		index++;
	}
}


//adders

void	Server::AddSocket(bool info, int index)
{
	if (index >= 0 && index < static_cast<int>(this->yipi.size()))
	{
		this->yipi[index] = info;
		return ;
	}
	throw Server::WrongExpression();
}
