#include <iostream>
#define BOOST_THREAD_DYN_LINK
#include "filesystem_utilities.h"
#include "context_global.h"
#include "infrastructure_global.h"

void exit_command()
{
	char q = 0;
	std::cout << "����q�˳�" << std::endl;
	std::cin >> q;
	while (q != 'q' && q != 'Q')
	{
		std::cout << "����q�˳�" << std::endl;
		std::cin >> q;
	}
}

//���Բ�����ʾ
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
	std::string data_path("data");
	auto data_dir = utilities::get_directorty_path(data_path);
	if (data_dir)
	{
		screwdriver::set_save_folder(data_dir->generic_string());
	}

	std::string config_path("config");
	auto dir = utilities::get_directorty_path(config_path);
	if (dir)
	{
		config_path = dir->generic_string() + "/cfg.xml";
		screwdriver::load_irig_config(config_path);
		auto parameters = screwdriver::get_all_parameters();
		std::for_each(parameters.begin(), parameters.end()
		              , [](const screwdriver::parameter_ptr& param_ptr)
		              {
			              boost::shared_ptr<parameter_display> param_display(new parameter_display);
			              param_ptr->connect_val_charged_signal(screwdriver::parameter_charged_slot_t(&parameter_display::display, param_display, _1).track(param_display));
		              });
		screwdriver::start_save_file();
		screwdriver::start_rtr_tm();
		exit_command();
	}
	return 0;
}
