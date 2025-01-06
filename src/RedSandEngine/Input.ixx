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
		begin,
		held
	};

	struct Key
	{
		KeyDirection direction;
		KeyStatus status;
	};
}