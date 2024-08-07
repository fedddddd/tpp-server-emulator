#pragma once

#include "../database.hpp"

namespace database::player_records
{
	constexpr auto lowest_grade = 0;
	constexpr auto highest_grade = 11;

	class player_record
	{
	public:
		DEFINE_FIELD(id, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(fob_grade, sqlpp::integer_unsigned);
		DEFINE_FIELD(prev_fob_grade, sqlpp::integer_unsigned);
		DEFINE_FIELD(fob_point, sqlpp::integer);
		DEFINE_FIELD(fob_rank, sqlpp::integer_unsigned);
		DEFINE_FIELD(prev_fob_rank, sqlpp::integer_unsigned);
		DEFINE_FIELD(is_insurance, sqlpp::boolean);
		DEFINE_FIELD(league_grade, sqlpp::integer_unsigned);
		DEFINE_FIELD(prev_league_grade, sqlpp::integer_unsigned);
		DEFINE_FIELD(league_rank, sqlpp::integer_unsigned);
		DEFINE_FIELD(prev_league_rank, sqlpp::integer_unsigned);
		DEFINE_FIELD(league_point, sqlpp::integer_unsigned);
		DEFINE_FIELD(playtime, sqlpp::integer_unsigned);
		DEFINE_FIELD(point, sqlpp::integer_unsigned);
		DEFINE_FIELD(fob_defense_win, sqlpp::integer_unsigned);
		DEFINE_FIELD(fob_defense_lose, sqlpp::integer_unsigned);
		DEFINE_FIELD(fob_sneak_win, sqlpp::integer_unsigned);
		DEFINE_FIELD(fob_sneak_lose, sqlpp::integer_unsigned);
		DEFINE_FIELD(shield_date, sqlpp::time_point);
		DEFINE_TABLE(player_records, id_field_t, player_id_field_t, fob_grade_field_t, prev_fob_grade_field_t,
			fob_point_field_t, fob_rank_field_t, prev_fob_rank_field_t, is_insurance_field_t,
			league_grade_field_t, prev_league_grade_field_t, league_rank_field_t, prev_league_rank_field_t, league_point_field_t,
			playtime_field_t, point_field_t, fob_defense_win_field_t, fob_defense_lose_field_t, fob_sneak_win_field_t,
			fob_sneak_lose_field_t, shield_date_field_t);

		inline static table_t table;

		template <typename ...Args>
		player_record(const sqlpp::result_row_t<Args...>& row)
		{
			this->id_ = row.id;
			this->player_id_ = row.player_id;
			this->fob_grade_ = static_cast<std::uint32_t>(row.fob_grade);
			this->prev_fob_grade_ = static_cast<std::uint32_t>(row.prev_fob_grade);
			this->fob_point_ = static_cast<std::int32_t>(row.fob_point);
			this->fob_rank_ = static_cast<std::uint32_t>(row.fob_rank);
			this->prev_fob_rank_ = static_cast<std::uint32_t>(row.prev_fob_rank);
			this->is_insurance_ = static_cast<std::uint32_t>(row.is_insurance);
			this->league_grade_ = static_cast<std::uint32_t>(row.league_grade);
			this->prev_league_grade_ = static_cast<std::uint32_t>(row.prev_league_grade);
			this->league_rank_ = static_cast<std::uint32_t>(row.league_rank);
			this->prev_league_rank_ = static_cast<std::uint32_t>(row.prev_league_rank);
			this->league_point_ = static_cast<std::uint32_t>(row.league_point);
			this->playtime_ = static_cast<std::uint32_t>(row.playtime);
			this->point_ = static_cast<std::uint32_t>(row.point);
			this->fob_defense_win_ = static_cast<std::uint32_t>(row.fob_defense_win);
			this->fob_defense_lose_ = static_cast<std::uint32_t>(row.fob_defense_lose);
			this->fob_sneak_win_ = static_cast<std::uint32_t>(row.fob_sneak_win);
			this->fob_sneak_lose_ = static_cast<std::uint32_t>(row.fob_sneak_lose);
			this->shield_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.shield_date.value().time_since_epoch());
		}

		std::uint64_t get_player_id() const
		{
			return this->player_id_;
		}

		std::uint32_t get_fob_grade() const
		{
			return this->fob_grade_;
		}

		std::uint32_t get_prev_fob_grade() const
		{
			return this->prev_fob_grade_;
		}

		std::int32_t get_fob_point() const
		{
			return this->fob_point_;
		}

		std::uint32_t get_fob_rank() const
		{
			return this->fob_rank_;
		}

		std::uint32_t get_prev_fob_rank() const
		{
			return this->prev_fob_rank_;
		}

		std::uint32_t get_is_insurance() const
		{
			return this->is_insurance_;
		}

		std::uint32_t get_league_grade() const
		{
			return this->league_grade_;
		}

		std::uint32_t get_prev_league_grade() const
		{
			return this->prev_league_grade_;
		}

		std::uint32_t get_league_rank() const
		{
			return this->league_rank_;
		}

		std::uint32_t get_prev_league_rank() const
		{
			return this->prev_league_rank_;
		}

		std::uint32_t get_league_point() const
		{
			return this->league_point_;
		}

		std::uint32_t get_playtime() const
		{
			return this->playtime_;
		}

		std::uint32_t get_point() const
		{
			return this->point_;
		}

		std::uint32_t get_defense_win() const
		{
			return this->fob_defense_win_;
		}

		std::uint32_t get_defense_lose() const
		{
			return this->fob_defense_lose_;
		}

		std::uint32_t get_sneak_win() const
		{
			return this->fob_sneak_win_;
		}

		std::uint32_t get_sneak_lose() const
		{
			return this->fob_sneak_lose_;
		}

		std::int64_t get_shield_date() const
		{
			const auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
			if (now > this->shield_date_)
			{
				return 0u;
			}

			return this->shield_date_.count();
		}

		int is_shield_active() const
		{
			return this->get_shield_date() != 0;
		}

	private:
		std::uint64_t id_;
		std::uint64_t player_id_;
		std::uint32_t fob_grade_;
		std::uint32_t prev_fob_grade_;
		std::int32_t fob_point_;
		std::uint32_t fob_rank_;
		std::uint32_t prev_fob_rank_;
		std::uint32_t is_insurance_;
		std::uint32_t league_grade_;
		std::uint32_t prev_league_grade_;
		std::uint32_t league_rank_;
		std::uint32_t prev_league_rank_;
		std::uint32_t league_point_;
		std::uint32_t playtime_;
		std::uint32_t point_;
		std::uint32_t fob_defense_win_;
		std::uint32_t fob_defense_lose_;
		std::uint32_t fob_sneak_win_;
		std::uint32_t fob_sneak_lose_;

		std::chrono::seconds shield_date_;

	};

	std::optional<player_record> find(const std::uint64_t player_id);
	player_record find_or_create(const std::uint64_t player_id);
	void add_sneak_result(const std::uint64_t player_id, const std::uint64_t owner_id, 
		const std::int32_t point_add, const bool is_win, const bool is_sneak);
	void sync_prev_values(const std::uint64_t player_id);

	std::vector<player_record> find_players_of_grade(const std::uint64_t player_id, const std::uint32_t grade, const std::uint32_t limit);
	std::vector<player_record> find_same_grade_players(const std::uint64_t player_id, const std::uint32_t limit);
	std::vector<player_record> find_higher_grade_players(const std::uint64_t player_id, const std::uint32_t limit);

	void set_shield_date(const std::uint64_t player_id, const bool is_win);
	void clear_shield_date(const std::uint64_t player_id);
}
