/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:00:18 by marvin            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/10/20 01:47:27 by marvin           ###   ########.fr       */
=======
/*   Updated: 2024/10/19 00:44:49 by ersees           ###   ########.fr       */
>>>>>>> refs/remotes/origin/master
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

// Response::Response( RequestClient req, std::vector<Server*> serv, std::string host, uint16_t port )
Response::Response( RequestClient req, std::string bodyClient, std::vector<Server*> serv, std::string host, uint16_t port )
	: version("HTTP/1.1"), isCGI(false), bodyClient(bodyClient)
{
	std::cout << "host:port " << host << ":" << port << std::endl;

	// std::cout << "ICI BODYCLIENT:" << this->bodyClient << std::endl;

	Server		server;
	Location	loc;
	try
	{
		int err = req.getError();
		server = this->findConfig(serv, host, port, req.getHost());
		loc = this->findLocation(server, req.getTarget());
		this->fillLoc(server, &loc);

		this->checkMethodsAllowed(loc, req.getMethod());

		// std::cout << std::endl<< "PRINT LOC CONFIG AFTER" << std::endl;
		// std::cout << loc << std::endl;

		// std::cout << "path loc " << loc.getPathLoc() << std::endl;
		// std::cout << "target " << req.getTarget() << std::endl;
		this->file = this->findFile(loc, req.getTarget(), err);

	}
	catch(RequestClient::ErrorRequest& e)
	{
		req.setError(e.getError());
		req.setTarget(e.getTarget());
		req.setMsgError(e.getMsg());
		this->file = req.getTarget();
	}

	std::cout << "FILE QUI QU'ON VA OUVRIR " << this->file << std::endl << std::endl;
	if (req.getError() != 301)
		this->addBody(this->file, req);

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
<<<<<<< HEAD
	this->constructResponse(req);
=======
	
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
>>>>>>> refs/remotes/origin/master
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
		std::string pathLoc = (serv.GetLocation(i))->getPathLoc();
		if (pathLoc[0] == '*')
		{
			std::string	ext = pathLoc.substr(1, pathLoc.length());
			if (target.find(ext) != std::string::npos)
			{
				this->isCGI = true;
				this->execCGI = (serv.GetLocation(i))->GetCgiPass();
				return (*serv.GetLocation(i));
			}
		}
	}

	for (size_t i = 0; i < serv.GetLocation().size(); i++)
	{
		if (!(serv.GetLocation(i))->getPathLoc().compare(target))
			return (*serv.GetLocation(i));
	}

	size_t	delim = 0;
	// partir par la fin plutot pour aller de l'url la plus grande a la plus petite ?
	while ((delim = target.find("/", delim + 1)) == 0) ; 
	std::string firstPart = target.substr(0, delim);
	for (size_t i = 0; i < serv.GetLocation().size(); i++)
	{
		if (!(serv.GetLocation(i))->getPathLoc().compare(firstPart))
			return (*serv.GetLocation(i));
	}

	for (size_t i = 0; i < serv.GetLocation().size(); i++)
	{
		if (!(serv.GetLocation(i))->getPathLoc().compare("/"))
			return (*serv.GetLocation(i));
	}

	return *serv.GetLocation(0);
}

void	Response::checkMethodsAllowed( Location serv, std::string method )
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
	std::cout << serv << std::endl;
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
			filename = tmp.append(target).append("/").append(serv.GetIndex(i));
			std::cout << "." << filename << "." << std::endl;
			file.open(filename.c_str());
			if (file.is_open())
			{
				file.close();
				return filename;
			}
		}
		if (i == serv.GetIndex().size() && !serv.getAutoIndex())
			throw RequestClient::ErrorRequest(403, "./not_found/403.html", "Forbidden");
		else if (i == serv.GetIndex().size() && serv.getAutoIndex())
		{
			this->isCGI = 1;
			this->execCGI = "/usr/bin/php-cgi";
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
	// pb je peux pas faire diff si c pas precise ou si c mis sur off
	// ou alors general predommine sur loc ?
	if (!loc->getAutoIndex())
		loc->setAutoIndex(serv.getAutoIndex());
}

void	Response::addBody(std::string filename, RequestClient req)
{
	std::ifstream		file(filename.c_str());
	std::stringstream	fileStream;
	fileStream << file.rdbuf();

	if (this->isCGI && req.getMethod().compare("DELETE"))
		this->handleCGI(filename, req);
	else if (!req.getMethod().compare("DELETE"))
		remove(filename.c_str());
	else
	{
		std::string	fileContent = fileStream.str();
		this->body = fileContent + "\r\n";
	}
}

std::string	Response::getFull( void )
{
	return this->full;
}

