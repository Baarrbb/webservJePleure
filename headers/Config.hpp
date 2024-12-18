#ifndef CONFIG_HPP
#define CONFIG_HPP

// #include "Server.hpp"

#include "Webserv.hpp"

class Config
{
	public:
		Config();
		Config(std::string filename);
		virtual ~Config();
		Config(const Config &copy);
		Config & operator = (const Config &copy);

		int	ParseGlobal();
		int	ServerCreate(int i, int y);
		int	ServerStart(char **envp);
		int	IsACgi();

		int					FillPollFd(struct pollfd *poll_fds, int *num_fds, std::map<int, std::string> &state);
		int					ServerNameByServer();
		void				AddServer(Server *server); // rajoute un simpel server aux servers
		void				AddServer();
		void				PrintAllServers();

		//setter / getters
		std::string			GetFilename() const;
		void				SetFilename(std::string filename);
		std::vector<Server*>	&GetServer();// sans index ca donne le vector en entier
		Server*				&GetServer(unsigned int index); // avec index ca donne un serveur en particulier
		void				SetServer(std::vector<Server*> server);
		//exceptions
		class WrongExpression: public std::exception {
			public:
				virtual const char* what(void) const throw();
		};

		// template get/seter
		int	Gettest() const;
		void Settest(int test);

	private:
		int						test;
		std::string				_filename;
		std::vector<Server*>	_servers;
		std::string				bodyClient;
		std::string				response;
		void				processClientRequest( int, std::string, uint16_t , s_updatepoll&);
		std::string			processChunkedBody( std::string& );
		void				responseClient( int , std::string );
};

std::ostream	&operator<<(std::ostream &out, Config &to_write); //rajouter le const apr les tests

#endif