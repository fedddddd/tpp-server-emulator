#pragma once

#include "base_handler.hpp"
#include "command_handler.hpp"
#include "endpoint_handler.hpp"
#include "platform_handler.hpp"

#include "utils/http_server.hpp"

#include <utils/memory.hpp>

namespace tpp
{
	class server;

	struct response
	{
		std::string headers;
		std::string body;
	};

	using response_task = std::future<std::optional<response>>;

	class server : public base_handler<platform_handler>
	{
	public:
		server();

		bool start();
		void run_frame();

		void request_handler(const utils::http_connection& conn, const utils::request_params& params);

		utils::response_params handle_request(const utils::request_params& params, const std::string& platform, 
			const std::string& endpoint, const std::string& body);

	private:
		std::atomic_bool killed_ = false;
		utils::http_server http_server;

	};
}
