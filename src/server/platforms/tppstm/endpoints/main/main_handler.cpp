#include <std_include.hpp>

#include "main_handler.hpp"

#include "commands/cmd_abort_mother_base.hpp"
#include "commands/cmd_active_sneak_mother_base.hpp"
#include "commands/cmd_auth_steamticket.hpp"
#include "commands/cmd_calc_cost_fob_deploy_replace.hpp"
#include "commands/cmd_calc_cost_time_reduction.hpp"
#include "commands/cmd_check_defence_motherbase.hpp"
#include "commands/cmd_check_server_item_correct.hpp"
#include "commands/cmd_consume_reserve.hpp"
#include "commands/cmd_create_player.hpp"
#include "commands/cmd_deploy_fob_assist.hpp"
#include "commands/cmd_develop_server_item.hpp"
#include "commands/cmd_gdpr_check.hpp"
#include "commands/cmd_get_abolition_count.hpp"
#include "commands/cmd_get_challenge_task_rewards.hpp"
#include "commands/cmd_get_challenge_task_target_values.hpp"
#include "commands/cmd_get_combat_deploy_result.hpp"
#include "commands/cmd_get_daily_reward.hpp"
#include "commands/cmd_get_fob_damage.hpp"
#include "commands/cmd_get_fob_deploy_list.hpp"
#include "commands/cmd_get_fob_event_detail.hpp"
#include "commands/cmd_get_fob_event_list.hpp"
#include "commands/cmd_get_fob_notice.hpp"
#include "commands/cmd_get_fob_param.hpp"
#include "commands/cmd_get_fob_reward_list.hpp"
#include "commands/cmd_get_fob_status.hpp"
#include "commands/cmd_get_fob_target_detail.hpp"
#include "commands/cmd_get_fob_target_list.hpp"
#include "commands/cmd_get_informationlist2.hpp"
#include "commands/cmd_get_league_result.hpp"
#include "commands/cmd_get_login_param.hpp"
#include "commands/cmd_get_mbcoin_remainder.hpp"
#include "commands/cmd_get_online_prison_list.hpp"
#include "commands/cmd_get_own_fob_list.hpp"
#include "commands/cmd_get_playerlist.hpp"
#include "commands/cmd_get_player_platform_list.hpp"
#include "commands/cmd_get_purchasable_area_list.hpp"
#include "commands/cmd_get_resource_param.hpp"
#include "commands/cmd_get_security_info.hpp"
#include "commands/cmd_get_security_setting_param.hpp"
#include "commands/cmd_get_server_item.hpp"
#include "commands/cmd_get_server_item_list.hpp"
#include "commands/cmd_mining_resource.hpp"
#include "commands/cmd_open_wormhole.hpp"
#include "commands/cmd_purchase_first_fob.hpp"
#include "commands/cmd_purchase_fob.hpp"
#include "commands/cmd_purchase_platform_construction.hpp"
#include "commands/cmd_reqauth_https.hpp"
#include "commands/cmd_sale_resource.hpp"
#include "commands/cmd_send_boot.hpp"
#include "commands/cmd_send_ipandport.hpp"
#include "commands/cmd_send_sneak_result.hpp"
#include "commands/cmd_set_currentplayer.hpp"
#include "commands/cmd_set_security_challenge.hpp"
#include "commands/cmd_sneak_mother_base.hpp"
#include "commands/cmd_sync_emblem.hpp"
#include "commands/cmd_sync_loadout.hpp"
#include "commands/cmd_sync_mother_base.hpp"
#include "commands/cmd_sync_resource.hpp"
#include "commands/cmd_sync_soldier_bin.hpp"
#include "commands/cmd_sync_soldier_diff.hpp"
#include "commands/cmd_update_session.hpp"

#include "database/database.hpp"
#include "database/models/players.hpp"

#include "utils/encoding.hpp"
#include "utils/tpp.hpp"

#include <utils/string.hpp>
#include <utils/compression.hpp>

