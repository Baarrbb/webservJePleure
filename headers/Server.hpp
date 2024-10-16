#ifndef SERVER_HPP
#define SERVER_HPP

//#include "Location.hpp"
class Location;

#include "Webserv.hpp"


enum Directive {
	SERVER_NAME,
	LISTEN,
	ROOT,
	INDEX,
	ALLOWMETHODS,
	LOCATION,
	ALIAS,
	CGI_PASS,
	CLIENT_BODY_SIZE,
	END,
	HOST,
	DEFAULT_ERROR_PAGE,
	NUM_DIRECTIVES
};

class Server
{
	public:
		Server();
		//Server(std::string filename);
		~Server();
		Server(const Server &copy);
		Server & operator = (const Server &copy);
		int						InitLocation(std::istringstream &iss, std::ifstream &file);
		//int						fonction(std::istringstream &iss, std::string directive, std::map<std::string, std::string>& option);
		int						ProcessDirective(const std::string& line, std::ifstream &file);
		int						ServerParse(std::ifstream &file);
		void					AddLocation(Location *location); // rajoute un simpel location aux locations
		void					AddLocation(std::string path_name);
		//int						addtoport(std::istringstream &iss, std::string directive, std::map<std::string, std::string>& option);
		int						hostportadd(std::istringstream &iss);
		void					ValidateNginxConfig();
		void					AddSocket(bool info, int index);
		
		//setter
		void					SetLocation(std::vector<Location*> location); // set un vecteur en entier
		void					SetServerName(const std::string& value);
		void					SetPort(const std::string& value);
		void					SetHost(const std::string& value);
		void					SetRoot(const std::string& value);
		void					SetIndex(const std::string& value);
		void					SetErrorPage(const std::string& value);
		void					SetAllowMethods(const std::string& value);
		void					SetAlias(const std::string& value);
		void					SetCgiPass(const std::string& value);
		void					SetClientBodyBufferSize(const std::string& value);
		void					SetSocket(bool info);

		// Get methods
		std::vector<Location*>	GetLocation() const;// sans index ca donne le vector en entier
		Location*				&GetLocation(unsigned int index); // avec index ca donne un serveur en particulier
		std::vector<std::string>			GetServerName() const;
		std::vector<std::string>					GetPort() const;
		std::vector<std::string>			GetHost() const;
		std::vector<std::string>			GetIndex() const;
		std::vector<std::string>			GetAllowMethods() const;
		std::string				GetServerName(const int& index) const;
		std::string						GetPort(const int& index) const;
		std::string				GetHost(const int& index) const;
		std::string				GetRoot() const;
		std::string				GetIndex(const int& index) const;
		std::string				GetErrorPage() const;
		std::string				GetAllowMethods(const int& index) const;
		std::string				GetAlias() const;
		std::string				GetCgiPass() const;
		std::string				GetClientBodyBufferSize() const;
		bool					GetSocket(const int &index) const;
		
		// Write methods
		void					WriteServerName() const;
		void					WritePort() const;
		void					WriteHost() const;
		void					WriteRoot() const;
		void					WriteIndex() const;
		void					WriteErrorPage() const;
		void					WriteAllowMethods() const;
		void					WriteAlias() const;
		void					WriteCgiPass() const;
		void					WriteClientBodyBufferSize() const;
		void					WriteLocation();
		void					WriteSocket() const;
		
		//exceptions
		class WrongExpression: public std::exception {
			public:
				virtual const char* what(void) const throw();
		};

		class errors : public std::exception {
			public:
				explicit errors(const std::string& message);
				virtual const char* what() const throw();
				virtual ~errors() throw();
			private:
				std::string _msg;
			};

		// template get/seter
		int	Gettest() const;
		void Settest(int test);

	protected:
		std::vector<Location*>		_locations;
		std::vector<std::string>	server_name;
		std::vector<std::string>	port;
		std::vector<std::string>	host;
		std::string					root;
		std::vector<std::string>	index;
		std::string					error_page;
		std::vector<std::string>	allow_methods;
		std::string					alias;
		std::string					cgi_pass;
		std::string					client_body_buffer_size;
		int							test;
		std::vector<bool>			yipi;

		bool								IsValidServerName(const std::string& name);
		bool								IsValidListen();
		bool								IsValidRoot(const std::string& root);
		bool								IsValidIndex(const std::string& index);
		bool								IsValidHost(const std::string& host);
		bool								IsValidDefaultErrorPage(const std::string& path);
		bool								IsValidClientBodySize(const std::string& size);
		bool								IsValidAllowMethods(const std::string& methods);
		bool								IsValidAlias(const std::string& alias);
		bool								IsValidCgiPass(const std::string& cgi_pass);
};

std::ostream	&operator<<(std::ostream &out, Server &to_write); // rajouter le const apr tt les tests

#endif