/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestClient.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:41:14 by marvin            #+#    #+#             */
/*   Updated: 2024/10/19 00:48:19 by ersees           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTCLIENT_HPP
#define REQUESTCLIENT_HPP

#include "Webserv.hpp"

class RequestClient
{
	public:
		RequestClient(std::string &);
		~RequestClient( void );

		int									getError( void );
		std::string							getMsgError( void );
		std::string							getMethod( void );
		std::string							getTarget( void );
		std::string							getHost( void );
		std::string							getOptions( std::string key );
		std::map<std::string, std::string>	getOptions( void );
		
		bool								getCookie() const;
		void								setCookie( bool );
		void								takeCookie();


		void		setTarget( std::string );
		void		setError( int );
		void		setMsgError( std::string );

	private:
		RequestClient( void );
		void	badSyntax( std::string line );

		int		checkIfHost( void );
		void	addMethod( std::string );
		void	addHost( void );
		int		addTarget( std::string );
		void	addOptions( std::string );

		void	othersOptions( std::string );

		int									error;
		std::string							msgError;
		std::string							method;
		std::string							target;
		std::string							host;
		std::string							query; // jsp si je prends en charge :'(
		std::map<std::string, std::string>	options;
		bool								cookie;

	public:
		class ErrorRequest : public std::exception
		{
			public:
				ErrorRequest(int code, std::string file, std::string msg)
				{
					this->code = code;
					this->filename = file;
					this->msg = msg;
				}
				virtual ~ErrorRequest ( void ) throw() {}

				int	getError( void )
				{
					return this->code;
				}

				std::string	getTarget( void )
				{
					return this->filename;
				}

				std::string	getMsg( void )
				{
					return this->msg;
				}

			private:
				int			code;
				std::string	filename;
				std::string	msg;
		};
};

#endif