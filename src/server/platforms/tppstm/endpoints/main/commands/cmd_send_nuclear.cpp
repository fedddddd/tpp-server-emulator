#include <std_include.hpp>

#include "cmd_send_nuclear.hpp"

// unimplemented

namespace tpp
{
	nlohmann::json cmd_send_nuclear::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;
		result["result"] = "ERR_NOTIMPLEMENTED";
		return result;
	}
}