namespace tpp
{
	main_handler::main_handler()
	{
		blow_.set_key(utils::tpp::get_static_key(), utils::tpp::get_static_key_len());

		this->register_handler<cmd_abort_mother_base>("CMD_ABORT_MOTHER_BASE");
		this->register_handler<cmd_active_sneak_mother_base>("CMD_ACTIVE_SNEAK_MOTHER_BASE");
		this->register_handler<cmd_auth_steamticket>("CMD_AUTH_STEAMTICKET");
		this->register_handler<cmd_calc_cost_fob_deploy_replace>("CMD_CALC_COST_FOB_DEPLOY_REPLACE");
		this->register_handler<cmd_calc_cost_time_reduction>("CMD_CALC_COST_TIME_REDUCTION");
		this->register_handler<cmd_check_defence_motherbase>("CMD_CHECK_DEFENCE_MOTHERBASE");
		this->register_handler<cmd_check_server_item_correct>("CMD_CHECK_SERVER_ITEM_CORRECT");
		this->register_handler<cmd_consume_reserve>("CMD_CONSUME_RESERVE");
		this->register_handler<cmd_create_player>("CMD_CREATE_PLAYER");
		this->register_handler<cmd_deploy_fob_assist>("CMD_DEPLOY_FOB_ASSIST");
		this->register_handler<cmd_develop_server_item>("CMD_DEVELOP_SERVER_ITEM");
		this->register_handler<cmd_gdpr_check>("CMD_GDPR_CHECK");
		this->register_handler<cmd_get_abolition_count>("CMD_GET_ABOLITION_COUNT");
		this->register_handler<cmd_get_challenge_task_rewards>("CMD_GET_CHALLENGE_TASK_REWARDS");
		this->register_handler<cmd_get_challenge_task_target_values>("CMD_GET_CHALLENGE_TASK_TARGET_VALUES");
		this->register_handler<cmd_get_combat_deploy_result>("CMD_GET_COMBAT_DEPLOY_RESULT");
		this->register_handler<cmd_get_daily_reward>("CMD_GET_DAILY_REWARD");
		this->register_handler<cmd_get_fob_damage>("CMD_GET_FOB_DAMAGE");
		this->register_handler<cmd_get_fob_deploy_list>("CMD_GET_FOB_DEPLOY_LIST");
		this->register_handler<cmd_get_fob_event_detail>("CMD_GET_FOB_EVENT_DETAIL");
		this->register_handler<cmd_get_fob_event_list>("CMD_GET_FOB_EVENT_LIST");
		this->register_handler<cmd_get_fob_notice>("CMD_GET_FOB_NOTICE");
		this->register_handler<cmd_get_fob_param>("CMD_GET_FOB_PARAM");
		this->register_handler<cmd_get_fob_reward_list>("CMD_GET_FOB_REWARD_LIST");
		this->register_handler<cmd_get_fob_status>("CMD_GET_FOB_STATUS");
		this->register_handler<cmd_get_fob_target_detail>("CMD_GET_FOB_TARGET_DETAIL");
		this->register_handler<cmd_get_fob_target_list>("CMD_GET_FOB_TARGET_LIST");
		this->register_handler<cmd_get_informationlist2>("CMD_GET_INFORMATIONLIST2");
		this->register_handler<cmd_get_league_result>("CMD_GET_LEAGUE_RESULT");
		this->register_handler<cmd_get_login_param>("CMD_GET_LOGIN_PARAM");
		this->register_handler<cmd_get_mbcoin_remainder>("CMD_GET_MBCOIN_REMAINDER");
		this->register_handler<cmd_get_online_prison_list>("CMD_GET_ONLINE_PRISON_LIST");
		this->register_handler<cmd_get_own_fob_list>("CMD_GET_OWN_FOB_LIST");
		this->register_handler<cmd_get_playerlist>("CMD_GET_PLAYERLIST");
		this->register_handler<cmd_get_player_platform_list>("CMD_GET_PLAYER_PLATFORM_LIST");
		this->register_handler<cmd_get_purchasable_area_list>("CMD_GET_PURCHASABLE_AREA_LIST");
		this->register_handler<cmd_get_resource_param>("CMD_GET_RESOURCE_PARAM");
		this->register_handler<cmd_get_security_info>("CMD_GET_SECURITY_INFO");
		this->register_handler<cmd_get_security_setting_param>("CMD_GET_SECURITY_SETTING_PARAM");
		this->register_handler<cmd_get_server_item>("CMD_GET_SERVER_ITEM");
		this->register_handler<cmd_get_server_item_list>("CMD_GET_SERVER_ITEM_LIST");
		this->register_handler<cmd_mining_resource>("CMD_MINING_RESOURCE");
		this->register_handler<cmd_open_wormhole>("CMD_OPEN_WORMHOLE");
		this->register_handler<cmd_purchase_first_fob>("CMD_PURCHASE_FIRST_FOB");
		this->register_handler<cmd_purchase_fob>("CMD_PURCHASE_FOB");
		this->register_handler<cmd_purchase_platform_construction>("CMD_PURCHASE_PLATFORM_CONSTRUCTION");
		this->register_handler<cmd_reqauth_https>("CMD_REQAUTH_HTTPS");
		this->register_handler<cmd_sale_resource>("CMD_SALE_RESOURCE");
		this->register_handler<cmd_send_boot>("CMD_SEND_BOOT");
		this->register_handler<cmd_send_ipandport>("CMD_SEND_IPANDPORT");
		this->register_handler<cmd_send_sneak_result>("CMD_SEND_SNEAK_RESULT");
		this->register_handler<cmd_set_currentplayer>("CMD_SET_CURRENTPLAYER");
		this->register_handler<cmd_set_security_challenge>("CMD_SET_SECURITY_CHALLENGE");
		this->register_handler<cmd_sneak_mother_base>("CMD_SNEAK_MOTHER_BASE");
		this->register_handler<cmd_sync_emblem>("CMD_SYNC_EMBLEM");
		this->register_handler<cmd_sync_loadout>("CMD_SYNC_LOADOUT");
		this->register_handler<cmd_sync_mother_base>("CMD_SYNC_MOTHER_BASE");
		this->register_handler<cmd_sync_resource>("CMD_SYNC_RESOURCE");
		this->register_handler<cmd_sync_soldier_bin>("CMD_SYNC_SOLDIER_BIN");
		this->register_handler<cmd_sync_soldier_diff>("CMD_SYNC_SOLDIER_DIFF");
		this->register_handler<cmd_update_session>("CMD_UPDATE_SESSION");
	}

