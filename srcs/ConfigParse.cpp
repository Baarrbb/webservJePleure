
#include "Webserv.hpp"

static int	NotaSpaceOrChara(std::string line, char c, int i)
{
	while(line[i] && (line[i] == ' ' || line[i] == c))
		i++;

	if (line[i])
		return 1;
	return 0;
}

int Config::ServerNameByServer()
{
	// std::cout << "endl" << std::endl;
	for (size_t y = 0; y < this->GetServer().size(); ++y)
	{
		std::vector<std::string> first = this->GetServer(y + 1)->GetServerName();
		for (size_t j = y + 1; j < this->GetServer().size(); ++j)
		{
			std::vector<std::string> second = this->GetServer(j + 1)->GetServerName();
			std::cout << "ouais!!"<< this->GetServer().size() << " " << first.size() << second.size() << std::endl;
			for (std::vector<std::string>::iterator it = first.begin(); it < first.end(); it++)
			{
				for (std::vector<std::string>::iterator itt = second.begin(); itt < second.end(); itt++)
				{
					// std::cout << "endl" << *it << *itt << std::endl;
					if ((*it).compare(*itt) == 0)
					{
						std::cerr << "Error: Server name \"" << *it 
								<< "\" is used in both server " << y
								<< " and server " << j << std::endl;
						return (1);
					}
				}
			}
		}
	}
	return (0);
}

int	Config::ParseGlobal()
{
	std::string line;
	std::ifstream file(this->_filename.c_str());
	int	i = 1;

	if (file.is_open())
	{

		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '\n') continue;
			if (std::string::npos != line.find("server", 0, 6))
			{
				if (NotaSpaceOrChara(line, '{', 6) == 1)
					return (std::cout << "peut etre mettre les mess derreur iciyy\n", 1);
				if (std::string::npos != line.find("{"))
				{
					this->AddServer();
					this->GetServer(i)->ServerParse(file);
					i++;
					std::cout << "begin server thing ig but idk if its a good idea\n";
				}
				else
				{
					while (std::getline(file, line) && NotaSpaceOrChara(line, '\n', 0) == 0){}
					if (NotaSpaceOrChara(line, '{', 0) == 1)
						return (std::cout << "peut etre mettre les mess derreur ici\n", 1);
					else
					{
						this->AddServer();
						this->GetServer(i)->ServerParse(file);
						i++;
					}
				}
			}
		}
	}
	else
	{
		std::cout << "mauvais file" << std::endl;
		return (1);
	}
	int lol = ServerNameByServer();
	if (lol)
	{
		std::cerr << "invalid parameter server_name in xxxx" << std::endl;
		return (1);
	}
	// std::cout << "different servername" << std::endl;
	for (size_t y = 0; y < this->GetServer().size(); y++)
	{
		for (size_t i = 0; i < this->GetServer(y + 1)->GetHost().size(); i++)
		{
			std::string currenthost = this->GetServer(y + 1)->GetHost(i);
			std::string currentport = this->GetServer(y + 1)->GetPort(i);
			std::vector<std::string> currentservname = this->GetServer(y + 1)->GetServerName();
			for (size_t j = y; j < this->GetServer().size(); j++)
			{
				// for (size_t k = i + 1; k < this->GetServer(j + 1)->GetHost().size(); k++)
				for (size_t k = i; k < this->GetServer(j + 1)->GetHost().size(); k++)
				{
					if (k != i || j != y)
					{
						std::string newhost = this->GetServer(j + 1)->GetHost(k);
						std::string newport = this->GetServer(j + 1)->GetPort(k);
						if ((currentport.compare(newport) == 0 && currenthost.compare(newhost) == 0))
						{
							// std::cout << "different servernamedd" << std::endl;
							this->GetServer(j + 1)->AddSocket(false, k);
							// std::cout << "different servernamedd" << std::endl;
							//yipi = true;
						}
					}
				}
			}
		}
	}

	return (0);
}

/*if (server && {)
servpart {
	switch
		fct server_name 
		fct listen
		fct root
		fct index
		fct	client_body_buffer_size
		fct allow_methods POST/GET
		fct location
			location part {
			switch
			allow_methods POST/GET
			root
			alias???
			client_body_buffer_size
			index
			cgi_pass
			location start part {

			end location part }
			end location part }
end servpart }
check si le file / directory exist
*/
