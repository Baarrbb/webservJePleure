/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestClient.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:45:26 by marvin            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/10/19 18:58:48 by marvin           ###   ########.fr       */
=======
/*   Updated: 2024/10/19 00:49:14 by ersees           ###   ########.fr       */
>>>>>>> refs/remotes/origin/master
/*                                                                            */
/* ************************************************************************** */

#include "RequestClient.hpp"

void	printOptions(std::map<std::string, std::string> options);

<<<<<<< HEAD
RequestClient::RequestClient(std::string &req) : error(0), cookie(false)
{
	std::cout << std::endl << "." << req << "." <<  std::endl << std::endl;
=======
// PRENDRE LE PARSING DE CONFIG ET COMPARER METHODS ACCEPTE
// EN FONCTION DE LA LOC AUSSI (JPP)
// SI PAS ACCEPTE RENVOYER ERREUR 403
// AVANT DE CHECK SI METHOD EXIST (DONC AVANT ERREUR 405 NOT ALLOWED)

void	RequestClient::takeCookie()
{
	/*std::cout << this->options["cookie"] << std::endl;
	if (!this->options["cookie"].empty())
	{
		this->cookie = true;
		std::ifstream line(this->options["cookie"]);

		
		return ;
	}
	this->cookie = false;
	return ;*/
}

RequestClient::RequestClient(std::string &req) : error(0)
{
	this->cookie = false;
	std::cout << std::endl << req << std::endl << std::endl;
	req.erase(req.length() - 1);
	std::string line = req.substr(0, req.find("\n"));
>>>>>>> refs/remotes/origin/master

	std::string	line = req.substr(0, req.find("\n"));
	try
	{
		this->badSyntax(line);
		this->addMethod(line.substr(0, line.find(" ")));
		req.erase(0, req.find("\n") + 1);
		this->othersOptions(req);
		// printOptions(this->options);
		this->addHost();
	}
	catch(RequestClient::ErrorRequest &e)
	{
		this->error = e.getError();
		this->target = e.getTarget();
		this->msgError = e.getMsg();
		return ;
	}

	if (this->options.find("cookie") != this->options.end())
		this->cookie = true;
<<<<<<< HEAD
=======
	std::cout << "method :" + this->method + "." << std::endl;
	std::cout << "file :" + this->target + "." << std::endl;
>>>>>>> refs/remotes/origin/master
}

RequestClient::~RequestClient( void )
{}

std::string	eraseSpace(std::string str)
{
	while(str[0] == ' ')
		str.erase(0, 1);
	return str;
}

std::string eraseSpaceAtTheEnd(std::string str)
{
	std::string::iterator it = str.end();
	while(it != str.begin() && isspace(*(it - 1)))
		--it;
	str.erase(it, str.end());
	return str;
}

// std::string	eraseAllSpace(std::string str)
// {
// 	while(isspace(str[0]))
// 		str.erase(0, 1);
// 	return str;
// }

int	checkUpCase(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isupper(str[i]))
			return 0;
	}
	return 1;
}

int	spaceInside( std::string &str )
{
	if (str.find(" ") != std::string::npos || str.find("\t") != std::string::npos)
		return 1;
	return 0;
}

void	RequestClient::badSyntax( std::string line )
{
	if (line.empty())
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	
	std::string tmp = line;
	tmp.erase(tmp.length() - 1);

	size_t		space = tmp.find(" ");
	std::string	method = tmp.substr(0, space);
	if (!checkUpCase(method) || space == std::string::npos || space == 0)
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	tmp.erase(0, space);
	tmp = eraseSpace(tmp);
	if (tmp[0] != '/' && tmp.find("http://", 0, 7) == std::string::npos)
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	space = tmp.find(" ");
	if (!this->addTarget(tmp.substr(0, space)))
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	tmp.erase(0, space);
	tmp = eraseSpace(tmp);
	size_t	version = tmp.find("HTTP/");
	if (version == std::string::npos)
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	tmp.erase(0, 5);
	space = tmp.find_last_not_of(' ');
	if (space != std::string::npos)
		tmp = tmp.substr(0, space + 1);
	if (tmp.find("\t") != std::string::npos)
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	if (tmp[0] != '1' && isdigit(tmp[0]))
		throw RequestClient::ErrorRequest(505, "not_found/505.html", "HTTP Version Not Supported");
	if (tmp.compare(0, 2, "1.") || tmp.length() != 3)
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	if (!tmp.compare(0, 2, "1.") && !isdigit(tmp[2]))
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
}

