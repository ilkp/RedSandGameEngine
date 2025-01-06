module;
#include <iostream>
module RedSandEngine:RseContext;

namespace rse
{
	void RseContext::onSdlAppIterate(void* appstate)
	{
		updateKeyMaps();
	}

	void RseContext::onSdlAppInit(void** appstate, int argc, char* argv[])
	{
	}

	void RseContext::onSdlAppEvent(void* appstate, SDL_Event* event)
	{
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
			return Key{ .direction = KeyDirection::up, .status = KeyStatus::held };
		return current.at(keycode);
	}

	void RseContext::updateKeyMaps()
	{
		auto& current = _keyMap.current();
		auto& previous = _keyMap.previous();
		std::map<SDL_Keycode, Key> tempKeys{};
		std::swap(tempKeys, previous);
		for (auto& [keycode, key] : current)
		{
			if (tempKeys.count(keycode) > 0 && tempKeys[keycode].direction == key.direction)
				key.status = KeyStatus::held;
			previous.insert({ keycode, Key{.direction = key.direction, .status = key.status } });
		}
	}

	void RseContext::updateKey(SDL_Keycode keycode, KeyDirection direction)
	{
		_keyMap.current()[keycode] = { .direction = direction, .status = KeyStatus::begin };
	}

	RseContext& rseContext()
	{
		static RseContext rseContext;
		return rseContext;
	}
}