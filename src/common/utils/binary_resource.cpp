#include "binary_resource.hpp"

#include <utility>
#include "nt.hpp"
#include "io.hpp"

namespace utils
{
	namespace
	{
		std::string write_exitisting_temp_file(const std::string& file, const std::string& data,
			const bool fatal_if_overwrite_fails)
		{
			const auto temp = nt::get_temp_folder();
			auto file_path = temp + file;

			std::string current_data;
			if (!io::read_file(file_path, &current_data))
			{
				if (!io::write_file(file_path, data))
				{
					throw std::runtime_error("Failed to write file: " + file_path);
				}

				return file_path;
			}

			if (current_data == data || io::write_file(file_path, data) || !fatal_if_overwrite_fails)
			{
				return file_path;
			}

			throw std::runtime_error(
				"Temporary file was already written, but differs. It can't be overwritten as it's still in use: " +
				file_path);
		}
	}

	binary_resource::binary_resource(const int id, std::string file)
		: filename_(std::move(file))
	{
		this->resource_ = nt::load_resource(id);

		if (this->resource_.empty())
		{
			throw std::runtime_error("Unable to load resource: " + std::to_string(id));
		}
	}

	void binary_resource::write_extracted_file(const bool fatal_if_overwrite_fails)
	{
		if (this->path_.empty())
		{
			this->path_ = write_exitisting_temp_file(this->filename_, this->resource_, fatal_if_overwrite_fails);
		}
	}

	std::string binary_resource::get_extracted_file(const bool fatal_if_overwrite_fails)
	{
		if (this->path_.empty())
		{
			this->path_ = write_exitisting_temp_file(this->filename_, this->resource_, fatal_if_overwrite_fails);
		}

		return this->path_;
	}

	const std::string& binary_resource::get_data() const
	{
		return this->resource_;
	}
}
