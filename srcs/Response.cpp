/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:00:18 by marvin            #+#    #+#             */
/*   Updated: 2024/10/25 01:00:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

// Response::Response( RequestClient req, std::vector<Server*> serv, std::string host, uint16_t port )
Response::Response( RequestClient req, std::string bodyClient, std::vector<Server*> serv, std::string host, uint16_t port )
	: version("HTTP/1.1"), isCGI(false), bodyClient(bodyClient)
{
	Server		server;
	Location	loc;
	try
	{
		int err = req.getError();
		server = this->findConfig(serv, host, port, req.getHost());

		loc = this->findLocation(server, req.getPath());

		std::cout << "LOC:" << loc << std::endl;

		if (err == 0 || err == 405)
		{
			this->checkLimitBodySize(req, loc, bodyClient.length());
			this->checkMethodsAllowed(loc, req.getMethod());
			if (isCGI)
			{
				std::string target = this->extractPathInfo(req.getPath());
				this->file = this->findFile(loc, target, err);
			}
			else
				this->file = this->findFile(loc, req.getPath(), err);
			this->checkCGI(server, this->file);
			if (isCGI)
			{
				loc = this->findLocation(server, this->file);
				CGI callCGI(req, loc, bodyClient, this->file, this->pathInfo, this->dir);
				this->cgiBody = callCGI.getBodyResponse();
				this->cgiHeaders = callCGI.getHeadResponse();
			}
		}
		else
			this->file = this->findFile(loc, req.getTarget(), err);
	}
	catch(RequestClient::ErrorRequest& e)
	{
		// req.setMethod("GET"); a voir ??
		req.setError(e.getError());
		req.setTarget(e.getTarget());
		req.setMsgError(e.getMsg());
		this->file = req.getTarget();
	}

	std::cout << "FILE:" << this->file << std::endl;

	if (req.getError() != 301)
		this->addBody(this->file, req, loc);

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
	this->constructResponse(req);
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

Location	Response::findLocation(Server serv, std::string target)
{
	for(size_t i = 0; i < serv.GetLocation().size(); i++)
	{
		std::string	pathLoc = (serv.GetLocation(i))->getPathLoc();
		if (pathLoc[0] == '*')
		{
			std::string	ext = pathLoc.substr(1, pathLoc.length());
			if (target.find(ext) != std::string::npos)
			{
				this->isCGI = true;
				this->ext = ext;
				this->fillLoc(serv, serv.GetLocation(i));
				return (*serv.GetLocation(i));
			}
		}
	}

	for (size_t i = 0; i < serv.GetLocation().size(); i++)
	{
		if (!serv.GetLocation(i)->getPathLoc().compare(target))
		{
			this->fillLoc(serv, serv.GetLocation(i));
			// std::cout << "APRES AVOIR FILL:" << *serv.GetLocation(i) << std::endl;
			return (*serv.GetLocation(i));
		}
	}

	Location	*tmpLoc;
	std::string	tmp = target;
	std::string	part;
	while (!tmp.empty())
	{
		part = tmp.substr(0, tmp.rfind("/"));
		if (part.empty())
			part = "/";
		for (size_t i = 0; i < serv.GetLocation().size(); i++)
		{
			tmpLoc = serv.GetLocation(i);
			if (!tmpLoc->getPathLoc().compare(part))
			{
				if (tmpLoc->GetLocation().size())
				{
					this->fillLoc(serv, tmpLoc);
					Location *newLoc = this->findLocationInLocation(tmpLoc->GetLocation(), target);
					if (newLoc)
					{
						this->fillLoc(*tmpLoc, newLoc);
						return *newLoc;
					}
				}
				this->fillLoc(serv, tmpLoc);
				return (*tmpLoc);
			}
		}
		tmp.erase(tmp.rfind("/"));
	}

	return *serv.GetLocation(0);
}

Location*	Response::findLocationInLocation(std::vector<Location*> newLoc, std::string target)
{
	Location *tmpLoc;
	std::string	tmp = target;
	std::string	part;
	while (!tmp.empty())
	{
		part = tmp.substr(0, tmp.rfind("/"));
		for (size_t i = 0; i < newLoc.size(); i++)
		{
			tmpLoc = newLoc[i];
			if (!tmpLoc->getPathLoc().compare(part))
				return tmpLoc;
			if (tmpLoc->GetLocation().size())
			{
				for (size_t j = 0; j < tmpLoc->GetLocation().size(); j++)
					this->fillLoc(*tmpLoc, tmpLoc->GetLocation(j));
				return findLocationInLocation(tmpLoc->GetLocation(), target);
			}
		}
		tmp.erase(tmp.rfind("/"));
	}
	return NULL;
}

void	Response::checkLimitBodySize( RequestClient req, Location loc, size_t sizeBody )
{
	if (req.getMethod().compare("POST") || loc.getBodyLimit() == -1)
		return;

	std::cout << JAUNE << static_cast<long>(sizeBody) << std::endl;
	std::cout << loc.getBodyLimit() << RESET << std::endl;
	if (static_cast<long>(sizeBody) > loc.getBodyLimit())
		throw RequestClient::ErrorRequest(413, "./not_found/413.html", "Request Entity Too Large");
	
}

void	Response::checkMethodsAllowed( Location serv, std::string method )
{
	if (!serv.GetAllowMethods().size())
		return ;
	if (serv.GetAllowMethods(0).empty())
		return;
	size_t i = 0;
	for (; i < serv.GetAllowMethods().size(); i++ )
	{
		if (!serv.GetAllowMethods(i).compare(method))
			break ;
	}
	if (i == serv.GetAllowMethods().size())
	{
		throw RequestClient::ErrorRequest(403, "./not_found/403.html", "Forbidden");
	}
}

void	Response::checkCGI(Server serv, std::string path)
{
	size_t pos = path.rfind(".");
	if (pos != std::string::npos)
	{
		std::string ext = path.substr(pos, path.length());
		for (size_t i = 0; i < serv.GetLocation().size(); i++)
		{
			if ((serv.GetLocation(i))->getPathLoc().find(ext) != std::string::npos)
				this->isCGI = true;
		}
	}
}

std::string	Response::extractPathInfo(std::string target)
{
	std::string tmp = target;
	this->pathInfo = tmp.substr(target.find(ext) + ext.length(), target.length());
	std::string name = tmp.substr(0, target.find(ext) + ext.length());
	return name;
}

std::string	Response::findFile( Location serv, std::string target, int err )
{
	std::string			path = serv.GetRoot();
	std::ifstream		file;
	std::string			filename;
	struct stat			s;

	std::string tmp = path + target;
	if (!stat(tmp.c_str(), &s) && S_ISDIR(s.st_mode) && target[target.length() - 1] != '/')
		throw RequestClient::ErrorRequest(301, target.append("/"), "Moved Permanently");

	if (err)
		filename = target;
	else if (!stat(tmp.c_str(), &s) && S_ISDIR(s.st_mode) && target[target.length() - 1] == '/')
	{
		size_t i = 0;
		for(; i < serv.GetIndex().size(); i++)
		{
			// filename = path.append(serv.getPathLoc()).append("/").append(serv.GetIndex(i));
			std::string tmp = path;
			// filename = tmp.append(serv.getPathLoc()).append("/").append(serv.GetIndex(i));
			filename = tmp.append(target).append(serv.GetIndex(i));
			std::cout << "." << filename << "." << std::endl;
			file.open(filename.c_str());
			if (file.is_open())
			{
				file.close();
				return filename;
			}
		}
		if (i == serv.GetIndex().size() && (serv.getAutoIndex().empty() || !serv.getAutoIndex().compare("off")) )
			throw RequestClient::ErrorRequest(403, "./not_found/403.html", "Forbidden");
		else if (i == serv.GetIndex().size() && !serv.getAutoIndex().compare("on"))
		{
			this->isCGI = 1;
			this->dir = path.append(target);
			return "./not_found/dir_list.php";
		}
			// throw RequestClient::ErrorRequest(404, "./not_found/404.html", "Not Found");
	}
	else
		filename = path.append(target);

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
	if (loc->getAutoIndex().empty())
		loc->setAutoIndex(serv.getAutoIndex());
	if (loc->getBodyLimit() == -1)
		loc->setBodyLimit(serv.getBodyLimit());
	if (loc->GetErrorPage().empty())
	{
		loc->SetErrorPage(serv.GetErrorPage());
		loc->setCodeError(serv.getCodeError());
	}
}

void	Response::addBody(std::string filename, RequestClient req, Location loc)
{
	std::cout << "ERR:" << req.getError() << std::endl;
	std::vector<int> tmp = loc.getCodeError();
	std::vector<int>::iterator it = find(tmp.begin(), tmp.end(), req.getError());

	if (req.getError() != 0 && it != tmp.end())
	{
		try {
			filename = this->findFile(loc, loc.GetErrorPage(), 0);
			std::cout << "filename:" << filename << std::endl;
		}
		catch(...)
		{
		}
	}
	if (this->isCGI && req.getMethod().compare("DELETE") && req.getError() == 0)
		this->body = this->cgiBody + "\r\n";
	else if (!req.getMethod().compare("DELETE"))
		remove(filename.c_str());
	else
	{
		std::ifstream		file(filename.c_str());
		std::stringstream	fileStream;
		fileStream << file.rdbuf();
		std::string	fileContent = fileStream.str();
		this->body = fileContent + "\r\n";
	}
}

std::string SessionTokenCreate( size_t length )
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

static void	InitOptionsCookies( std::vector<std::string> &options )
{
	options.push_back("session_id");
	options.push_back("expires");
	options.push_back("path");
	options.push_back("HttpOnly");
	options.push_back("Secure");
	options.push_back("SameSite");
}

void Response::addCookieValues( void )
{
	std::vector<std::string> options;
	InitOptionsCookies(options);
	// for (size_t i = 0; i < options.size(); ++i)
	// 	std::cout << options[i] << std::endl;
}

void	Response::checkCGIHeadersStatusCode( void )
{
	if (this->cgiHeaders.empty())
		return;
	if (this->cgiHeaders.find("Status:") != std::string::npos)
	{
		std::string tmp = this->cgiHeaders;
		int	posStatus = this->cgiHeaders.find("Status:");
		tmp.erase(0, posStatus);
		int	endLineStatus = tmp.find("\r\n");
		tmp.erase(endLineStatus, tmp.length());
		this->cgiHeaders.erase(posStatus, tmp.length() + 2);
		int	pos = tmp.find(":");
		tmp.erase(0, pos + 1);
		while (tmp[0] == ' ')
			tmp.erase(0, 1);
		this->code = tmp.substr(0, 3);
		tmp.erase(0, 3);
		while (tmp[0] == ' ')
			tmp.erase(0, 1);
		this->msg = tmp.substr(0, tmp.length());
	}
	else
	{
		this->code = "301";
		this->msg = "Moved Permanently";
	}
}

void	Response::constructResponse(RequestClient req)
{
	if (!this->cgiHeaders.empty() && this->cgiHeaders.find("Location:") != std::string::npos)
		this->checkCGIHeadersStatusCode();

	this->full = this->version + " " + this->code + " " + this->msg + "\r\n";
	if (!this->cgiHeaders.empty())
		this->full += this->cgiHeaders + "\r\n";

	if (req.getError() == 301)
		this->full += "Location: " + req.getTarget() + "\r\n";

	if (!req.getCookie())
	{
		this->full += "Set-Cookie: session_id=" + SessionTokenCreate(32) + ";\r\n";
		this->addCookieValues();
	}
	this->full += "Content-Length: " + lengthBody() + "\r\n\r\n";

	if (req.getMethod().compare("HEAD"))
		this->full += this->body;
}


// GETTERS

std::string	Response::getFull( void )
{
	return this->full;
}

