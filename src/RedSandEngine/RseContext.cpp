module;
#include <iostream>
module RedSandEngine:RseContext;

namespace rse
{
	void RseContext::onSdlAppIterate(void* appstate)
	{
		auto& current = _keyMap.current();
		auto& previous = _keyMap.previous();
		std::map<SDL_Keycode, Key> tempKeys{};
		std::swap(tempKeys, previous);
		if (tempKeys.size() > 0)
			int x = 6;
		for (auto& [keycode, key] : current)
		{
			if (tempKeys.count(keycode) > 0 && tempKeys[keycode].keyDirection == key.keyDirection)
				key.keyStatus = KeyStatus::directionHeld;
			previous.insert({ keycode, Key{ .keyDirection = key.keyDirection, .keyStatus = key.keyStatus } });
		}
	}

	void RseContext::onSdlAppInit(void** appstate, int argc, char* argv[])
	{
	}

	void RseContext::onSdlAppEvent(void* appstate, SDL_Event* event)
	{
		//std::cout << "app event" << std::endl;
		switch (event->type)
		{
		case SDL_EventType::SDL_EVENT_KEY_DOWN:
		{
			updateKey(event->key.key, KeyDirection::down);
			break;
		}
		case SDL_EventType::SDL_EVENT_KEY_UP:
		{
			updateKey(event->key.key, KeyDirection::up);
			break;
		}
		case SDL_EventType::SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			break;
		}
		case SDL_EventType::SDL_EVENT_MOUSE_BUTTON_UP:
		{

			break;
		}
		}
	}

	void RseContext::onSdlAppQuit(void* appstate, SDL_AppResult result)
	{
	}

	Key RseContext::getKey(SDL_Keycode keycode)
	{
		const auto& current = _keyMap.current();
		if (current.count(keycode) == 0)
			return Key{ .keyDirection = KeyDirection::up, .keyStatus = KeyStatus::directionHeld };
		return current.at(keycode);
	}

	void RseContext::updateKey(SDL_Keycode keycode, KeyDirection direction)
	{
		_keyMap.current()[keycode] = { .keyDirection = direction, .keyStatus = KeyStatus::directionBegin };
	}

	RseContext& rseContext()
	{
		static RseContext rseContext;
		return rseContext;
	}
}