#pragma once

#include "types/command_handler.hpp"

namespace tpp
{
	class cmd_approve_steam_shop : public command_handler
	{
		nlohmann::json execute(nlohmann::json& data, const std::optional<database::players::player>& player) override;
	};
}