void	RequestClient::addMethod( std::string arg)
{
	std::string allow_methods[4] = {"GET", "HEAD", "POST", "DELETE"};
	int i = 0;
	for (; i < 4; i++)
	{
		if (!arg.compare(allow_methods[i]))
		{
			this->method = arg;
			break;
		}
	}
	if (i == 4)
		throw RequestClient::ErrorRequest(405, "not_found/405.html", "Not Allowed");
}

int	RequestClient::addTarget( std::string arg )
{
	size_t	delim;
	this->target = arg;
	if (arg[0] == '/')
	{
		this->target = arg;
		return 1;
	}
	arg.erase(0, 7);
	if (arg.empty() || arg[0] == '/')
		return 0;
	delim = arg.find("/");
	if (delim == std::string::npos)
		this->target = "/";
	else
		this->target = arg.substr(delim, arg.length());
	return 1;
}

void	RequestClient::addOptions( std::string line )
{
	std::string	name;
	std::string	val;

	size_t pos = line.find(":");
	if (pos == std::string::npos)
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	name = line.substr(0, pos);
	if (spaceInside(name))
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	for(size_t i = 0; i < name.length(); i++)
		name[i] = std::tolower(name[i]);
	line.erase(0, pos + 1);
	// line = eraseAllSpace(line);
	line = eraseSpace(line);
	if (line.empty()) // est ce que si il ya des tab je considere faux ?? nginx non
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	line = eraseSpaceAtTheEnd(line);
	val = line;
	if (!val.compare("host") && checkIfHost())
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
	this->options.insert(std::make_pair(name, val));
}

int	RequestClient::checkIfHost( void )
{
	std::map<std::string, std::string>::const_iterator	it = this->options.find("host");

	if (it != this->options.end())
		return 1;
	return 0;
}

void	RequestClient::addHost( void )
{
	std::map<std::string, std::string>::const_iterator	it = this->options.find("host");
	
	if (it != this->options.end() && !it->second.empty() && it->second.find("\t") == std::string::npos)
		this->host = it->second;
	else
		throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
}

void	RequestClient::othersOptions( std::string next )
{
	while (!next.empty())
	{
		std::string line = next.substr(0, next.find("\n"));
		try
		{
			this->addOptions(line);
		}
		catch(RequestClient::ErrorRequest &e)
		{
			this->error = e.getError();
			this->target = e.getTarget();
			this->msgError = e.getMsg();
			return ;
		}
		next.erase(0, line.length() + 1);
	}
}


// Getters

int RequestClient::getError( void )
{
	return this->error;
}

std::string	RequestClient::getMsgError( void )
{
	return this->msgError;
}

std::string	RequestClient::getMethod( void )
{
	return this->method;
}

std::string	RequestClient::getTarget( void )
{
	return this->target;
}

std::string	RequestClient::getHost( void )
{
	return this->host;
}

std::string	RequestClient::getOptions( std::string key )
{
	std::map<std::string, std::string>::const_iterator it = this->options.find(key);
	if (it != this->options.end())
		return it->second;
	else
		return "";
}

std::map<std::string, std::string>	RequestClient::getOptions( void )
{
	return this->options;
}
bool	RequestClient::getCookie() const
{
	return this->cookie;
}

// std::string	RequestClient::getBody( void )
// {
// 	return this->body;
// }

bool		RequestClient::getCookie( void ) const
{
	return this->cookie;
}

// Setters

void	RequestClient::setCookie( bool cond)
{
	this->cookie = cond;
}
void	RequestClient::setTarget( std::string file )
{
	this->target = file;
}

void	RequestClient::setError(int err)
{
	this->error = err;
}

void	RequestClient::setMsgError( std::string msg )
{
	this->msgError = msg;
}


// A SUPP

void	printOptions(std::map<std::string, std::string> options)
{
	for (std::map<std::string, std::string>::iterator it = options.begin(); it != options.end(); ++it)
	{
		std::cout << it->first << " = " << it->second << std::endl;
	}
}
