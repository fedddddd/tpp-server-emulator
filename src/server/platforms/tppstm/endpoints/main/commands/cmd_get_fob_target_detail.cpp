#include <std_include.hpp>

#include "cmd_get_fob_target_detail.hpp"

#include "database/models/fobs.hpp"
#include "database/models/player_records.hpp"
#include "database/models/players.hpp"
#include "database/models/wormholes.hpp"

#include <utils/nt.hpp>

namespace tpp
{
	nlohmann::json cmd_get_fob_target_detail::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		nlohmann::json result;
		result["result"] = "NOERR";

		if (!player.has_value())
		{
			return error(ERR_INVALID_SESSION);
		}

		const auto player_data = database::player_data::find(player->get_id());
		if (!player_data.get())
		{
			return error(ERR_INVALIDARG);
		}
		
		const auto stats = database::player_records::find(player->get_id());
		if (!stats.has_value())
		{
			return error(ERR_INVALIDARG);
		}

		const auto& is_sneak_j = data["is_sneak"];
		const auto& is_event_j = data["is_event"];
		const auto& is_plus_j = data["is_plus"];
		const auto& mode_j = data["mode"];
		const auto& mother_base_id_j = data["mother_base_id"];

		if (!is_sneak_j.is_number_integer() || !is_event_j.is_number_integer() || !is_plus_j.is_number_integer() ||
			!mode_j.is_string() || !mother_base_id_j.is_number_integer())
		{
			return error(ERR_INVALIDARG);
		}

		const auto mother_base_id = mother_base_id_j.get<std::uint64_t>();
		auto fob = database::fobs::get_fob(mother_base_id);
		if (!fob.has_value())
		{
			return error(ERR_INVALIDARG);
		}

		const auto mode_str = mode_j.get<std::string>();
		const auto mode = database::players::get_sneak_mode_id(mode_str);
		if (mode == database::players::mode_invalid)
		{
			return error(ERR_INVALIDARG);
		}

		auto& detail = result["detail"];

		detail["captured_rank_bottom"] = 0;
		detail["captured_rank_top"] = 0;
		detail["captured_staff"] = 0;
		detail["captured_rank_bottom"] = 0;

		detail["mother_base_param"]["area_id"] = 0;
		detail["mother_base_param"]["fob_index"] = fob->get_index();
		detail["mother_base_param"]["platform_count"] = 0;
		detail["mother_base_param"]["price"] = 0;
		detail["mother_base_param"]["security_rank"] = 0;
		detail["mother_base_param"]["cluster_param"] = fob->get_cluster_param();
		detail["mother_base_param"]["construct_param"] = fob->get_construct_param();
		detail["mother_base_param"]["mother_base_id"] = fob->get_id();

		auto damage_params = player_data->get_fob_deploy_damage_param();
		database::player_data::apply_deploy_damage_params(fob->get_id(), detail["mother_base_param"]["cluster_param"], damage_params);

		detail["owner_player_id"] = fob->get_player_id();

		const auto owner = database::players::find(fob->get_player_id());
		if (!owner.has_value())
		{
			return error(ERR_DATABASE);
		}

		const auto owner_record = database::player_records::find(fob->get_player_id());
		if (!owner_record.has_value())
		{
			return error(ERR_DATABASE);
		}

		static std::vector<std::string> resource_names =
		{
			"emplacement_gun_east",
			"emplacement_gun_west" ,
			"gatling_gun",
			"gatling_gun_east",
			"gatling_gun_west",
			"mortar_normal"
		};

		detail["placement"][resource_names[0]] = player_data->get_resource_value(database::player_data::processed_server, 34);
		detail["placement"][resource_names[1]] = player_data->get_resource_value(database::player_data::processed_server, 35);
		detail["placement"][resource_names[2]] = 0;
		detail["placement"][resource_names[3]] = player_data->get_resource_value(database::player_data::processed_server, 36);
		detail["placement"][resource_names[4]] = player_data->get_resource_value(database::player_data::processed_server, 37);
		detail["placement"][resource_names[5]] = player_data->get_resource_value(database::player_data::processed_server, 38);

