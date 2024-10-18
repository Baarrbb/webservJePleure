
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

	std::cout << this->GetServer(y + 1)->GetHost(i).c_str() << this->GetServer(y + 1)->GetPort(i).c_str() << std::endl;
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

int	CgiHandling(char **envp, int clientsocket)
{
	int status;
	int pipefd[2];
	int pid;
	char **test = 0; 
	if (pipe(pipefd) == -1)
	{
		std::cerr << "pipe\n"; 
			return (-1);
	}
	if ((pid = fork()) == -1)
	{
		std::cerr << "fork" <<std::endl;
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], 1) == -1)
		{
			std::cerr << "dup2" << std::endl;
			exit(127);
		}
		close(pipefd[1]);
		if (execve("pathfilecgi", test, envp) == -1) //test = params_Cgi si on doit l implementer ( pas la pr le moment en tt cas)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			std::cerr << "execve" << std::endl;
			exit(127);
		}
	}
	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		close(pipefd[1]);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
			std::cerr << "cgi" << std::endl;
		char cgi_output[4096]; // j ai lu que ct 4096 mais jsp si c reel
		int readd = read(pipefd[0], cgi_output, sizeof(cgi_output));
		if (readd > 0)
		{
			cgi_output[readd] = '\0';
			send(clientsocket, cgi_output, readd, 0);
		}
		else if (readd == -1)
		{
			std::cerr << "read" << std::endl;
			return (-1);
		}
		close(pipefd[0]);
		return (0);
	}
	return (0);
}

int	Config::IsACgi()
{
	return (0);
}

int	Config::ServerStart(char **envp)
{
	int socketserverfd;
	struct sockaddr_storage their_addr;
	char s[INET6_ADDRSTRLEN];
	struct pollfd poll_fds[CONNECTIONS];
	int num_fds = 0;
	int num_srvs = 0;

	for (size_t y = 0; y < this->GetServer().size(); y++)
	{
		std::cout << "survive" << this->GetServer().size() << "!" << this->GetServer(y + 1)->GetHost().size() << std::endl;
		for (size_t i = 0; i < this->GetServer(1 + y)->GetHost().size(); i++)
		{
			std::cout << "for turn : " << num_fds << std::endl;
			if (this->GetServer(1 + y)->GetSocket(i) == true)
			{
				if ((socketserverfd = this->ServerCreate(i, y)) == -1)
					return (1);
				poll_fds[num_fds].fd = socketserverfd;
				poll_fds[num_fds].events = POLLIN;
				num_fds++;
			}
		}
	}
	std::cout <<"survive \n";
	num_srvs = num_fds;
	std::cout << num_fds << std::endl;
	while (1)
	{
		if (poll(poll_fds, num_fds, -1) == -1)
		{
			std::cerr << "poll" << std::endl;
			return 1;
		}
		for (int i = 0; i < num_fds; ++i)
		{
			if (poll_fds[i].revents & POLLIN)
			{
				if (i < num_srvs)
				{
					socklen_t their_size = sizeof(their_addr);
					// int new_fd = accept(socketserverfd, (struct sockaddr *)&their_addr, &their_size);
					int new_fd = accept(poll_fds[i].fd, (struct sockaddr *)&their_addr, &their_size);
					if (new_fd == -1) {
						std::cerr << "accept\n";
						continue;
					}
					inet_ntop(their_addr.ss_family, &(((struct sockaddr_in*)&their_addr)->sin_addr), s, sizeof s);
					std::cout << "server: got connection from " << std::string(s) << std::endl;

					poll_fds[num_fds].fd = new_fd;
					poll_fds[num_fds].events = POLLIN;
					num_fds++;
				}
				else
				{

					struct sockaddr_storage server_addr;
					socklen_t server_size = sizeof(server_addr);
					if (getsockname(poll_fds[i].fd, (struct sockaddr*)&server_addr, &server_size) == -1)
					{
						std::cerr << "sockname\n";
						return 1;
					}
					inet_ntop(server_addr.ss_family, &(((struct sockaddr_in*)&server_addr)->sin_addr), s, sizeof s);
					uint16_t port = ntohs(((struct sockaddr_in*)&server_addr)->sin_port);
					std::cout << "server is :" << s << " " << port << std::endl;

					this->processClientRequest(poll_fds[i].fd, std::string(s), port);
					(void)envp;
					close(poll_fds[i].fd);
					poll_fds[i] = poll_fds[num_fds - 1]; 
					num_fds--;
					// char buffer[1024];
					// int bytes_recv = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0); // rec header
					// if (bytes_recv <= 0)
					// {
					// 	if (bytes_recv == 0)
					// 		std::cout << "Client disconnected\n";
					// 	else
					// 	{
					// 		return (1);
					// 		std::cerr << "recv\n";
					// 	}
					// 	close(poll_fds[i].fd);
					// 	poll_fds[i] = poll_fds[num_fds - 1]; 
					// 	num_fds--;
					// }
					// else if (IsACgi()) //check si la reception est un cgi a faire mais j dois lire des trucs jsp cmt ca mrche
					// {
					// 	if (CgiHandling(envp, poll_fds[i].fd) == -1)
					// 	{
					// 		return (1);
					// 	}
					// 	close(poll_fds[i].fd);
					// 	poll_fds[i] = poll_fds[num_fds - 1]; 
					// 	num_fds--;
					// }
					// else
					// {
					// 	if (send(poll_fds[i].fd, buffer, bytes_recv, 0) == -1)// send header
					// 	{
					// 		std::cout << "send" << std::endl;
					// 		return (1);
					// 	}
					// 	close(poll_fds[i].fd);
					// 	poll_fds[i] = poll_fds[num_fds - 1]; 
					// 	num_fds--;
					// }
				}
			}
			if (global_variable == 127)
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
				return(0);
			}
		}
	}
	return (0);
}

void	Config::processClientRequest(int clientFd, std::string host, uint16_t port)
{
	int valread;
	char buffer[30000] = {0};
	std::string reqString;
	// bool keepAlive = false;
	while (1)
	{
		valread = recv(clientFd, buffer, 30000, 0);
		if (valread > 0)
		{
			std::string bufString(buffer, 30000);
			reqString.append(buffer, valread);
			// if (reqString.find_first_not_of("\r\n") == std::string::npos)
			// {
			// 	reqString.clear();
			// 	continue;
			// }

			if (reqString.find("\r\n\r\n") != std::string::npos
				|| reqString.find("\n\n") != std::string::npos)
			{
				RequestClient	req(reqString);
				Response	rep( req, this->_servers, host, port );
				std::string	response = rep.getFull();
				// if (response.find("keep-alive") != std::string::npos)
				// 	keepAlive = true;
				// else
				// 	keepAlive = false;

				// std::cout << response << std::endl;

				send(clientFd, response.c_str(), response.length(), 0);
				break ;

				// if (keepAlive)
				// 	reqString.clear();
				// else
				// 	break ;

			}
		}
	}
}
