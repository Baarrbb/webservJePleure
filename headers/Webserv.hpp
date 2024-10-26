/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:42:39 by marvin            #+#    #+#             */
/*   Updated: 2024/10/26 02:07:05 by ersees           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

# define JAUNE "\x1B[33m"
# define RESET "\033[0m"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <exception>
#include <cstring>
#include <iterator>
#include <cstdio>
#include <ctime>
#include <climits>

#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>

struct s_updatepoll {
	std::map<int, std::string>& state;
	int* num_fds;
	struct pollfd (&poll_fds)[1024];
	s_updatepoll(std::map<int, std::string>& st, int* nfds, struct pollfd (&p_fds)[1024])
		: state(st), num_fds(nfds), poll_fds(p_fds) {}
};


#include "Server.hpp"
#include "Config.hpp"
#include "ALocationheredite.hpp"
#include "RequestClient.hpp"
#include "Response.hpp"
#include "CGI.hpp"

#endif