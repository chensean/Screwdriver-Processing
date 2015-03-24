#include "filesystem_utilities.h"

namespace utilities
{
	boost::optional<fs::path> get_path(const fs::path& current_path, const std::string& sub_dir)
	{
		typedef boost::optional<fs::path> result_t;
		typedef fs::recursive_directory_iterator rd__iterator;
		rd__iterator end;
		for (rd__iterator pos(current_path.parent_path()); pos != end; ++pos)
		{
			if (fs::is_directory(*pos) && pos->path().filename() == sub_dir)
			{
				return result_t(pos->path());
			}
		}
		return result_t();
	}

	boost::optional<fs::path> get_directorty_path(const std::string& sub_dir)
	{
		typedef boost::optional<fs::path> result_t;
		auto current_path = fs::current_path();

		auto dir = get_path(current_path, sub_dir);
		if (dir)
		{
			return dir;
		}
		else
		{
			if (current_path.has_parent_path())
			{
				current_path = current_path.parent_path();
			}
			return get_path(current_path, sub_dir);
		}
	}
}
