#include <iostream>
#define BOOST_THREAD_DYN_LINK
#include "filesystem_utilities.h"
#include "context_global.h"

void exit_command()
{
	char q = 0;
	std::cout << "输入q退出" << std::endl;
	std::cin >> q;
	while (q != 'q' && q != 'Q')
	{
		std::cout << "输入q退出" << std::endl;
		std::cin >> q;
	}
}

int main()
{
	std::string config_path("data");
	auto dir = utilities::get_directorty_path(config_path);
	if (dir)
	{
		screwdriver::create_rtr_tm_client("127.0.0.1",dir->generic_string());
		screwdriver::start_rtr_tm();
		exit_command();
	}
	return 0;
}
