#pragma once
#include <modules/Module.h>
#include <thread>
#include <App.h>
#include "APIContext.h"

namespace devtools::api {
	class API : public Module {
	private:
		APIContext context{};
		std::thread httpThread;
		uWS::App* pApp{};
		uWS::Loop* loop{};
	
	public:
		static constexpr unsigned int id = 0x41504930;

		API(csl::fnd::IAllocator* allocator, const char* host, unsigned short port);
		~API();

		virtual unsigned int GetId() const override;
		virtual void Update() override;
	};
}