		detail["platform"] = 255;
		detail["reward_rate"] = 0;

		detail["primary_reward"] = nlohmann::json::array();

		for (auto i = 0u; i < 10; i++)
		{
			for (const auto& name : database::player_data::unit_names)
			{
				detail["section_staff"][i][name] = 0;
			}
		}

		for (auto i = 0u; i < player_data->get_staff_count(); i++)
		{
			const auto staff = player_data->get_staff(i);
			const auto key_opt = database::player_data::unit_name_from_designation(staff.status_sync.designation);
			if (!key_opt.has_value())
			{
				continue;
			}

			if (!database::player_data::is_usable_staff(staff))
			{
				continue;
			}

			const auto& key = key_opt.value();
			const auto& value = detail["section_staff"][staff.header.peak_rank][key];
			detail["section_staff"][staff.header.peak_rank][key] = 1 + value.get<std::uint32_t>();
		}

		detail["security_section_rank"] = player_data->get_unit_level(database::player_data::unit_security);

		result["event_clear_bit"] = 0;
		result["is_restrict"] = stats->is_shield_active();

		result["session"]["ip"] = "0.0.0.0";
		result["session"]["is_invalid"] = 1;
		result["session"]["npid"]["handler"]["data"] = "";
		result["session"]["npid"]["handler"]["dummy"] = {0, 0, 0};
		result["session"]["npid"]["handler"]["term"] = 0;
		result["session"]["npid"]["opt"] = {0, 0, 0, 0, 0, 0, 0, 0};
		result["session"]["npid"]["reserved"] = {0, 0, 0, 0, 0, 0, 0, 0};
		result["session"]["port"] = 0;
		result["session"]["secure_device_address"] = "NotImplement";
		result["session"]["steamid"] = 0;
		result["session"]["xnaddr"] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
		result["session"]["xnkey"] = {};
		result["session"]["xnkid"] = {};
		result["session"]["xuid"] = 0;

		const auto wormhole = database::wormholes::get_wormhole_status(player->get_id(), fob->get_player_id());

		if ((!wormhole.open || !wormhole.first) && owner_record->is_shield_active())
		{
			return error(ERR_SNEAK_RESTRICTION);
		}

		const auto is_sneak = is_sneak_j.get<std::uint32_t>() == 1;
		if (is_sneak)
		{
			result["session"]["ip"] = player->get_ex_ip();
			result["session"]["is_invalid"] = 0;
			result["session"]["port"] = player->get_ex_port();
			result["session"]["steamid"] = player->get_account_id();
			result["session"]["xuid"] = player->get_account_id();

			if (!database::players::set_active_sneak(player->get_id(), fob->get_id(), fob->get_player_id(), 0, mode,
				database::players::status_menu, true, owner->is_security_challenge_enabled()))
			{
				return error(ERR_ALREADY_SNEAK);
			}
		}
		else
		{
			const auto active_sneak = database::players::get_active_sneak(mother_base_id);
			if (!active_sneak.has_value())
			{
				return error(ERR_DATABASE);
			}

			const auto attacker = database::players::find(active_sneak->get_player_id());
			if (!attacker.has_value())
			{
				return error(ERR_DATABASE);
			}

			detail["platform"] = active_sneak->get_platform();

			result["session"]["ip"] = attacker->get_ex_ip();
			result["session"]["is_invalid"] = 0;
			result["session"]["port"] = attacker->get_ex_port();
			result["session"]["steamid"] = attacker->get_account_id();
			result["session"]["xuid"] = attacker->get_account_id();

			if (!database::players::set_active_sneak(player->get_id(), fob->get_id(), fob->get_player_id(), active_sneak->get_platform(), mode,
				database::players::status_menu, false, owner->is_security_challenge_enabled()))
			{
				return error(ERR_ALREADY_SNEAK);
			}
		}

		return result;
	}
}
