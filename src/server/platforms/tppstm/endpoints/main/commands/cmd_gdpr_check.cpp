#include <std_include.hpp>

#include "cmd_gdpr_check.hpp"

#include <utils/nt.hpp>

namespace tpp
{
	cmd_gdpr_check::cmd_gdpr_check()
	{
		this->list_ = resource(RESOURCE_GDPR_CHECK);
	}

	nlohmann::json cmd_gdpr_check::execute(nlohmann::json& data, const std::optional<database::players::player>& player)
	{
		return this->list_;
	}
}
