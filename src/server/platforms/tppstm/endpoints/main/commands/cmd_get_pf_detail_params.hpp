#pragma once

#include "types/command_handler.hpp"

namespace tpp
{
	class cmd_get_pf_detail_params : public command_handler
	{
		nlohmann::json execute(nlohmann::json& data, const std::optional<database::players::player>& player) override;
	};
}
