/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:42:39 by marvin            #+#    #+#             */
/*   Updated: 2024/10/24 18:57:48 by marvin           ###   ########.fr       */
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

#include "Server.hpp"
#include "Config.hpp"
#include "ALocationheredite.hpp"
#include "RequestClient.hpp"
#include "Response.hpp"
#include "CGI.hpp"

#endif