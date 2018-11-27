#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		struct RZ_EXP InputLayoutElement
		{
			String SemanticName;
			uint32 SemanticIndex;
			eFORMAT Format;
			uint32 InputSlot;
			uint32 AlignedByteOffset;
			eINPUT_CLASSIFICATION InputSlotClass;
			uint32 InstanceDataStepRate;
		};

		class VertexShader;

		class RZ_EXP InputLayout
		{
		public:
			~InputLayout() { Destroy(); };

			void Destroy();

			void AddElement(const char* SemanticName, uint32 SemanticIndex, eFORMAT Format, uint32 InputSlot,
				uint32 AlignedByteOffset, eINPUT_CLASSIFICATION InputSlotClass, uint32 InstanceDataStepRate);
			void ClearElementList();
			bool CreateInputLayout(VertexShader& VShader, GfxCore* gfx);
			uint32 GetElementSize();
			void SetThisInputLayout(GfxCore* gfx);

			ID3D11InputLayout* m_InputLayout;
			Vector<InputLayoutElement> m_VectorElements;
		};

	}
};