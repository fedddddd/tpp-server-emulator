#pragma once

namespace auth
{
	struct auth_ticket_response
	{
		std::string account_id;
		std::string currency;
		std::string password;
		std::string smart_device_id;
	};

	struct auth_response
	{
		bool success;
		std::uint64_t player_id;
		std::string session_id;
		std::string crypto_key;
		std::string smart_device_id;
	};

	void initialize_lists();
	void reload_lists();

	std::optional<auth_ticket_response> authenticate_user_with_ticket(const std::string& auth_ticket, const size_t ticket_size);
	std::optional<auth_response> authenticate_user(const std::string& account_id, const std::string& password);
}
