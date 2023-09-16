#include <std_include.hpp>

#include "items.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>
#include <utils/nt.hpp>

#define TABLE_DEF R"(
create table if not exists `items`
(
	id                  bigint unsigned	not null	auto_increment,
	player_id           bigint unsigned	not null,
	item_id				bigint unsigned	not null,
	create_date			datetime        default current_timestamp not null,
	primary key (`id`),
	foreign key (`player_id`) references players(`id`)
))"

namespace database::items
{
	namespace
	{
		std::vector<item_t> load_static_list()
		{
			static const auto data = database::items::get_static_list_json();
			std::vector<item_t> items;

			if (!data.is_array())
			{
				return items;
			}

			for (auto i = 0; i < data.size(); i++)
			{
				item_t item{};
				auto& entry = data[i];

				item.dev_coin = entry["dev_coin"].get<std::uint32_t>();
				item.dev_gmp = entry["dev_gmp"].get<std::uint32_t>();
				item.dev_item_1 = entry["dev_item_1"].get<std::uint32_t>();
				item.dev_item_2 = entry["dev_item_2"].get<std::uint32_t>();

				for (auto plat = 0; plat < 7; plat++)
				{
					const auto key = std::format("dev_platlv0{}", plat + 1);
					item.dev_platform_levels[plat] = entry[key].get<std::uint32_t>();
				}

				item.dev_resource_values[0] = entry["dev_rescount01_value"].get<std::uint32_t>();
				item.dev_resource_values[1] = entry["dev_rescount02_value"].get<std::uint32_t>();

				item.dev_resource_ids[0] = entry["dev_resource01_id"].get<std::uint32_t>();
				item.dev_resource_ids[1] = entry["dev_resource02_id"].get<std::uint32_t>();

				item.dev_skill = entry["dev_skil"].get<std::uint32_t>();
				item.dev_special = entry["dev_special"].get<std::uint32_t>();
				item.dev_time = entry["dev_time"].get<std::uint32_t>();

				item.id = entry["id"].get<std::uint32_t>();
				item.type = entry["type"].get<std::uint32_t>();

				item.use_gmp = entry["use_gmp"].get<std::uint32_t>();

				item.use_resource_values[0] = entry["use_rescount01_value"].get<std::uint32_t>();
				item.use_resource_values[1] = entry["use_rescount02_value"].get<std::uint32_t>();

				item.use_resource_ids[0] = entry["use_resource01_id"].get<std::uint32_t>();
				item.use_resource_ids[1] = entry["use_resource02_id"].get<std::uint32_t>();

				items.emplace_back(item);
			}

			return items;
		}

		class a
		{
		public:
			a()
			{
				load_static_list();
			}
		};

		a ab;

		std::unordered_map<std::uint32_t, item_t> load_static_map()
		{
			static const auto list = load_static_list();
			std::unordered_map<std::uint32_t, item_t> map;

			for (const auto& item : list)
			{
				map.insert(std::make_pair(item.id, item));
			}

			return map;
		}
	}

	auto items_table = items_table_t();

	nlohmann::json get_static_list_json()
	{
		static const auto data = nlohmann::json::parse(utils::nt::load_resource(RESOURCE_ITEM_LIST));
		return data;
	}

	std::vector<item_t>& get_static_list()
	{
		static std::vector<item_t> list = load_static_list();
		return list;
	}

	std::unordered_map<std::uint32_t, item_t>& get_static_map()
	{
		static std::unordered_map<std::uint32_t, item_t> list = load_static_map();
		return list;
	}

	item_t get_item(const std::uint32_t id)
	{
		const auto& map = get_static_map();
		const auto iter = map.find(id);
		if (iter == map.end())
		{
			return {};
		}

		return iter->second;
	}

	void item_status::set_data(const item_t& item_data, std::unique_ptr<player_data::player_data>& p_data)
	{
		const auto now = std::chrono::system_clock::now();

		this->max_second_ = item_data.dev_time;
		this->gmp_ = item_data.dev_gmp;

		const auto diff = now - this->create_date_;
		const auto seconds_passed = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

		if (this->created_)
		{
			this->left_second_ = static_cast<std::uint32_t>(std::max(0ull, (seconds_passed - static_cast<std::uint64_t>(this->max_second_))));
		}
		else
		{
			this->left_second_ = this->max_second_;
		}

		this->mb_coin_ = utils::tpp::calculate_mb_coins(this->left_second_);

		this->develop_ = this->left_second_ == 0 ? 2 : 0;
		this->open_ = this->left_second_ == 0;

		if (this->open_ && p_data.get())
		{
			const auto count_1 = p_data->get_resource_value(player_data::processed_server, item_data.dev_resource_ids[0]);
			const auto count_2 = p_data->get_resource_value(player_data::processed_server, item_data.dev_resource_ids[1]);
			const auto gmp = p_data->get_server_gmp();

			auto should_be_open = false;
			if (gmp >= item_data.dev_gmp && 
				count_1 >= item_data.dev_resource_values[0] && 
				count_2 >= item_data.dev_resource_values[1])
			{
				should_be_open = true;

				for (auto i = 0; i < player_data::unit_count; i++)
				{
					if (p_data->get_unit_level(i) < item_data.dev_platform_levels[i])
					{
						should_be_open = false;
						break;
					}
				}

				auto has_skill = false;
				if (should_be_open && item_data.dev_skill != 0)
				{
					for (auto i = 0u; i < p_data->get_staff_count(); i++)
					{
						const auto staff = p_data->get_staff(i);
						if (staff.header.skill == item_data.dev_skill)
						{
							has_skill = true;
							break;
						}
					}
				}

				if (!has_skill)
				{
					should_be_open = false;
				}
			}

			this->open_ = should_be_open;
		}
	}

	std::unordered_map<std::uint32_t, item_status> get_item_list(const std::uint64_t player_id)
	{
		auto results = database::get()->operator()(
			sqlpp::select(
				sqlpp::all_of(items_table))
					.from(items_table)
						.where(items_table.player_id == player_id));

		std::unordered_map<std::uint32_t, item_status> list;

		for (const auto& row : results)
		{
			item_status status(row);
			list.insert(std::make_pair(status.get_id(), status));
		}

		auto p_data = player_data::find(player_id);

		for (auto i = 0; i < player_data::resource_type_count; i++)
		{
			const auto count = p_data->get_resource_value(player_data::processed_server, i);
			printf("%i: %i\n", i, count);
		}

		const auto& static_list = get_static_list();
		for (const auto& item : static_list)
		{
			const auto iter = list.find(item.id);
			if (iter == list.end())
			{
				item_status status(item.id, player_id);
				status.set_data(item, p_data);
				list.insert(std::make_pair(item.id, status));
			}
			else
			{
				iter->second.set_data(item, p_data);
			}
		}

		return list;
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

REGISTER_TABLE(database::items::table, -1)
