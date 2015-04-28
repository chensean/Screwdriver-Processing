#include <iostream>
#define BOOST_THREAD_DYN_LINK
#include "filesystem_utilities.h"
#include "context_global.h"
#include "infrastructure_global.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

# pragma warning( push )
#pragma warning(disable:4005)
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
# pragma warning( pop )

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

	void display(const screwdriver::parameter_value_ptr& param_ptr)
	{
		std::cout << "name:" << param_ptr->get_name() << " time:" << param_ptr->get_time() << " value:" << param_ptr->get_value();
	}
};


int main()
{
	//using namespace boost::posix_time;
	//using namespace boost::gregorian;
	//ptime start_time(time_from_string("1970-01-01 00:00:00.000"));
	//ptime end_time(time_from_string("2015-01-01 00:00:00.000"));
	//auto td = end_time - start_time;
	//std::cout<<td.total_seconds()<<std::endl;

	std::string config_path("config");
	auto dir = utilities::get_directorty_path(config_path);
	if (dir)
	{
		config_path = dir->generic_string() + "/cfg2.xml";
		screwdriver::load_irig_config(config_path);

		//auto parameters = screwdriver::get_all_parameters();
		//std::for_each(parameters.begin(), parameters.end()
		//              , [](const screwdriver::parameter_ptr& param_ptr)
		//              {
		//	              boost::shared_ptr<parameter_display> param_display(new parameter_display);
		//	              param_ptr->connect_val_charged_signal(screwdriver::parameter_charged_slot_t(&parameter_display::display, param_display, _1).track(param_display));
		//              });

		std::string data_path("data");
		auto data_dir = utilities::get_directorty_path(data_path);
		if (data_dir)
		{
			screwdriver::start_save_file(data_dir->generic_string());
		}
		screwdriver::start_rtr_tm();
		//screwdriver::start_rtr_monitor();
		exit_command();
	}
	return 0;
}

//int main()
//{
//	using namespace boost::property_tree;
//	ptree pt;
//	ptree pt_child;
//
//	pt_child.put("<xmlattr>.WordLength", 16);
//	pt.add_child("CFG.ACQ_SYS.SOURCE.IRIG.IRIGFrameStruc", pt_child);
//
//	ptree pt_child2 = pt.get_child("CFG.ACQ_SYS.SOURCE.IRIG.IRIGFrameStruc");
//	pt_child2.put("<xmlattr>.WordLength", 32);
//	std::string config_path("config");
//	auto dir = utilities::get_directorty_path(config_path);
//	if (dir)
//	{
//		config_path = dir->generic_string() + "/cfgtest.xml";
//		try
//		{
//			write_xml(config_path, pt);
//		}
//		catch (xml_parser_error& e)
//		{
//			std::cout << "xml_parser_error! path:" << config_path << std::endl;
//			std::cout << e.message() << std::endl;
//			return -1;
//		}
//	}
//
//	return 0;
//}

