
#include "Webserv.hpp"

bool Server::IsValidServerName(const std::string& name)
{
	return !name.empty();
}

bool Server::IsValidListen()
{
	for (std::vector<std::string>::iterator it = port.begin(); it != port.end(); ++it)
	{
		for (std::vector<std::string>::iterator itt = it + 1; itt != port.end(); ++itt)
		{
			if (!(*it).compare(*itt))
			{
				std::cout << "port utilise 2x" << std::endl;
				return false;
			}
		}
		if (strtol((*it).c_str(), NULL, 10) < 1000 || strtol((*it).c_str(), NULL, 10) > 65535)
		{
			return false; // Port invalide
		}
	}
	return true; // Port valide
}

bool Server::IsValidRoot(const std::string& root)
{
	struct stat info;
	return (stat(root.c_str(), &info) == 0 && S_ISDIR(info.st_mode)); // Vérifie si c'est un répertoire valide
}

bool Server::IsValidIndex(const std::string& index)
{
	return !index.empty();
}

bool Server::IsValidHost(const std::string& host)
{
	
	return !host.empty();
}

bool Server::IsValidDefaultErrorPage(const std::string& path)
{
	return !path.empty();
}

bool Server::IsValidClientBodySize(const std::string& size)
{
	errno = 0;
	char* endptr;
	long body_size = strtol(size.c_str(), &endptr, 10);
	if (endptr == size.c_str() || *endptr != '\0' || errno == ERANGE || body_size < 0)
		return false;
	return true;
}

bool Server::IsValidAllowMethods(const std::string& methods)
{
	std::vector<std::string> valid_methods;
	valid_methods.push_back("GET");
	valid_methods.push_back("POST");
	valid_methods.push_back("HEAD");
	valid_methods.push_back("NONE");
	
	return std::find(valid_methods.begin(), valid_methods.end(), methods) != valid_methods.end();
}

bool Server::IsValidAlias(const std::string& alias)
{
	return !alias.empty();
}

bool Server::IsValidCgiPass(const std::string& cgi_pass)
{
	//peux rajotuer des .php genre ou truc dans le genre a tester mais jsp si c est necessaire
	if (!cgi_pass.empty())
		if (access(cgi_pass.c_str(), F_OK) || access(cgi_pass.c_str(), X_OK))
        	return false;

	return true;
}

void Server::ValidateNginxConfig()
{
	for (std::vector<std::string>::iterator it = server_name.begin(); it != server_name.end(); ++it)
		if (!IsValidServerName(*it))
			std::cerr << "Invalid server name: " << *it << std::endl;

	if (!IsValidListen())
		std::cerr << "Invalid port directive: " << std::endl;

	for (std::vector<std::string>::iterator it = host.begin(); it != host.end(); ++it)
		if (!IsValidHost(*it))
			std::cerr << "Invalid host: " << *it << std::endl;

	if (!IsValidRoot(this->root))
		std::cerr << "Invalid root directory: " << this->root << std::endl;

	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); ++it)
		if (!IsValidIndex(*it))
			std::cerr << "Invalid index: " << *it << std::endl;

	if (!IsValidDefaultErrorPage(this->error_page))
		std::cerr << "Invalid default error page: " << this->error_page << std::endl;

	for (std::vector<std::string>::iterator it = this->allow_methods.begin(); it != this->allow_methods.end(); ++it)
		if (!IsValidAllowMethods(*it))
			std::cerr << "Invalid allow methods: " << *it << std::endl;
			
	if (!IsValidClientBodySize(this->client_body_buffer_size))
		std::cerr << "Invalid client body size: " << this->client_body_buffer_size << std::endl;

	if (!IsValidAlias(this->alias))			
		std::cerr << "Invalid alias: " << this->alias << std::endl;

	if (!IsValidCgiPass(this->cgi_pass))
		std::cerr << "Invalid cgi pass: " << this->cgi_pass << std::endl;
}
