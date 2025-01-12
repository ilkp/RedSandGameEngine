module;
#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
export module RedSandEngine:Input;

import <map>;
import <array>;
import <set>;

export namespace rse
{
	enum class MouseButton : Uint8
	{
		left = static_cast<Uint8>(SDL_BUTTON_LEFT),
		middle = static_cast<Uint8>(SDL_BUTTON_MIDDLE),
		right = static_cast<Uint8>(SDL_BUTTON_RIGHT),
		x1 = static_cast<Uint8>(SDL_BUTTON_X1),
		x2 = static_cast<Uint8>(SDL_BUTTON_X2)
	};

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

	class KeyboardMap
	{
	public:
		std::map<SDL_Keycode, rse::Key>& current() { return _maps[0]; }
		std::map<SDL_Keycode, rse::Key>& previous() { return _maps[1]; }

	private:
		std::array<std::map<SDL_Keycode, rse::Key>, 2> _maps;
	};

	class MouseButtonMap
	{
	public:
		std::map<MouseButton, rse::Key>& current() { return _maps[0]; }
		std::map<MouseButton, rse::Key>& previous() { return _maps[1]; }

	private:
		std::array<std::map<MouseButton, rse::Key>, 2> _maps;
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
		Key getMouseButton(MouseButton button);
		void updateKeyboardMaps();
		void updateMouseMaps();

	private:
		std::set<int> _onSdlEventCallbackIds;
		std::set<int> _onSdlIterateCallbackIds;
		KeyboardMap _keyMap;
		MouseButtonMap _mouseMap;

		void onSdlKeyChanged(SDL_Event* event);
		void onSdlMouseButtonEvent(SDL_Event* event);
		void onSdlMouseMotionEvent(SDL_Event* event);
		void onSdlIterate();
	};
}