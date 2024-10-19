#ifndef ALOCATIONHEREDITE_HPP
#define ALOCATIONHEREDITE_HPP

// #include "Server.hpp"
#include "Webserv.hpp"

class Location : public Server
{
	public:
		Location();
		Location(std::string filename);
		~Location();
		Location(const Location &copy);
		Location & operator = (const Location &copy);
		void	WriteFilePath() const;

		std::string	getPathLoc( void );

	private:
		std::string _file_path;
};

std::ostream	&operator<<(std::ostream &out, Location &to_write); // rajouter le const apr tt les tests


#endif