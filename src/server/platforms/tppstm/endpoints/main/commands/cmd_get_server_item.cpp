#include <std_include.hpp>

#include "cmd_get_server_item.hpp"

#include "database/models/items.hpp"
#include "database/models/players.hpp"

#include <utils/nt.hpp>

namespace tpp
{
	nlohmann::json cmd_get_server_item::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;

		if (!player.has_value())
		{
			return error(ERR_INVALID_SESSION);
		}

		const auto& item_id_j = data["item_id"];
		if (!item_id_j.is_number_integer())
		{
			return error(ERR_INVALIDARG);
		}

		const auto item_id = item_id_j.get<std::uint32_t>();
		auto item_status = database::items::get_item(player->get_id(), item_id);

		result["item"]["create_date"] = item_status.get_create_time();
		result["item"]["develop"] = item_status.get_develop();
		result["item"]["gmp"] = item_status.get_gmp();
		result["item"]["id"] = item_status.get_id();
		result["item"]["left_second"] = item_status.get_left_second();
		result["item"]["max_second"] = item_status.get_max_second();
		result["item"]["mb_coin"] = utils::tpp::calculate_mb_coins(item_status.get_max_second(), database::vars.cost_factor_item_dev);
		result["item"]["open"] = item_status.is_open();

		result["result"] = "NOERR";
		result["xuid"] = {};

		return result;
	}
}
