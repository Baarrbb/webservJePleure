/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:36:45 by marvin            #+#    #+#             */
/*   Updated: 2024/10/20 01:16:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP


#include "Webserv.hpp"

class RequestClient;
class Server;

class Response
{
	public:
		// Response( RequestClient, std::vector<Server*>, std::string, uint16_t );
		Response( RequestClient, std::string, std::vector<Server*>, std::string, uint16_t );
		~Response( void );

		std::string	getFull( void );

	private:
		std::string	version;
		std::string	code;
		std::string	msg;
		std::string	body;
		std::string	full;
		std::string	file;
		std::string	execCGI;
		bool		isCGI;
		std::string	bodyClient;
		std::string	dir;

		Server		findConfig( std::vector<Server*>, std::string, uint16_t, std::string );
		Location	findLocation( Server, std::string );
		void		toCGI( std::string, RequestClient, int fd[2], int fds[2]);
		void		getReturnCGI( RequestClient, int fd[2], int fds[2], int );
		void		handleCGI(std::string, RequestClient);
		void		checkMethodsAllowed( Location, std::string );
		std::string	findFile( Location, std::string, int );
		void		fillLoc(Server serv, Location* loc);
		void		addBody( std::string, RequestClient );
		std::string	lengthBody( void );
		void		constructResponse( RequestClient );
		void		addCookieValues( void );
};

#endif