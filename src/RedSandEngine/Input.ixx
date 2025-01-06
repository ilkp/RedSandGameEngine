module;
#include <SDL3/SDL.h>
#include <map>
export module RedSandEngine:Input;

export namespace rse
{
	enum class KeyDirection
	{
		up,
		down
	};

	enum class KeyStatus
	{
		directionBegin,
		directionHeld
	};

	struct Key
	{
		KeyDirection keyDirection;
		KeyStatus keyStatus;
	};
}