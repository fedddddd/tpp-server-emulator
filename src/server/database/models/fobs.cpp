#include <std_include.hpp>

#include "fobs.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>
#include <utils/nt.hpp>

#define TABLE_DEF R"(
create table if not exists `fobs`
(
	id                  bigint unsigned	not null	auto_increment,
	player_id	        bigint unsigned	not null,
	fob_index			bigint unsigned	not null,
	platform_count		int	unsigned	not null default 0,
	security_rank		int unsigned	not null default 0,
	area_id	            int unsigned	not null default 0,
	cluster_param		json,
	construct_param		int unsigned	not null default 0,
	create_date			datetime        default current_timestamp not null,
	primary key (`id`),
	foreign key (`player_id`) references players(`id`)
))"

namespace database::fobs
{
	nlohmann::json& get_area_list()
	{
		static auto list = utils::resources::load_json(RESOURCE_AREA_LIST);
		return list;
	}

	std::optional<nlohmann::json> get_area(const std::uint32_t area_id)
	{
		auto& list = get_area_list();
		auto& areas = list["area"];

		for (auto i = 0; i < areas.size(); i++)
		{
			if (areas[i]["area_id"] == area_id)
			{
				return {areas[i]};
			}
		}

		return {};
	}

	std::vector<fob> get_fob_list(const std::uint64_t player_id)
	{
		return database::access<std::vector<fob>>([&](database::database_t& db)
			-> std::vector<fob>
		{
			auto results = db->operator()(
				sqlpp::select(
					sqlpp::all_of(fob::table))
						.from(fob::table)
							.where(fob::table.player_id == player_id)
								.order_by(fob::table.create_date.asc()));

			std::vector<fob> list;

			for (const auto& row : results)
			{
				list.emplace_back(row);
			}

			return list;
		});
	}

	void create(const std::uint64_t player_id, const std::uint32_t area_id)
	{
		database::access([&](database::database_t& db)
		{
			const auto list = get_fob_list(player_id);
			const auto index = list.size();

			db->operator()(
				sqlpp::insert_into(fob::table)
					.set(fob::table.player_id = player_id,
						 fob::table.area_id = area_id,
						 fob::table.fob_index = index,
						 fob::table.cluster_param = "[]",
						 fob::table.construct_param = 0,
						 fob::table.create_date = std::chrono::system_clock::now()
				));
		});
	}

	void sync_data(const std::uint64_t player_id, std::vector<fob>& fobs)
	{
		auto list = get_fob_list(player_id);

		auto index = 0;
		for (auto& server_fob : list)
		{
			if (index >= fobs.size())
			{
				return;
			}

			auto& fob = fobs[index];
			auto& server_cluster_param = server_fob.get_cluster_param();
			auto& cluster_param = fob.get_cluster_param();

			const auto merge_custom_security = [](nlohmann::json& data, nlohmann::json& server_data, const std::string& key)
			{
				if (!data.is_object() || data[key].is_null())
				{
					data[key] = server_data[key];
				}
			};

			for (auto i = 0; i < cluster_param.size(); i++)
			{
				auto& data = cluster_param[i];
				auto& server_data = server_cluster_param[i];

				if (server_data.is_null() || data.is_null())
				{
					continue;
				}

				merge_custom_security(data, server_data, "voluntary_coord_camera_params");
				merge_custom_security(data, server_data, "voluntary_coord_mine_params");
			}

			const auto cluster_param_str = cluster_param.dump();

			database::access([&](database::database_t& db)
			{
				db->operator()(
					sqlpp::update(fob::table)
						.set(fob::table.security_rank = fob.get_security_rank(),
							 fob::table.platform_count = fob.get_platform_count(),
							 fob::table.construct_param = fob.get_construct_param(),
							 fob::table.cluster_param = cluster_param_str)
								.where(fob::table.player_id == player_id && fob::table.id == server_fob.get_id()));
			});

			++index;
		}
	}

	std::optional<fob> get_fob(const std::uint64_t id)
	{
		return database::access<std::optional<fob>>([&](database::database_t& db)
			-> std::optional<fob>
		{
			auto results = db->operator()(
				sqlpp::select(
					sqlpp::all_of(fob::table))
						.from(fob::table)
							.where(fob::table.id == id));

			if (results.empty())
			{
				return {};
			}

			return fob(results.front());
		});
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database->execute(TABLE_DEF);
		}
	};
}

REGISTER_TABLE(database::fobs::table, -1)
