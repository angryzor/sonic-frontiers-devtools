#include "API.h"
#include "DebugCamera.h"
#include "GameManager.h"
#include "ObjectWorld.h"
#include "RenderManager.h"

namespace devtools::api {
	API::API(csl::fnd::IAllocator* allocator, const char* host, unsigned short port) : Module{ allocator }, httpThread{ [this, host, port]() {
		bool running{};
		auto app = uWS::App();

		context.app = &app;

		debug_camera::RegisterRoutes(context);
		game_manager::RegisterRoutes(context);
		object_world::RegisterRoutes(context);
		render_manager::RegisterRoutes(context);

		app.listen(host, port, [&running](auto* socket) {
			running = socket != nullptr;
		});

		loop = running ? app.getLoop() : nullptr;

		app.run();
	} }
	{
	}

	API::~API() {
		if (loop)
			loop->defer([this]() { context.app->close(); });
		httpThread.join();
	}

	unsigned int API::GetId() const {
		return id;
	}

	void API::Update() {
		context.ProcessPendingRequests();
	}
}
