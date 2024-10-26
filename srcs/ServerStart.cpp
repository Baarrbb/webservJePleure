
#include "Webserv.hpp"

#ifndef CONNECTIONS
# define CONNECTIONS 1024
#endif

int	Config::ServerCreate(int i, int y)
{
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int socketserverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketserverfd == -1)
	{
		std::cerr << "socket" << std::endl;
		return (-1);
	}

	// moi ajout
	int opt = 1;
	if (setsockopt(socketserverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cerr << "ERROR: setsockopt failed: " << strerror(errno) << std::endl;
		close(socketserverfd);
		return 1;
	}

	// std::cout << this->GetServer(y + 1)->GetHost(i).c_str() << this->GetServer(y + 1)->GetPort(i).c_str() << std::endl;
	if (getaddrinfo(this->GetServer(y + 1)->GetHost(i).c_str(), this->GetServer(y + 1)->GetPort(i).c_str(), &hints, &res) != 0)
	{
		std::cerr << "getaddrinfo" << std::endl;
		close(socketserverfd);
		return (-1);
	}
	if (fcntl(socketserverfd, F_SETFD, O_NONBLOCK | FD_CLOEXEC) == -1) //pas sur de ca pour le cloexec (pr cgi)
	{
		std::cerr << "fnctl" << std::endl;
		close(socketserverfd);
		freeaddrinfo(res);
		return (-1);
	}
	if (bind(socketserverfd, res->ai_addr, res->ai_addrlen) == -1)
	{
		std::cerr << "bind" << std::endl;
		close(socketserverfd);
		freeaddrinfo(res);
		return (-1);
	}
	if (listen(socketserverfd, CONNECTIONS) == -1)
	{
		std::cerr << "listen" << std::endl;
		close(socketserverfd);
		freeaddrinfo(res);
		return (-1);
	}
	freeaddrinfo(res);
	return (socketserverfd);
}

int Config::FillPollFd(struct pollfd *poll_fds, int *num_fds, std::map<int, std::string> &state)
{
	int socketserverfd;

	for (size_t y = 0; y < this->GetServer().size(); y++)
	{
		// std::cout << "survive" << this->GetServer().size() << "!" << this->GetServer(y + 1)->GetHost().size() << std::endl;
		for (size_t i = 0; i < this->GetServer(1 + y)->GetHost().size(); i++)
		{
			// std::cout << "for turn : " << num_fds << std::endl;
			if (this->GetServer(1 + y)->GetSocket(i) == true)
			{
				if ((socketserverfd = this->ServerCreate(i, y)) == -1)
					return (1);
				state[socketserverfd] = "Server";
				poll_fds[(*num_fds)].fd = socketserverfd;
				poll_fds[(*num_fds)].events = POLLIN;
				(*num_fds)++;
			}
		}
	}
	return (0);
}

static void	closepoll(int num_fds, struct pollfd *poll_fds)
{
	for (int i = 0; i < num_fds; ++i)
	{
		if (poll_fds[i].fd != -1)
		{
			close(poll_fds[i].fd);
			poll_fds[i].fd = -1;
		}
	}
	num_fds = 0;
}

static int GetClientInfos(struct pollfd *poll_fds, int i, char *s, uint16_t *port)
{
	struct sockaddr_storage server_addr;
	socklen_t server_size = sizeof(server_addr);

	if (getsockname(poll_fds[i].fd, (struct sockaddr*)&server_addr, &server_size) == -1)
	{
		std::cerr << "sockname\n";
		return 1;
	}
	inet_ntop(server_addr.ss_family, &(((struct sockaddr_in*)&server_addr)->sin_addr), s, INET_ADDRSTRLEN);
	*port = ntohs(((struct sockaddr_in*)&server_addr)->sin_port);
	return (0);
}

