#include <std_include.hpp>

#include "cmd_cancel_short_pfleague.hpp"

// unimplemented

namespace tpp
{
	nlohmann::json cmd_cancel_short_pfleague::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;
		result["result"] = "ERR_NOTIMPLEMENTED";
		return result;
	}
}
