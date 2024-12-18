/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:36:45 by marvin            #+#    #+#             */
/*   Updated: 2024/10/26 02:02:11 by ersees           ###   ########.fr       */
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
		Response( RequestClient, std::string, std::vector<Server*>, std::string, uint16_t , s_updatepoll&);
		~Response( void );

		std::string	getFull( void );

	private:
		std::string	version;
		std::string	code;
		std::string	msg;
		std::string	body;
		std::string	cgiBody;
		std::string	cgiHeaders;
		std::string	full;
		std::string	file;
		bool		isCGI;
		std::string	bodyClient;
		std::string	dir;
		std::string	pathInfo;
		std::string	ext;

		Server		findConfig( std::vector<Server*>, std::string, uint16_t, std::string );
		Location	findLocation( Server, std::string );
		void		fillLoc(Server serv, Location* loc);
		Location*	findLocationInLocation( std::vector<Location*>, std::string );
		void		checkMethodsAllowed( Location, std::string );
		std::string	findFile( Location, std::string, int );
		void		addBody( std::string, RequestClient, Location );
		std::string	lengthBody( void );
		void		constructResponse( RequestClient );
		void		checkCGI( Server , std::string );
		std::string	extractPathInfo( std::string );
		void		addCookieValues( void );
		void		checkLimitBodySize( RequestClient, Location, size_t );
		void		checkCGIHeadersStatusCode( void );
};

#endif
