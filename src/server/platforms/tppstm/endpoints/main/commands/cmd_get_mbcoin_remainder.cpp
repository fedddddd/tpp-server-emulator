#include <std_include.hpp>

#include "cmd_get_mbcoin_remainder.hpp"

#include "database/models/player_data.hpp"
#include "database/models/players.hpp"

#include <utils/nt.hpp>

namespace tpp
{
	nlohmann::json cmd_get_mbcoin_remainder::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;

		if (!player.has_value())
		{
			result["result"] = "ERR_INVALID_SESSION";
			return result;
		}

		const auto p_data = database::player_data::find(player->get_id());
		if (!player.has_value())
		{
			result["result"] = "ERR_INVALIDARG";
			return result;
		}

		result["remainder"] = p_data->get_mb_coin();

		return result;
	}
}
