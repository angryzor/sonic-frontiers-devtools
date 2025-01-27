#pragma once
#include <queue>
#include <future>
#include <App.h>
#include <sstream>
#include "Common.h"

namespace devtools::api {
	class APIContext {
		using Response = std::variant<ErrorResponse, rfl::Generic>;

		CRITICAL_SECTION mutex{};
		std::queue<std::packaged_task<Response()>> requests{};

		template<typename Res, typename F>
		inline void submitCall(Res* res, F&& f) {
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
					res->end(rfl::json::write(ErrorMessage{ .message = val.message }));
				}
				else {
					res->end(rfl::json::write(val));
				}
			}, future.get());
		}

	public:
		uWS::App* app;

		APIContext() {
			InitializeCriticalSection(&mutex);
		}

		~APIContext() {
			DeleteCriticalSection(&mutex);
		}

		inline void ProcessPendingRequests() {
			while (requests.size() > 0) {
				EnterCriticalSection(&mutex);
				auto t = std::move(requests.front());
				requests.pop();
				LeaveCriticalSection(&mutex);
				t();
			}
		}

		template<typename T, typename Res, typename F>
		inline void apiCallWithBody(Res* res, F f) {
			auto input = std::make_shared<std::ostringstream>();
			auto isAborted = std::make_shared<bool>();

			res->onData([this, input, isAborted, res, f](std::string_view chunk, bool isFin) {
				if (*isAborted)
					return;

				*input << chunk;

				if (!isFin)
					return;
			
				submitCall(res, [body = input->str(), f]() {
					auto parsedBody = rfl::json::read<T>(body);

					if (!parsedBody) {
						throw ErrorResponse{
							.status = "400 Bad Request",
							.message = parsedBody.error().value().what(),
						};
					}

					return f(parsedBody.value());
				});
			});
			res->onAborted([isAborted]() {
				*isAborted = true;
			});
		}

		template<typename Res, typename F>
		inline void apiCall(Res* res, F f) {
			submitCall(res, std::forward<F>(f));
		}

		template<typename T, typename Req>
		inline T getParameter(Req* req, const char* name) {
			T res{};
			std::istringstream iss{ std::string{ req->getParameter(name) } };
			iss >> res;
			return res;
		}
	};
}
