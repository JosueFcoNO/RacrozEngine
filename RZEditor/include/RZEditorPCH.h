#pragma once
#include <RZDirectXPCH.h>
#include <RZEnginePCH.h>
#include <RZRendererPCH.h>

#include "RZEditorCore.h"

#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN             // Excluir material rara vez utilizado de encabezados de Windows
// Archivos de encabezado de Windows:
#include <windows.h>
#endif