module;
#include <SDL3/SDL.h>
#include <array>
#include <map>
export module RedSandEngine:RseContext;
import :Input;

export namespace rse
{
	class RseContext
	{
	public:
		void onSdlAppIterate(void* appstate);
		void onSdlAppInit(void** appstate, int argc, char* argv[]);
		void onSdlAppEvent(void* appstate, SDL_Event* event);
		void onSdlAppQuit(void* appstate, SDL_AppResult result);

		Key getKey(SDL_Keycode keycode);

	private:
		class KeyMap
		{
		public:
			std::map<SDL_Keycode, rse::Key>& current() { return _maps[0]; }
			std::map<SDL_Keycode, rse::Key>& previous() { return _maps[1]; }

		private:
			std::array<std::map<SDL_Keycode, rse::Key>, 2> _maps;
		};

		KeyMap _keyMap;

		void updateKey(SDL_Keycode keycode, KeyDirection direction);
	};

	RseContext& rseContext();
}