void	Response::toCGI( std::string filename, RequestClient req, int pipefd[2], int pipefd_stdin[2])
{
	close(pipefd[0]);
	close(pipefd_stdin[1]);
	if (dup2(pipefd[1], 1) == -1 || dup2(pipefd_stdin[0], STDIN_FILENO) == -1)
	{
		std::cerr << "Error: dup2: " << strerror(errno) << std::endl;
		close(pipefd[1]);
		close(pipefd_stdin[0]);
		exit(127);
	}

	close(pipefd[1]);
	close(pipefd_stdin[0]);

	std::string	method = "REQUEST_METHOD=" + req.getMethod();
	std::string	content_length = "CONTENT_LENGTH=" + req.getOptions("content-length");
	std::string	content_type = "CONTENT_TYPE=" + req.getOptions("content-type");
	std::string	script_filename = "SCRIPT_FILENAME=" + filename;
	std::string	redirect = "REDIRECT_STATUS=200";
	std::string	target = "TARGET_DIR=" + this->dir;

	std::cerr << target << std::endl;

	char *envp[] = { 
		const_cast<char*>(method.c_str()),
		const_cast<char*>(content_length.c_str()),
		const_cast<char*>(content_type.c_str()),
		const_cast<char*>(script_filename.c_str()),
		const_cast<char*>(redirect.c_str()),
		const_cast<char*>(target.c_str()),
		NULL
	};

	std::cerr << "FILENAME:" << filename << std::endl;

	const char *args[] = {this->execCGI.c_str(), filename.c_str(), NULL};

	if (execve(this->execCGI.c_str(), const_cast<char* const*>(args), envp) == -1)
	{
		std::cerr << "Error: execve: " << strerror(errno) << std::endl;
		exit(127);
	}
}

void	Response::getReturnCGI( RequestClient req, int pipefd[2], int pipefd_stdin[2], int pid)
{
	int	status;

	close(pipefd[1]);
	close(pipefd_stdin[0]);

	if (!req.getMethod().compare("POST") && !this->bodyClient.empty())
		write(pipefd_stdin[1], this->bodyClient.c_str(), this->bodyClient.size());

	close(pipefd_stdin[1]);

	waitpid(pid, &status, 0);

	char cgi_output[4096]; // j ai lu que ct 4096 mais jsp si c reel
	int readd = read(pipefd[0], cgi_output, sizeof(cgi_output));
	if (readd > 0)
	{
		cgi_output[readd] = '\0';

		std::string output(cgi_output);
		while ((readd = read(pipefd[0], cgi_output, sizeof(cgi_output) - 1)) > 0) {
			cgi_output[readd] = '\0';
			output += cgi_output;
		}

		size_t		head_end = output.find("\n");
		std::string	body;
		if (head_end != std::string::npos)
		{
			std::string headers = output.substr(0, head_end);
			output.erase(0, headers.length());
			while (static_cast<int>(output[0]) == 10 || static_cast<int>(output[0]) == 13)
				output.erase(0, 1);
			body = output;
		}
		this->body = body + "\r\n";
	}
	else if (readd == -1)
		std::cerr << "Error: read: " << strerror(errno) << std::endl;

	if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
		std::cerr << "cgi execution failed" << std::endl;
	close(pipefd[0]);
}

void	Response::handleCGI(std::string filename, RequestClient req)
{
	int	pipefd[2];
	int	pid;
	int	pipefd_stdin[2];

	if (pipe(pipefd) == -1 || pipe(pipefd_stdin) == -1)
	{
		std::cerr << "Error: pipe: " << strerror(errno) << std::endl; 
		return ;
	}
	if ((pid = fork()) == -1)
	{
		std::cerr << "Error: fork: " << strerror(errno) << std::endl;
		close(pipefd[0]);
		close(pipefd[1]);
		close(pipefd_stdin[0]);
		close(pipefd_stdin[1]);
		return ;
	}
	if (pid == 0)
		this->toCGI(filename, req, pipefd, pipefd_stdin);
	if (pid > 0)
		this->getReturnCGI(req, pipefd, pipefd_stdin, pid);
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

void	Response::constructResponse(RequestClient req)
{
	if (this->body.find("Location:") != std::string::npos)
		this->code = "301";

	this->full = this->version + " " + this->code + " " + this->msg + "\r\n";

	if (req.getError() == 301 && this->body.find("Location:") == std::string::npos)
		this->full += "Location: " + req.getTarget() + "\r\n";

	this->full += "Content-Length: " + lengthBody() + "\r\n";
	if (!req.getCookie())
	{
		this->full += "Set-Cookie: session_id=" + SessionTokenCreate(32) + ";\r\n";
		this->addCookieValues();
	}

	if (req.getMethod().compare("HEAD"))
	{
		if (this->body.compare(0, 9, "Location:"))
			this->full += "\r\n";
		this->full += this->body;
	}
}