static int NewClientConnexion(struct pollfd *poll_fds, int i, int *num_fds, std::map<int, std::string> &state)
{
	struct sockaddr_storage their_addr;
	socklen_t their_size = sizeof(their_addr);
	char s[INET6_ADDRSTRLEN];
	int new_fd = accept(poll_fds[i].fd, (struct sockaddr *)&their_addr, &their_size);
	if (new_fd == -1)
	{
		std::cerr << "accept\n";
		return 1;
	}
	inet_ntop(their_addr.ss_family, &(((struct sockaddr_in*)&their_addr)->sin_addr), s, sizeof s);
	// std::cout << "server: got connection from " << std::string(s) << std::endl;
	state[poll_fds[i].fd] = "Client";
	poll_fds[(*num_fds)].fd = new_fd;
	poll_fds[(*num_fds)].events = POLLIN;
	(*num_fds)++;
	return 0;
}

static int IsACgiFile(std::map<int, std::string> state, int fd)
{
	std::map<int, std::string>::iterator it = state.find(fd);
	if (it != state.end() && it->second == "Cgi")
		return (1);
	return (0);
}

int	Config::ServerStart(char **envp)
{
	char s[INET6_ADDRSTRLEN];
	int num_srvs = 0;
	struct pollfd poll_fds[CONNECTIONS];
	int num_fds = 0;
	std::map<int, std::string> state;
	s_updatepoll test(state, &num_fds, poll_fds);

	if (FillPollFd(poll_fds, &num_fds, state) == 1)
		return (1);
	// std::cout <<"survive \n";
	num_srvs = num_fds;
	// std::cout << num_fds << std::endl;
	while (1)
	{
		if (poll(poll_fds, num_fds, -1) == -1)
		{
			if (global_variable == 127)
			{
				closepoll(num_fds, poll_fds);
				return(0);
			}
			std::cerr << "poll crash" << std::endl;
			return (1);
		}
		for (int i = 0; i < num_fds; ++i)
		{
			if (poll_fds[i].revents & POLLIN)
			{
				if (i < num_srvs)
				{
					if (NewClientConnexion(poll_fds, i, &num_fds, state) == 1)
						return 1;
				}
				else
				{
					uint16_t	port;
					(void)envp;
					/*if (IsACgiFile(state, poll_fds[i].fd))
					{
						char		cgi_output[4096];
						int			rd;
						std::string	output;
						while ((rd = read(poll_fds[i].fd, cgi_output, sizeof(cgi_output) - 1)) > 0)
						{
							if (rd > 0)
							{
								cgi_output[rd] = '\0';
								output += cgi_output;
							}
							end = std::time(NULL);
							if (difftime(end, start) > 10)
								throw RequestClient::ErrorRequest(504, "./not_found/504.html", "Gateway Timeout");
						}
						if (rd == -1)
						{
							std::cerr << "Error: read: " << strerror(errno) << std::endl;
							return ;
						}
					}
					else
					{*/
					if (GetClientInfos(poll_fds, i, s, &port) == 1)
						return (1);
					std::cout << "server is :" << s << " " << port << std::endl;
					this->processClientRequest(poll_fds[i].fd, std::string(s), port, state, poll_fds);
					//}
					poll_fds[i].revents = POLLOUT;
				}
			}
			else if (poll_fds[i].revents & POLLOUT)
			{
			/*	if (IsACgiFile(state, poll_fds[i].fd))
				{
					this->bodyClient;
				}
				else
				{
					Response	rep(req, this->bodyClient, this->_servers, host, port);
					std::string	response = rep.getFull();*/
				//}
				this->responseClient(poll_fds[i].fd, this->response);
				state.erase(poll_fds[i].fd);
				close(poll_fds[i].fd);
				poll_fds[i] = poll_fds[num_fds - 1]; 
				num_fds--;
			}
		}
	}
	return (0);
}

