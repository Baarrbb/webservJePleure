/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:00:18 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 00:44:49 by ersees           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include <ctime>

std::string SessionTokenCreate(size_t length)
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string token;
	std::srand(static_cast<unsigned int>(std::time(0)));  // Seed the random number generator

	for (size_t i = 0; i < length; ++i)
	{
		token += charset[std::rand() % (sizeof(charset) - 1)];
	}
	return token;
}

static void	InitOptionsCookies(std::vector<std::string> &options)
{
	options.push_back("session_id");
	options.push_back("expires");
	options.push_back("path");
	options.push_back("HttpOnly");
	options.push_back("Secure");
	options.push_back("SameSite");
}


void Response::addCookieValues()
{
	std::vector<std::string> options;
	InitOptionsCookies(options);
	for (size_t i = 0; i < options.size(); ++i)
		std::cout << options[i] << std::endl;
}

Response::Response( RequestClient req, std::vector<Server*> serv, std::string host, uint16_t port )
	: version("HTTP/1.1")
{
	std::cout << "host:port " << host << ":" << port << std::endl;

	Server	server;
	Location loc;
	try
	{
		int err = req.getError();
		server = this->findConfig(serv, host, port, req.getHost());

		loc = this->findLocation(server, req.getTarget());

		// this->checkMethodsAllowed(server, req.getMethod());

		this->fillLoc(server, &loc);

		this->file = this->findFile(loc, req.getTarget(), err);
	}
	catch(RequestClient::ErrorRequest& e)
	{
		req.setError(e.getError());
		req.setTarget(e.getTarget());
		req.setMsgError(e.getMsg());
		this->file = req.getTarget();
	}

	this->addBody(this->file);

	if (!req.getError())
	{
		this->code = "200";
		this->msg = "OK";
	}
	else
	{
		std::stringstream ss;
		ss << req.getError();
		this->code = ss.str();
		this->msg = req.getMsgError();
	}
	
	this->full = this->version + " " + this->code + " " + this->msg + "\n";
	if (req.getCookie() == false)
	{
		this->full += "Set-Cookie: session_id=" + SessionTokenCreate(32) + ";\r\n";
		addCookieValues();
	}
	this->full += "Content-Length: " + lengthBody() + "\n";
	this->full += "\n";
	if (req.getMethod().compare("HEAD"))
		this->full += this->body + "\n";
}

Response::~Response( void )
{
}

std::string	Response::lengthBody( void )
{
	std::stringstream len;
	len << this->body.length();
	return len.str();
}

Server	Response::findConfig( std::vector<Server*> serv, std::string host, uint16_t port, std::string reqHost)
{
	std::stringstream ssport;
	ssport << port;
	std::string sport = ssport.str();
	std::vector<size_t> posServ;

	for (size_t i = 0; i < serv.size(); i++)
	{
		for (size_t j = 0; j < serv[i]->GetPort().size(); j++ )
		{
			if ((!serv[i]->GetHost(j).compare(host) || !serv[i]->GetHost(j).compare("0.0.0.0"))
				&& !serv[i]->GetPort(j).compare(sport))
					posServ.push_back(i);
		}
	}
	if (reqHost.find(":") != std::string::npos)
		reqHost = reqHost.substr(0, reqHost.find(":"));
	if (posServ.size() == 1)
		return (*serv[posServ[0]]);
	for (size_t i = 0; i < posServ.size(); i++)
	{
		for (size_t j = 0; j < serv[posServ[i]]->GetServerName().size(); j++)
		{
			if (!serv[posServ[i]]->GetServerName(j).compare(reqHost))
				return (*serv[posServ[i]]);
		}
	}
	return (*serv[posServ[0]]);
}


// ajouter wildcard pour extension
Location	Response::findLocation(Server serv, std::string target)
{
	for (size_t i = 0; i < serv.GetLocation().size(); i++)
	{
		if (!(serv.GetLocation(i))->getPathLoc().compare(target))
		{
			std::cout << "je macth avec " << (serv.GetLocation(i))->getPathLoc() << std::endl;
			return (*serv.GetLocation(i));
			// return static_cast<Server>(*serv.GetLocation(i));
		}
	}

	size_t posLastLim = target.rfind('/');
	if (posLastLim != 0)
	{
		std::string firstPart = target.substr(0, posLastLim);
		for (size_t i = 0; i < serv.GetLocation().size(); i++)
		{
			if (!(serv.GetLocation(i))->getPathLoc().compare(firstPart))
			{
				std::cout << "je macth avec " << (serv.GetLocation(i))->getPathLoc() << std::endl;
				return (*serv.GetLocation(i));
				// return static_cast<Server>(*serv.GetLocation(i));
			}
		}
	}

	return *serv.GetLocation(0);
	// throw Response::NoMatchingConfig();
	// return *serv;
}

void	Response::checkMethodsAllowed( Server serv, std::string method )
{
	if (!serv.GetAllowMethods().size())
		return ;
	size_t i = 0;
	for (; i < serv.GetAllowMethods().size(); i++ )
	{
		if (!serv.GetAllowMethods(i).compare(method))
			break ;
	}
	if (i == serv.GetAllowMethods().size())
		throw RequestClient::ErrorRequest(403, "./not_found/403.html", "Forbidden");
}

std::string	Response::findFile( Location serv, std::string target, int err )
{
	std::cout << std::endl<< "PRINT LOC CONFIG AFTER" << std::endl;
	std::cout << serv << std::endl;

	std::string			path = serv.GetRoot();
	std::ifstream		file;
	std::string			filename;

	std::cout << "path loc " << serv.getPathLoc() << std::endl;
	std::cout << "target " << target << std::endl;

	if (err)
		filename = target;
	// else if (!target.compare("/"))
	else if (!target.compare(serv.getPathLoc()))
	{
		size_t i = 0;
		for(; i < serv.GetIndex().size(); i++)
		{
			filename = path.append(serv.getPathLoc()).append("/").append(serv.GetIndex(i));
			std::cout << "filename" << filename << std::endl;
			file.open(filename.c_str());
			if (file.is_open())
			{
				file.close();
				return filename;
			}
		}
		if (i == serv.GetIndex().size())
			throw RequestClient::ErrorRequest(404, "./not_found/404.html", "Not Found");

	}
	else
		filename = path.append(target);
	
	std::cout << filename << std::endl;
	file.open(filename.c_str());
	if (!file.is_open())
		throw RequestClient::ErrorRequest(404, "./not_found/404.html", "Not Found");

	file.close();
	return filename;
}

void	Response::fillLoc(Server serv, Location* loc)
{
	if (loc->GetRoot().empty())
		loc->SetRoot(serv.GetRoot());
	if (loc->GetIndex().empty())
	{
		for (size_t i = 0; i < serv.GetIndex().size(); i++)
			loc->SetIndex(serv.GetIndex(i));
	}
	if (loc->GetAllowMethods().empty())
	{
		for (size_t i = 0; i < serv.GetAllowMethods().size(); i++)
			loc->SetAllowMethods(serv.GetAllowMethods(i));
	}
}

void	Response::addBody(std::string filename)
{
	std::ifstream		file(filename.c_str());
	std::stringstream	fileStream;
	fileStream << file.rdbuf();

	std::string	fileContent = fileStream.str();
	this->body = fileContent;
}

std::string Response::getFull( void )
{
	return this->full;
}
