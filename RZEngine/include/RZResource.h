#pragma once

namespace rczEngine
{
	class ResVault;

	///The resource base class. 
	class RZ_UTILITY_EXPORT Resource
	{
	public:
		virtual ~Resource() {};

		ResourceHandle GetHandle() { return m_Handle; };
		void SetHandle(ResourceHandle newHandle) { m_Handle = newHandle; };

		virtual void Load(const char* filePath, const char* resName, bool addToResourceManager) = 0;
		virtual void Release() = 0;

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI() = 0;
#endif

		String m_FilePath;
		String m_Name = "Unnamed Resource";

	protected:
		ResourceHandle m_Handle = -1;

	};
};