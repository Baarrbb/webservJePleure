/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:42:39 by marvin            #+#    #+#             */
/*   Updated: 2024/10/23 12:33:06 by ersees           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

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