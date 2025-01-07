module;
#include <SDL3/SDL.h>
export module RedSandEngine:RseContext;

import :RseTypes;
import :Input;

import <unordered_map>;
import <set>;
import <memory>;
import <cstdint>;

export namespace rse
{
	class RseContext
	{
	public:
		RseContext();

		static RseContext& instance()
		{
			static RseContext rseContext;
			return rseContext;
		}

		uint64_t addSdlEventCallback(SDL_EventType event, OnSdlEvent onSdlEvent);
		bool removeSdlEventCallback(SDL_EventType event, int callbackId);
		uint64_t addSdlIterateCallback(OnSdlIterate onSdlIterate);
		bool removeSdlIterateCallback(int callbackId);

		void onSdlAppInit(void** appstate, int argc, char* argv[]);
		void onSdlAppIterate(void* appstate);
		void onSdlAppEvent(void* appstate, SDL_Event* event);
		void onSdlAppQuit(void* appstate, SDL_AppResult result);

	private:
		uint64_t _sdlCallbackId = 0;
		std::unordered_map<SDL_EventType, std::unordered_map<int, OnSdlEvent>> _sdlEventCallbacks;
		std::unordered_map<int, OnSdlIterate> _sdlIterateCallbacks;
	};
}