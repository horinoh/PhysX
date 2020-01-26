#pragma once

#include "resource.h"

#ifdef _DEBUG
//#define USE_PVD
#endif

#define PX_RELEASE(x) if(nullptr != x) { x->release(); x=nullptr; }
#ifdef USE_PVD
#define PVD_HOST "127.0.0.1"
#define PVD_FILE "PVD"
#endif

#include "PxPhysicsAPI.h"

#include "vehicle/PxVehicleSDK.h"

//#include "extensions/PxDefaultErrorCallback.h"
//#include "extensions/PxDefaultCpuDispatcher.h"
//#include "extensions/PxDefaultSimulationFilterShader.h"
//#include "extensions/PxExtensionsAPI.h"

//#ifdef USE_PVD
//#include "pvd/PxPvd.h"
//#include "pvd/PxPvdTransport.h"
//#endif

//!< PxDefaultAllocator‚àextension‚¾‚ªA‚±‚ê‚ÍŽg‚¦‚é
physx::PxDefaultAllocator Allocator;
#if 0
//!< PxDefaultErrorCallback‚Íextension
physx::PxDefaultErrorCallback ErrorCallback;
#else
class MyErrorCallback : public physx::PxErrorCallback
{
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override {}
};
MyErrorCallback ErrorCallback;
#endif

physx::PxFoundation* Foundation = nullptr;
physx::PxPvd* Pvd = nullptr;
physx::PxPhysics* Physics = nullptr;

physx::PxDefaultCpuDispatcher* Dispatcher = nullptr;
physx::PxScene* Scene = nullptr;

physx::PxMaterial* Material = nullptr;