	std::optional<nlohmann::json> main_handler::decrypt_request(const std::string& data, std::optional<database::players::player>& player)
	{
		if (!data.starts_with("httpMsg="))
		{
			return {};
		}

		const auto result = data.substr(8);
		const auto decoded_data = utils::encoding::decode_url_string(result);

		const auto str = this->blow_.decrypt(decoded_data);
		auto json = nlohmann::json::parse(str);

		if (!json["data"].is_string())
		{
			return json;
		}

		const auto& compressed_val = json["compress"];
		if (!compressed_val.is_boolean())
		{
			return json;
		}

		const auto compressed = compressed_val.get<bool>();
		const auto& session_crypto = json["session_crypto"];
		const auto data_str = json["data"].get<std::string>();

		if (session_crypto.is_boolean() && session_crypto.get<bool>())
		{
			const auto session_key = json["session_key"].get<std::string>();
			player = database::players::find_by_session_id(session_key, false);
			if (!player.has_value())
			{
				json["data"] = {};
				return json;
			}

			utils::cryptography::blowfish session_blow;
			session_blow.set_key(player->get_crypto_key());

			const auto decrypted = session_blow.decrypt(data_str);
			if (!compressed)
			{
				const auto unescaped_data = utils::encoding::unescape_json(decrypted);
				json["data"] = nlohmann::json::parse(unescaped_data);
			}
			else
			{
				const auto decompressed = utils::compression::zlib::decompress(decrypted);
				const auto unescaped_data = utils::encoding::unescape_json(decompressed);
				json["data"] = nlohmann::json::parse(unescaped_data);
			}
		}
		else
		{
			if (!compressed)
			{
				const auto unescaped_data = utils::encoding::unescape_json(data_str);
				const auto data_json = nlohmann::json::parse(unescaped_data);
				json["data"] = data_json;

			}
			else
			{
				const auto decoded = utils::cryptography::base64::decode(data_str);
				const auto decompressed = utils::compression::zlib::decompress(decoded);
				const auto unescaped_data = utils::encoding::unescape_json(decompressed);
				json["data"] = nlohmann::json::parse(unescaped_data);
			}
		}

		return json;
	}

	bool main_handler::verify_request(const nlohmann::json& request)
	{
		const auto& data = request["data"];
		if (!data.is_object())
		{
			return false;
		}

		const auto& session_crypto = request["session_crypto"];
		const auto& session_key = request["session_key"];
		if (!session_crypto.is_boolean())
		{
			return false;
		}

		if (session_crypto.get<bool>() && (!session_key.is_string() || session_key.get<std::string>().empty()))
		{
			return false;
		}

		const auto& msgid = data["msgid"];
		const auto& rq_id = data["rqid"];

		if (!msgid.is_string() || !rq_id.is_number_integer())
		{
			return false;
		}

		return true;
	}

	std::optional<std::string> main_handler::encrypt_response(const nlohmann::json& request, nlohmann::json data, 
		const std::optional<database::players::player>& player)
	{
		const auto& session_crypto_val = request["session_crypto"];
		const auto session_crypto = session_crypto_val.is_boolean() && session_crypto_val.get<bool>();

		if (session_crypto)
		{
			data["crypto_type"] = "COMPOUND";
		}
		else
		{
			data["crypto_type"] = "COMMON";
		}

		data["flowid"] = {};
		data["xuid"] = {};
		data["rqid"] = request["data"]["rqid"];
		data["msgid"] = request["data"]["msgid"];

		if (data["result"].is_null())
		{
			data["result"] = "NOERR";
		}

		auto data_dump = data.dump();
		const auto original_size = data_dump.size();

		data_dump = utils::compression::zlib::compress(data_dump);
		data_dump += '\0';

		if (!session_crypto)
		{
			data_dump = utils::cryptography::base64::encode(data_dump);
		}
		else
		{
			if (!player.has_value())
			{
				return {};
			}

			utils::cryptography::blowfish session_blow;
			session_blow.set_key(player->get_crypto_key());
			data_dump = session_blow.encrypt(data_dump);
		}

		nlohmann::json response;

		response["compress"] = true;
		response["data"] = utils::encoding::split_into_lines(data_dump);
		response["original_size"] = original_size;
		response["session_crypto"] = request["session_crypto"];
		response["session_key"] = request["session_key"];

		const auto response_str = response.dump();
		const auto str = this->blow_.encrypt(response_str);

		const auto encoded = utils::encoding::split_into_lines(str);
		return {encoded};
	}

	nlohmann::json error(const std::string& id)
	{
		nlohmann::json result;
		result["result"] = id;
		return result;
	}
}
