module;
#include <algorithm>
module RedSandEngine:RseContext;

import :RseTypes;
import :Input;

namespace rse
{
	RseContext::RseContext()
	{
	}

	uint64_t RseContext::addSdlEventCallback(SDL_EventType event, OnSdlEvent onSdlEvent)
	{
		if (_sdlEventCallbacks.count(event) == 0)
			_sdlEventCallbacks[event] = {};
		_sdlEventCallbacks[event].insert({ _sdlCallbackId++, std::move(onSdlEvent) });
		return _sdlCallbackId;
	}

	bool RseContext::removeSdlEventCallback(SDL_EventType event, int callbackId)
	{
		if (_sdlEventCallbacks.count(event) > 0 && _sdlEventCallbacks[event].count(callbackId) > 0)
		{
			_sdlEventCallbacks[event].erase(callbackId);
			return true;
		}
		return false;
	}

	uint64_t RseContext::addSdlIterateCallback(OnSdlIterate onSdlIterate)
	{
		_sdlIterateCallbacks.insert({ _sdlCallbackId++, std::move(onSdlIterate) });
		return _sdlCallbackId;
	}

	void RseContext::onSdlAppInit(void** appstate, int argc, char* argv[])
	{
	}

	bool RseContext::removeSdlIterateCallback(int callbackId)
	{
		if (_sdlIterateCallbacks.count(callbackId) > 0)
		{
			_sdlIterateCallbacks.erase(callbackId);
			return true;
		}
		return false;
	}

	void RseContext::onSdlAppIterate(void* appstate)
	{
		for (const auto& [id, callback] : _sdlIterateCallbacks)
			callback();
	}

	void RseContext::onSdlAppEvent(void* appstate, SDL_Event* event)
	{
		const SDL_EventType eventType = static_cast<SDL_EventType>(event->type);
		if (_sdlEventCallbacks.count(eventType) > 0)
			for (const auto& [id, callback] : _sdlEventCallbacks.at(eventType))
				callback(event);
	}

	void RseContext::onSdlAppQuit(void* appstate, SDL_AppResult result)
	{
	}
}