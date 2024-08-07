#include <std_include.hpp>

#include "cmd_update_session.hpp"

#include "database/models/players.hpp"

#include <utils/nt.hpp>

namespace tpp
{
	nlohmann::json cmd_update_session::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;

		if (!player.has_value())
		{
			return error(ERR_INVALID_SESSION);
		}

		if (!database::players::update_session(player.value()))
		{
			return error(ERR_DATABASE);
		}

		const auto active_sneak = database::players::find_active_sneak(player->get_id(), true, true);
		auto sneak_mode = active_sneak.has_value() ? 0 : -1;
		auto fob_index = active_sneak.has_value() ? 0 : -1;

		result["fob_index"] = sneak_mode;
		result["sneak_mode"] = fob_index;
		result["result"] = utils::tpp::get_error(NOERR);

		return result;
	}
}
