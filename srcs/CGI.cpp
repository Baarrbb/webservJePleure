/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:39:02 by marvin            #+#    #+#             */
/*   Updated: 2024/10/25 18:49:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

CGI::CGI(RequestClient req, Location serv, std::string bodyClient, std::string scriptPath, std::string pathInfo, std::string dir)
	: bodyClient(bodyClient), scriptPath(scriptPath), pathInfo(pathInfo), dir(dir)
{
	this->cgiPass = serv.GetCgiPass();
	this->method = req.getMethod();

	this->addEnvp(req);
	this->handleCGI(scriptPath, req);
}

CGI::~CGI( void )
{}


void	CGI::addEnvp(RequestClient req)
{
	if (req.getOptions("content-length").compare(""))
		this->env.push_back("CONTENT_LENGTH=" + req.getOptions("content-length"));
	else
	{
		std::stringstream len;
		len << this->bodyClient.length();
		this->env.push_back("CONTENT_LENGTH=" + len.str());
	}

	this->env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	this->env.push_back("CONTENT_TYPE=" + req.getOptions("content-type"));
	this->env.push_back("PATH_INFO=" + this->pathInfo);
	this->env.push_back("QUERY_STRING=" + req.getQuery());
	this->env.push_back("REQUEST_METHOD=" + req.getMethod());
	this->env.push_back("SCRIPT_FILENAME=" + this->scriptPath);
	this->env.push_back("REDIRECT_STATUS=200");
	this->env.push_back("TARGET_DIR=" + this->dir);
}

// GETTERS

std::string	CGI::getBodyResponse( void ) const
{
	return this->bodyResponse;
}

std::string	CGI::getHeadResponse( void ) const
{
	return this->headResponse;
}

void	CGI::toCGI( std::string filename, RequestClient req, int pipefd[2], int pipefd_stdin[2])
{
	(void)req;
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

	char *envp[] = { 
		const_cast<char*>((this->env[0]).c_str()),
		const_cast<char*>((this->env[1]).c_str()),
		const_cast<char*>((this->env[2]).c_str()),
		const_cast<char*>((this->env[3]).c_str()),
		const_cast<char*>((this->env[4]).c_str()),
		const_cast<char*>((this->env[5]).c_str()),
		const_cast<char*>((this->env[6]).c_str()),
		const_cast<char*>((this->env[7]).c_str()),
		const_cast<char*>((this->env[8]).c_str()),
		NULL
	};

	const char *args[] = {this->cgiPass.c_str(), filename.c_str(), NULL};

	if (execve(this->cgiPass.c_str(), const_cast<char* const*>(args), envp) == -1)
	{
		std::cerr << "Error: execve: " << strerror(errno) << std::endl;
		//ICi je throw 500 ??
		exit(127);
	}
}

void	CGI::getReturnCGI( RequestClient req, int pipefd[2], int pipefd_stdin[2], int pid)
{
	int			status;
	char		cgi_output[4096];
	std::string	output;

	close(pipefd[1]);
	close(pipefd_stdin[0]);

	if (!req.getMethod().compare("POST") && !this->bodyClient.empty())
	{
		if (write(pipefd_stdin[1], this->bodyClient.c_str(), this->bodyClient.size()) < 0)
		{
			std::cerr << "Error: write: " << strerror(errno) << std::endl;
			return ;
		}
	}
	close(pipefd_stdin[1]);

	std::time_t start = std::time(NULL);
	std::time_t end;
	int rd;
	while ((rd = read(pipefd[0], cgi_output, sizeof(cgi_output) - 1)) > 0)
	{
		if (rd > 0)
		{
			cgi_output[rd] = '\0';
			output += cgi_output;
		}
		end = std::time(NULL);
		if (difftime(end, start) > 10)
		{
			kill(pid, SIGKILL);
			throw RequestClient::ErrorRequest(504, "./not_found/504.html", "Gateway Timeout");
		}
	}
	if (rd == -1)
	{
		std::cerr << "Error: read: " << strerror(errno) << std::endl;
		return ;
	}

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
		std::cerr << "cgi execution failed" << std::endl;

	size_t		head_end = output.find("\r\n\r\n");
	std::string	body;
	if (head_end != std::string::npos)
	{
		this->headResponse = output.substr(0, head_end);
		output.erase(0, this->headResponse.length());
		while (static_cast<int>(output[0]) == 10 || static_cast<int>(output[0]) == 13)
			output.erase(0, 1);
		body = output;
	}
	this->bodyResponse = body + "\r\n";

	close(pipefd[0]);
}

void	CGI::handleCGI(std::string filename, RequestClient req)
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
