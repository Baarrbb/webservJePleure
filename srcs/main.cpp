
#include "Webserv.hpp"

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

	if (argc == 2)
	{
		std::string file_name = argv[1];
		Config test(file_name);
		if (test.ParseGlobal() == 1)
			return (1);
		// std::cout << test << std::endl;
		if (test.ServerStart(envp) == 1)
			return (1);
	}
	else if (argc == 1)
	{
		Config test("./conf/default.conf");
		if (test.ParseGlobal() == 1)
			return (1);
		std::cout << "PPPPPPPPPPPPPPPPP" << std::endl;
		std::cout << std::endl;
		std::cout << test << std::endl;
		if (test.ServerStart(envp) == 1)
			return (1);
	}
	else
	{
		std::cout << "PB" << std::endl;
		std::cout << "./webserv [configuration file]" << std::endl;
		return 0;
	}
	
	return (0);
}

