#pragma once

namespace rczEngine
{
	///A struct that equals the Camera Constant Buffer in the shaders.
	struct CameraData
	{
		Vector4 ViewPosition;
		Vector4 ViewDirection;
		Vector4 NearPlane;
		Vector4 FarPlane;
		Matrix4 ViewMatrix;
		Matrix4 ProjectionMatrix;
		Matrix4 PreviousViewMatrix;
		Matrix4 PreviousProjectionMatrix;
	};

	///A Camera Manager Class that holds pointers to all cameras in the Scene and can change between them.
	class RZ_UTILITY_EXPORT CameraManager
	{
	private:
		static CameraManager*& _Instance();

	public:
		static void Start();
		static CameraManager* Pointer();
		static void ShutDown();

		///Calls Destroy();
		~CameraManager() { Destroy(); };
		///Inits the Camera Manager and buffers
		void Init(Gfx::GfxCore* gfx);
		///Destroys the Camera Manager and all buffers.
		void Destroy();


		///Adds a Camera Cmp to the Manager
		ComponentId AddCamera(StrPtr<CameraCmp> cameraPtr);
		///Removes a Camera By Name
		void RemoveCamera(ComponentId id);
		///Returns a weak pointer to the active camera's component
		WeakPtr<CameraCmp> GetActiveCamera();


		///Sets the Active Camera By idd
		void SetActiveCamera(ComponentId id, Gfx::GfxCore* gfx);
		///Updates the Camera Buffer With the Active Camera's data and sets it on the VS and PS specified slots.
		void UpdateAndSetCameraBuffer(Gfx::GfxCore* gfx, int32 vertexShaderSlot, int32 PixelShaderSlot);

	private:
		///A Map of Camera Component Ptrs with String as key values
		Map<ComponentId, StrPtr<CameraCmp>> m_Cameras;
		///The Active Camera's pointer
		StrPtr<CameraCmp> m_ActiveCamera;
		///A constant Buffer to hold Camera Data
		Gfx::ConstantBuffer m_CameraBuffer;
		///The Camera's Data
		CameraData m_ActiveCameraData;
	};
}