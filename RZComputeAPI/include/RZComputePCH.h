#pragma once
#include <RZDirectXPCH.h>


#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#include <CL/cl_d3d11.h>

#include "RZForwardDecl.h"
#include "RZComputeEnums.h"
#include "RZComputeAPI.h"
#include "RZMemBuffer.h"
#include "RZProgram.h"
#include "RZKernel.h"
