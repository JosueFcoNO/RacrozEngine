#pragma once
#include "RZMacroUtil.h"
#include "RZPlatformDefines.h"
#include "RZPlatformTypes.h"
#include "RZFwdDeclUtil.h"
#include "RZStdHeaders.h"
#include "RZMacroUtil.h"
#include "RZContainers.h"
#include "RZMemoryAllocators.h"

#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif RZ_PLATFORM == RZ_PLATFORM_LINUX

#endif

#include "CbnOSDefs.h"
#include "CbnWindow.h"
#include "CbnOSLayer.h"

#include "RZParser.h"

#include "RZFileCoder.h"

#include "RZSerializable.h"
#include "RZSerializer.h"

#include "RZLogObject.h"
#include "RZLogger.h"

#include "RZRadian.h"
#include "RZDegree.h"

#include "RZVector2Di.h"
#include "RZVector2D.h"
#include "RZVector3D.h"
#include "RZVector4D.h"
#include "RZQuaternion.h"

#include "RZMatrix3D.h"
#include "RZMatrix4D.h"

#include "RZAABB.h"
#include "RZRect2D.h"
#include "RZRect2Di.h"
#include "RZRect3D.h"
#include "RZLine.h"
#include "RZPlane.h"
#include "RZSphere.h"
#include "RZShapes3D.h"
#include "RZTriangle.h"

#include "CbnLine2D.h"

#include "RZFrustum.h"
#include "RZCamera.h"

#include "RZMath.h"

#include "RZColor.h"

#include "RZTimer.h"

#include "RZRandomNumberGen.h"
#include "RZRandomZiggurat.h"
#include "RZBoxMuller.h"

#include "RZTextTool.h"

#include "RZProfilerEvent.h"
#include "RZProfiler.h"
#include "RZProfilerObj.h"

#include "RZMemoryProfilerEvent.h"
#include "RZMemoryProfiler.h"

#include "RZPath.h"
