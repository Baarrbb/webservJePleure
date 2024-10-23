
#include "Webserv.hpp"

static void	InitOptions(std::vector<std::string> &options)
{
	options.push_back("server_name");
	options.push_back("listen");
	options.push_back("root");
	options.push_back("index");
	options.push_back("allow_methods");
	options.push_back("location");
	options.push_back("alias");
	options.push_back("cgi_pass");
	options.push_back("client_body_limit_size");
	options.push_back("}");
	options.push_back("host");
	options.push_back("default_error_page");
	options.push_back("autoindex");
}

int	Server::InitLocation(std::istringstream &iss, std::ifstream &file)
{
	//struct stat info;
	std::string path;
	if (iss >> path && !path.empty()) // && stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode)
		this->AddLocation(path);
	else
		return (std::cout << "chemin pas valide\n", 0);
	if (iss >> path && path == "{")
	{
		this->GetLocation(this->GetLocation().size() - 1)->ServerParse(file);
		// this->GetLocation(1)->ServerParse(file);
	}
	else
		return (std::cout << "pas de {\n", 0);
	return (1);
}

int	Server::hostportadd(std::istringstream &iss)
{
	std::string	value;
	size_t		pos;
	std::string	endd;

	if (iss >> value && !value.empty())
	{
		pos = value.find(":");
		if (pos == std::string::npos)
		{
			this->host.push_back("0.0.0.0");
			this->port.push_back(value.substr(0, value.size() - 1));
			this->yipi.push_back(true);
		}
		else
		{
			this->host.push_back(value.substr(0, pos));
			pos++;
			this->port.push_back(value.substr(pos, value.size() - 1 - pos));
			this->yipi.push_back(true);
		}
	}
	else
		return (0);
	if (!(iss >> endd) || endd.empty() == false || value[value.size() - 1] != ';')
		return (0);
	return (1); 
}

static bool IsErrorCodeWellWritten(const std::string& code)
{
	if (code.size() != 3)
		return false;
	for (size_t i = 0; i < code.size(); ++i)
		if (!isdigit(code[i])) 
			return false;
	return true;
}

static int ErrorPage(std::istringstream &iss, std::string&option, std::vector<int>&options)
{
	std::string		value;
	std::string		end;
	char*			endptr;
	errno = 0;

	while (iss >> value)
	{
		std::cout << "lol value ;" << value;
		if (IsErrorCodeWellWritten(value))
		{
			long number = strtol(value.c_str(), &endptr, 10);
			options.push_back(static_cast<int>(number));
			if (errno != 0 || *endptr != '\0' || number > INT_MAX || number < INT_MIN)
				return (0);
		}
		else
			option = value;
	}
	if (!(iss >> end) || options.empty() == false || end.empty() == false || value[value.size() - 1] != ';')
		return (0);
	return (1);
}

static int	OnOrOff(std::istringstream &iss, std::string& option)
{
	std::string value;
	std::string end;
	if (iss >> value && value.empty() != 0)
	{
		if (value.compare("on;") == 0 || value.compare("off;") == 0)
			option = value.substr(0, value.size() - 1);
		//else if (value.compare("off;") == 0)
		//	option = false;
		else
			return (0);
	}
	else
		return (0);
	iss >> end;
	if (!(iss >> end) || end.empty() == false || value[value.size() - 1] != ';')
		return (0);
	return (1);
}

static int	fonctionbiss(std::istringstream &iss, std::vector<std::string>& option)
{
	std::string value;
	std::string  end;

	while (iss >> value && !value.empty())
	{
		if (value[value.size() - 1] != ';')
			option.push_back(value);
		else
			option.push_back(value.substr(0, value.size() - 1));
	}
	if (!(iss >> end) || end.empty() == false || value[value.size() - 1] != ';')
		return (0);
	return (1);
}

