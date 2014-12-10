#ifndef MPACK_GLOBAL_HPP
#define MPACK_GLOBAL_HPP

#include "Types.hpp"

#define HUD_LAYER		500.0f

class Camera2D;
class TextureMappedFont;
namespace Core
{
	class Context;
	class EventLoop;
};

namespace Global
{
#ifdef ANDROID_PLATFORM
	extern android_app 				*pAndroidApp;
	extern AAssetManager 			*pAAssetManager;
#endif

	extern Core::Context			*pContext;
	extern Core::EventLoop			*pEventLoop;

	extern TextureMappedFont		*pFont;

	extern Camera2D					*pActiveCamera;
}

#endif