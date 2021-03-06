#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOMINMAX
#include <Windows.h>

#include <set>
#include <map>
#include <string>
#include <tuple>
#include <array>
#include <random>
#include <memory>
#include <sstream>
#include <fstream>
#include <optional>
#include <exception>
#include <functional>
#include <filesystem>
#include <unordered_map>

#include "resource.h"

#include <wrl.h>
#include <d3d11.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#ifdef _DEBUG
#include <DXErr.h>
#pragma comment(lib, "DxErr.lib")
#endif

#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")

#include <DirectXMath.h>

#include "Engine/Exception.h"
#include "Graphic/DXGIInfoQueue.h"
#include "UI/Imgui/imgui.h"
#include "UI/Imgui/imgui_impl_dx11.h"
#include "UI/Imgui/imgui_impl_win32.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc141-mtd.lib")
#pragma comment(lib, "zlibstaticd.lib")
#pragma comment(lib, "IrrXMLd.lib")
#else
#pragma comment(lib, "assimp-vc141-mt.lib")
#pragma comment(lib, "zlibstatic.lib")
#pragma comment(lib, "IrrXML.lib")
#endif

template<typename Type> using ScopedPointer = std::unique_ptr<Type>;
template<typename Type> using SharedPointer = std::shared_ptr<Type>;

using HID = size_t;

inline static HID CreateObjectID(const std::string& Value) 
{
	return std::hash<std::string>{}(Value);
}

template<typename Type, typename ... Arg>
constexpr ScopedPointer<Type> CreateScopedPointer(Arg&& ... Args)
{
	return std::make_unique<Type>(std::forward<Arg>(Args)...);
}

template<typename Type, typename ... Arg>
constexpr SharedPointer<Type> CreateSharedPointer(Arg&& ... Args)
{
	return std::make_shared<Type>(std::forward<Arg>(Args)...);
}

#define H_UNUSED(x) \
static_cast<void>((x));

#define H_ERROR(x) \
throw Howlite::HException{ __FILE__, __LINE__, (HRESULT)x, Howlite::EExceptionType::Platform }; 

#define H_ASSERT(x, ErrorMessage) \
{ \
if(!(x)) \
{ \
throw Howlite::HException{ __FILE__, __LINE__, ErrorMessage }; \
} \
}

#ifdef _DEBUG
#define H_DXGI_INFOQUEUE(GraphicSystem) \
Howlite::HDXGIInfoQueue& DXGIInfoQueue = GetDXGIInfoQueue(GraphicSystem);

#define H_DX_SAFECALL(x) \
{ \
DXGIInfoQueue.ClearMessageQueue(); \
HRESULT hresult = (x); \
if(FAILED(hresult)) \
{ \
std::string message; \
for(const std::string& line : DXGIInfoQueue.GetMessageQueue()) \
{ \
message += line; \
} \
throw Howlite::HException{ __FILE__, __LINE__, hresult, Howlite::EExceptionType::Graphic, message }; \
} \
}

#else
#define H_DXGI_INFOQUEUE(GraphicSystem)

#define H_DX_SAFECALL(x) \
{ \
HRESULT hresult = (x); \
if(FAILED(hresult)) \
{ \
throw Howlite::HException{ __FILE__, __LINE__, hresult, Howlite::EExceptionType::Graphic }; \
} \
}

#endif

#define H_TYPENAME(x) #x

#define H_GENERATE_EVENT(x) \
static inline EEventType GetStaticEventType() noexcept { return x; } \
inline EEventType GetEventType() const noexcept override { return GetStaticEventType(); } \
inline const char* GetEventName() const noexcept override { return #x; } 

#define H_BIND_EVENT(x) \
std::bind(&x, this, std::placeholders::_1)

