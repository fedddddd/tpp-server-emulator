#include <std_include.hpp>

#include "cmd_purchase_fob.hpp"

#include "database/models/fobs.hpp"
#include "database/models/players.hpp"

#include <utils/nt.hpp>

namespace tpp
{
	nlohmann::json cmd_purchase_fob::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;

		result["result"] = "NOERR";

		if (!player.has_value())
		{
			return error(ERR_INVALID_SESSION);
		}
		
		const auto& area_id_j = data["area_id"];
		
		const auto p_data = database::player_data::find(player->get_id());
		const auto fob_list = database::fobs::get_fob_list(player->get_id());

		if (!p_data.get() || fob_list.size() >= 4 || fob_list.size() < 1 || !area_id_j.is_number_integer())
		{
			return error(ERR_INVALIDARG);
		}

		const auto area_id = area_id_j.get<std::uint32_t>();
		const auto area_opt = database::fobs::get_area(area_id);
		if (!area_opt.has_value())
		{
			return error(ERR_INVALIDARG);
		}

		const auto& area = area_opt.value();
		const auto price = area["price"].get<std::uint32_t>();

		if (database::player_data::spend_coins(player->get_id(), price))
		{
			database::fobs::create(player->get_id(), area_id);
		}
		else
		{
			result["result"] = utils::tpp::get_error(ERR_MBCOIN_SHORTAGE);
		}
		
		return result;
	}
}