static int	fonctionbis(std::istringstream &iss, std::string& option)
{
	std::string value;
	std::string end;

	if (iss >> value && !value.empty())
		option = value.substr(0, value.size() - 1);
	else
		return (0);
	iss >> end;
	if (end.empty() == false || value[value.size() - 1] != ';')
		return (0);
	return (1);
}

int	Server::ProcessDirective(const std::string& line, std::ifstream &file)
{
	std::string directive;
	std::vector<std::string> options;
	InitOptions(options);

	std::istringstream iss(line);
	if (!(iss >> directive))
		return 0;
	std::vector<std::string>::iterator it = std::find(options.begin(), options.end(), directive);
	std::cout << "wtf" << directive << std::endl;
	if (it != options.end())
	{
		int index = std::distance(options.begin(), it);
		switch(index)
		{
			case SERVER_NAME:
				std::cout<< "0\n";
				if (fonctionbiss(iss, this->server_name) == 0)
				{
					throw Server::errors("error in server_name");
				}
				break;
			case LISTEN:
				std::cout<< "1\n";
				if (hostportadd(iss) == 0)
				{
					throw Server::errors("error in port");
				}
				break;
			case ROOT:
				// std::cout << "2" << std::endl;
				if (fonctionbis(iss, this->root) == 0)
				{
					throw Server::errors("error in root");
				}
				break;
			case INDEX:
				// std::cout << "3" << std::endl;
				if (fonctionbiss(iss, this->index) == 0)
				{
					throw Server::errors("error in index");
				}
				break;
			case ALLOWMETHODS:
				// std::cout << "4" << std::endl;
				if (fonctionbiss(iss, this->allow_methods) == 0)
				{
					throw Server::errors("error in allow_methods");
				}
				break;
			case LOCATION:
				// std::cout << "5" << std::endl;
				if (InitLocation(iss, file) == 0)
					throw Server::errors("error in location");
				break;
			case ALIAS:
				// std::cout << "6" << std::endl;
				if (fonctionbis(iss, this->alias) == 0)
				{
					throw Server::errors("error in alias");
				}
				break;
			case CGI_PASS:
				// std::cout << "7" << std::endl;
				if (fonctionbis(iss, this->cgi_pass) == 0)
				{
					throw Server::errors("error in cgi pass");
				}
				break;
			case CLIENT_BODY_SIZE:
				// std::cout << "8" << std::endl;
				if (fonctionbis(iss, this->client_body_limit_size) == 0)
				{
					throw Server::errors("error in client body size");
				}
				break;
			case END:
				// std::cout << "9" << std::endl;
				std::cout << "line : " << directive << " ";
				std::cout << "end of server" << std::endl;
				return (2);
			case HOST:
				// std::cout << "10" << std::endl;
				if (fonctionbiss(iss, this->host) == 0)
				{
					throw Server::errors("error in host");
				}
				break;
			case DEFAULT_ERROR_PAGE:
				// std::cout << "11" << std::endl;
				if (ErrorPage(iss, this->error_page, this->code_error_pages) == 0)
				{
					throw Server::errors("error in error page");
				}
				break;
			case AUTO_INDEX:
				std::cout << "999999999999999999" << std::endl;
				if (OnOrOff(iss, this->autoindex) == 0)
				{
					throw Server::errors("error in autoindex");
				}
				break;
			default:
				std::cout << "Error : Unknow found" << std::endl;
				throw Server::errors("ERROR IN IDK CMT IL EST PASSE");
		}
	}
	else
	{
		std::cout << "the directive " << directive << "is not inside the table !" << std::endl;
		throw Server::errors("test");
	}
	return (1);
}

int	Server::ServerParse(std::ifstream &file)
{
	std::string line;
	bool		continueProcessing = true;

	if (file.is_open())
	{
		while (continueProcessing && std::getline(file, line))
		{
			try
			{
				if (ProcessDirective(line, file) == 2)
					break ;
			}
			catch(const Server::errors &e)
			{
				std::cerr << "Exception : " << e.what() << std::endl;
			}
		}
	}
	ValidateNginxConfig();
	//std::cout << line;
	return(0);
}