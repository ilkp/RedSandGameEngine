module;
#include <algorithm>
#include <functional>
module RedSandEngine:Input;

import :RseContext;

namespace rse
{
	Input::Input()
	{
		_onSdlEventCallbackIds.insert(RseContext::instance().addSdlEventCallback(
			SDL_EventType::SDL_EVENT_KEY_DOWN,
			std::bind(&Input::onSdlKeyChanged, this, std::placeholders::_1)
		));
		_onSdlEventCallbackIds.insert(RseContext::instance().addSdlEventCallback(
			SDL_EventType::SDL_EVENT_KEY_UP,
			std::bind(&Input::onSdlKeyChanged, this, std::placeholders::_1)
		));
		_onSdlIterateCallbackIds.insert(RseContext::instance().addSdlIterateCallback(
			std::bind(&Input::onSdlIterate, this)
		));
	}

	Key rse::Input::getKey(SDL_Keycode keycode)
	{
		const auto& current = _keyMap.current();
		if (current.count(keycode) == 0)
			return Key{ .direction = KeyDirection::up, .status = KeyStatus::held };
		return current.at(keycode);
	}

	void Input::updateKeyMaps()
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

	void Input::updateKey(SDL_Keycode keycode, KeyDirection direction)
	{
		_keyMap.current()[keycode] = { .direction = direction, .status = KeyStatus::begin };
	}

	void Input::onSdlKeyChanged(SDL_Event* event)
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
		}
	}

	void Input::onSdlIterate()
	{
		updateKeyMaps();
	}
}