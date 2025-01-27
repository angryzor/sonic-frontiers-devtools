#pragma once
#include "Module.h"
#include <thread>
#include <queue>
#include <future>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <App.h>

class API : public Module, public hh::game::GameManagerListener {
public:
	struct ErrorResponse {
		std::string status{};
		std::string message{};
	};

private:
	struct ErrorJsonResponse {
		std::string message{};
	};

	using Response = std::variant<ErrorResponse, rfl::Generic>;

	hh::game::ObjectWorld* objectWorld;
	std::thread httpThread;
	uWS::App* pApp;
	uWS::Loop* loop;
	CRITICAL_SECTION mutex{};
	std::queue<std::packaged_task<Response()>> requests{};

	template<typename Res, typename F>
	void apiCall(Res* res, F&& f) {
		std::packaged_task<Response()> task{ [f]() -> Response {
			try {
				return rfl::generic::write(f());
			}
			catch (const ErrorResponse& err) {
				return err;
			}
		} };
		auto future = task.get_future();
		EnterCriticalSection(&mutex);
		requests.push(std::move(task));
		LeaveCriticalSection(&mutex);
		future.wait();

		std::visit([res](auto&& val) {
			using T = std::decay_t<decltype(val)>;
			if constexpr (std::is_same_v<T, ErrorResponse>) {
				res->writeStatus(val.status);
				res->end(rfl::json::write(ErrorJsonResponse{ .message = val.message }));
			}
			else {
				res->end(rfl::json::write(val));
			}
		}, future.get());
	}
	
public:
	static constexpr unsigned int id = 0x41504930;

	API(csl::fnd::IAllocator* allocator, const char* host, unsigned short port);
	~API();

	virtual unsigned int GetId() const override;
	virtual void Update() override;
	virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
	virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
};
