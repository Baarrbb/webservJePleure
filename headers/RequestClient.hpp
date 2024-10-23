/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestClient.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:41:14 by marvin            #+#    #+#             */
/*   Updated: 2024/10/22 00:19:13 by marvin           ###   ########.fr       */
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

		int									getError( void ) const;
		std::string							getMsgError( void ) const;
		std::string							getMethod( void ) const;
		std::string							getTarget( void ) const;
		std::string							getHost( void ) const;
		std::string							getOptions( std::string ) const;
		std::map<std::string, std::string>	getOptions( void ) const;
		bool								getCookie( void ) const;
		std::string							getPath( void ) const;
		std::string							getQuery( void ) const;

		void		setTarget( std::string );
		void		setError( int );
		void		setMsgError( std::string );

	private:
		void	badSyntax( std::string line );

		int		checkIfHost( void );
		void	addMethod( std::string );
		void	addHost( void );
		int		addTarget( std::string );
		void	addQuery( std::string );
		void	addOptions( std::string );

		void	othersOptions( std::string );

		int									error;
		std::string							msgError;
		std::string							method;
		std::string							target;
		std::string							host;
		std::string							path;
		std::string							query;
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