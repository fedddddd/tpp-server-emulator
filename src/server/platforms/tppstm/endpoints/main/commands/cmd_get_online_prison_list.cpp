#include <std_include.hpp>

#include "cmd_get_online_prison_list.hpp"

#include <utils/nt.hpp>

namespace tpp
{
	nlohmann::json cmd_get_online_prison_list::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;

		result["prison_soldier_param"] = nlohmann::json::array();
		result["rescue_list"] = nlohmann::json::array();
		result["rescue_num"] = 0;
		result["soldier_num"] = 0;
		result["total_num"] = 0;
		result["result"] = utils::tpp::get_error(NOERR);

		return result;
	}
}
