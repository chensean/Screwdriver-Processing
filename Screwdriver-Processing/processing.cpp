#include <iostream>
#define BOOST_THREAD_DYN_LINK
#include "filesystem_utilities.h"
#include "context_global.h"
#include "infrastructure_global.h"

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

//测试参数显示
class parameter_display
{
public:
	parameter_display()
	{
	}

	~parameter_display()
	{
	}

	void display(const screwdriver::parameter_ptr& param_ptr)
	{
		std::cout << "name:" << param_ptr->get_name() << " time:" << param_ptr->get_time() << " value:" << param_ptr->get_value();
	}
};

int main()
{
	std::string config_path("config");
	auto dir = utilities::get_directorty_path(config_path);
	if (dir)
	{
		config_path = dir->generic_string() + "/cfg.xml";
		screwdriver::load_irig_config(config_path);
		//screwdriver::create_rtr_tm_client("127.0.0.1",dir->generic_string());
		//screwdriver::start_rtr_tm();

		auto parameters = screwdriver::get_all_parameters();
		std::for_each(parameters.begin(), parameters.end()
		              , [](const screwdriver::parameter_ptr& param_ptr)
		              {
			              boost::shared_ptr<parameter_display> param_display(new parameter_display);
			              param_ptr->connect_val_charged_signal(screwdriver::parameter_charged_slot_t(&parameter_display::display, param_display, _1).track(param_display));
		              });
		exit_command();
	}
	return 0;
}