void	Config::processClientRequest(int clientFd, std::string host, uint16_t port, s_updatepoll& poll_data)
{
	int				rd;
	char			buffer[8192] = {0};
	std::string		reqString;
	bool			first = true;
	RequestClient	req;

	while (1)
	{
		rd = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
		if (rd == -1)
		{
			std::cerr << "Error: recv: " << strerror(errno) << std::endl;
			return ;
		}
		reqString.append(buffer, rd);
		size_t	endHeaders = reqString.find("\r\n\r\n");
		if (endHeaders != std::string::npos)
		{
			std::string		headers = reqString.substr(0, endHeaders);
			this->bodyClient = reqString.substr(endHeaders + 4, reqString.length());
			if (first)
			{
				RequestClient getFirst(headers);
				req = getFirst;
				first = false;
			}
			if (!req.getMethod().compare("POST"))
			{
				try
				{
					if (req.getOptions("content-length").compare("") && req.getOptions("transfer-encoding").compare(""))
						throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
					if (req.getOptions("content-length").compare(""))
					{
						if (static_cast<long>(this->bodyClient.length()) < strtol(req.getOptions("content-length").c_str(), 0, 10))
						{
							this->bodyClient.append(buffer, rd);
							continue;
						}
					}
					if (req.getOptions("transfer-encoding").compare(""))
					{
						std::string val = req.getOptions("transfer-encoding");
						for(size_t i = 0; i < val.length(); i++)
							val[i] = std::tolower(val[i]);
						if (val.compare("chunked"))
							throw RequestClient::ErrorRequest(501, "not_found/501.html", "Not Implemented");
						if (this->bodyClient.find("\r\n0\r\n") == std::string::npos)
						{
							this->bodyClient.append(buffer, rd);
							continue;
						}
						this->bodyClient = this->processChunkedBody(this->bodyClient);
					}
				}
				catch(RequestClient::ErrorRequest& e)
				{
					req.setMethod("GET");
					req.setError(e.getError());
					req.setPath("/");
					req.setTarget(e.getTarget());
					req.setMsgError(e.getMsg());
				}
			}
			// if (body.find("\r\n") != std::string::npos)
			// 	body = body.substr(0, body.find("\r\n"));
			// if (send(clientFd, response.c_str(), response.length(), 0) == -1)
			// {
			// 	std::cerr << "Error: send: " << strerror(errno) << std::endl;
			// 	return ;
			// }
			Response	rep( req, this->bodyClient, this->_servers, host, port, poll_data);
			this->response = rep.getFull();
			//this->responseClient(clientFd, response);
			break ;
		}
	}
}

void	Config::responseClient(int clientFd, std::string response)
{
	if (send(clientFd, response.c_str(), response.length(), 0) == -1)
	{
		std::cerr << "Error: send: " << strerror(errno) << std::endl;
		return ;
	}
}

std::string	Config::processChunkedBody( std::string &body )
{
	std::string	tmp = body;
	std::string	bodyUnchunk;
	long		sizeData;
	long		realSize;
	char		*end;

	// std::cout << "PROCESSCHUNKED:" << tmp << "." << std::endl;

	while (!tmp.empty())
	{
		size_t endLine = tmp.find("\r\n");
		std::string lineSize = tmp.substr(0, endLine);
		// std::cout << "LINESIZE:" << lineSize << std::endl;
		sizeData = strtol(lineSize.c_str(), &end, 16);
		// std::cout << "sizeData:" << sizeData << std::endl;
		if (end == lineSize.c_str() || errno == EINVAL)
			throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
		if (sizeData == 0)
			return bodyUnchunk;
		tmp.erase(0, endLine + 2);
		endLine = tmp.find("\r\n");
		std::string data = tmp.substr(0, endLine);
		realSize = data.length();
		// std::cout << "realSize:" << realSize << std::endl;
		// std::cout << "LINE:" << data << std::endl;
		if (sizeData != realSize)
			throw RequestClient::ErrorRequest(400, "not_found/400.html", "Bad Request");
		bodyUnchunk += data;
		tmp.erase(0, endLine + 2);
	}
	return bodyUnchunk;
}