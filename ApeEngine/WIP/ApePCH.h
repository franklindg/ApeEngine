#pragma once


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <comdef.h>
#include <CommCtrl.h> // Windows controls
#include <mmsystem.h> // Joystick controls
#pragma comment(lib, "winmm.lib")

// Windows Runtime library
#include <wrl.h>

// DirectX specific headers.
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
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#include <boost/math/special_functions/round.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <boost/any.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/xml_woarchive.hpp>
#include <boost/serialization/vector.hpp>

template <typename T>
inline void SafeDelete(T& ptr)
{
	if (ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}

template <typename T>
inline void SafeDeleteArray(T& ptr)
{
	if (ptr != NULL)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

// ASSIMP
#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>

#pragma comment(lib, "assimp.lib")

// FreeImage
#define FREEIMAGE_LIB
#include <FreeImage.h>