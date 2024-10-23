/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:38:40 by marvin            #+#    #+#             */
/*   Updated: 2024/10/21 20:46:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "Webserv.hpp"

class CGI
{
	public:
		CGI( RequestClient, Location, std::string, std::string, std::string, std::string );
		~CGI( void );

		std::string	getBodyResponse( void ) const;
		std::string	getHeadResponse( void ) const;

	private:
		std::string	bodyClient;
		std::string	scriptPath;
		std::string	pathInfo;
		std::string	dir;

		std::string	headResponse;
		std::string	bodyResponse;

		std::string	cgiPass;
		std::string	method;

		void	addPathInfo( Location );
		void	addEnvp( RequestClient );
		void	toCGI( std::string, RequestClient, int fd[2], int fds[2]);
		void	getReturnCGI( RequestClient, int fd[2], int fds[2], int );
		void	handleCGI(std::string, RequestClient);

		std::vector<std::string>	env;
	
};

#endif