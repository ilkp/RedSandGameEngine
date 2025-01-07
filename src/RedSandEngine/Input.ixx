module;
#include <SDL3/SDL.h>
export module RedSandEngine:Input;

import <map>;
import <array>;
import <set>;

export namespace rse
{
	enum class KeyDirection
	{
		up,
		down
	};

	enum class KeyStatus
	{
		begin,
		held
	};

	struct Key
	{
		KeyDirection direction;
		KeyStatus status;
	};

	class KeyMap
	{
	public:
		std::map<SDL_Keycode, rse::Key>& current() { return _maps[0]; }
		std::map<SDL_Keycode, rse::Key>& previous() { return _maps[1]; }

	private:
		std::array<std::map<SDL_Keycode, rse::Key>, 2> _maps;
	};

	class Input
	{
	public:
		Input();

		static Input& instance()
		{
			static Input input;
			return input;
		}

		Key getKey(SDL_Keycode keycode);
		void updateKeyMaps();

	private:
		std::set<int> _onSdlEventCallbackIds;
		std::set<int> _onSdlIterateCallbackIds;
		KeyMap _keyMap;

		void updateKey(SDL_Keycode keycode, KeyDirection direction);
		void onSdlKeyChanged(SDL_Event* event);
		void onSdlIterate();
	};
}