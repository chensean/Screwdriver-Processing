#include "raw_data.h"


namespace screwdriver
{
	struct raw_data::raw_data_imp_t
	{
		raw_data_imp_t(const std::string& name)
			:name_(name)
		{
		}

		std::string name_;
		raw_data_charged_signal_t raw_data_charged_;
	};


	raw_data::raw_data(const std::string& name)
		:imp_(new raw_data_imp_t(name))
	{
	}

	raw_data::~raw_data(void)
	{
	}

	std::string raw_data::get_name() const
	{
		return imp_->name_;
	}

	void raw_data::set_data(const data_ptr& data)
	{
		imp_->raw_data_charged_(data);
	}


	boost::signals2::connection raw_data::connect_val_charged_signal(const raw_data_charged_slot_t& slot)
	{
		return imp_->raw_data_charged_.connect(slot);
	}
}
