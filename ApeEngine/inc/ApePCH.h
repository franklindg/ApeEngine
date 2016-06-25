#pragma once
#ifndef APE_PCH_H
#define APE_PCH_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <comdef.h>
#include <CommCtrl.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Windows runtime library
#include <wrl.h>

// Direct3D
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>
#pragma warning(disable: 4996)
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

// STL
#include <locale>
#include <codecvt>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include <random>
#include <atomic>
#include <mutex>
#include <thread>

// BOOST
#include <boost\uuid\uuid.hpp>
#include <boost\uuid\uuid_generators.hpp>

#include <boost\foreach.hpp>
#define foreach BOOST_FOREACH
#include <boost\math\special_functions\round.hpp>

#include <boost\filesystem.hpp>
#include <boost\filesystem\fstream.hpp>
namespace fs = boost::filesystem;

#include <boost\bind.hpp>
#include <boost\function.hpp>
#include <boost\signals2.hpp>
#include <boost\any.hpp>

#include <boost\archive\xml_iarchive.hpp>
#include <boost\archive\xml_oarchive.hpp>
#include <boost\archive\xml_wiarchive.hpp>
#include <boost\archive\xml_woarchive.hpp>
#include <boost\serialization\vector.hpp>

inline void ReportErrorAndThrow(const std::string& file, int line, const std::string& function, const std::string& message)
{
	std::stringstream ss;

	DWORD errorCode = GetLastError();
	LPTSTR errorMessage = nullptr;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorMessage, 0, nullptr);

	if (errorMessage)
	{
		ss << file << "(" << line << "): " << errorCode << ": " << errorMessage << std::endl;
		LocalFree(errorMessage);
	}
	else
	{
		ss << file << "(" << line << "): " << message << std::endl;
	}

	OutputDebugStringA(ss.str().c_str());
	MessageBoxA(nullptr, message.c_str(), function.c_str(), MB_ICONERROR);
	throw new std::exception(message.c_str());
}

#define ReportError(msg) ReportErrorAndThrow(__FILE__, __LINE__, __FUNCTION__, (msg))

template<typename T>
inline void SafeDelete(T& ptr)
{
	if (ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}

template<typename T>
inline void SafeDeleteArray(T& ptr)
{
	if (ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}

// Convert a multi-byte character string (UTF-8) to a wide (UTF-16) encoded string.
inline std::wstring ConvertString(const std::string& string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(string);
}

// Convert a wide (UTF-16) encoded string into a multi-byte (UTF-8) character string.
inline std::string ConvertString(const std::wstring& wstring)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstring);
}

//// Get a string resource from the module's resource.
//inline std::string GetStringResource(int ID, const std::string& type)
//{
//	HMODULE hModule = GetModuleHandle(nullptr);
//	HRSRC hResource = FindResourceA(hModule, MAKEINTRESOURCE(ID), type.c_str());
//	if (hResource)
//	{
//		HGLOBAL hResourceData = LoadResource(hModule, hResource);
//		DWORD resourceSize = SizeofResource(hModule, hResource);
//		if (hResourceData && resourceSize > 0)
//		{
//			const char* resourceData = static_cast<const char*>(LockResource(hResourceData));
//			std::string strData(resourceData, resourceSize);
//			return strData;
//		}
//	}
//	// Return an empty string.
//	return std::string();
//}

// Assimp
#include <assimp\DefaultLogger.hpp>
#include <assimp\Importer.hpp>
#include <assimp\Exporter.hpp>
#include <assimp\ProgressHandler.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\mesh.h>

//#pragma comment(lib, "assimp.lib")

// FreeImage
#include <FreeImage.h>
#define FREEIMAGE_LIB  // Static linking

//#pragma comment(lib, "FreeImage.lib")
#endif