
#include "Webserv.hpp"

int global_variable = 0;

static void 	my_sig(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received.\n";
	// global_variable = 127;
	// return ;
	exit(127);
}

int	main(int argc, char **argv, char **envp)
{
	// if (argc != 2)
	// {
	// 	std::cout << "Please insert a config file there." << std::endl;
	// 	std::cout << "./webserv [configuration file]" << std::endl;
	// 	return 0;
	// }
	// (void)argv;
	// std::string file_name = argv[1];
	// Config test(file_name);
	// if (test.ParseGlobal() == 1)
	// 	return (1);
	// std::cout << test << std::endl;
	// if (test.ServerStart(envp) == 1)
	// 	return (1);
	// return (0);

	// if (argc == 2)
	// {
	// 	std::string file_name = argv[1];
	// 	Config conf(file_name);
	// 	if (conf.ParseGlobal() == 1)
	// 		return (1);
	// 	// std::cout << conf << std::endl;
	// 	if (conf.ServerStart(envp) == 1)
	// 		return (1);
	// }
	// else if (argc == 1)
	// {
	// 	Config conf("./conf/default.conf");
	// 	if (conf.ParseGlobal() == 1)
	// 		return (1);
	// 	// std::cout << conf << std::endl;
	// 	signal(SIGINT, my_sig);
	// 	if (conf.ServerStart(envp) == 1)
	// 		return (1);
	// }
	// else
	// {
	// 	std::cout << "Usage:" << std::endl;
	// 	std::cout << "./webserv [configuration file]" << std::endl;
	// 	return 0;
	// }

	Config		conf;
	std::string	file_name;
	if (argc == 2)
		file_name = argv[1];
	else if (argc == 1)
		file_name = "./conf/default.conf";
	else
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "./webserv [configuration file]" << std::endl;
		return 0;
	}
	conf.SetFilename(file_name);
	if (conf.ParseGlobal() == 1)
		return (1);
	signal(SIGINT, my_sig);
	if (conf.ServerStart(envp) == 1)
		return (1);

	return (0);
}

