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

		class InputLayout
		{
		public:
			RZ_EXP ~InputLayout() { Destroy(); };

			RZ_EXP void Destroy();

			RZ_EXP void AddElement(const String& SemanticName, uint32 SemanticIndex, eFORMAT Format, uint32 InputSlot,
				uint32 AlignedByteOffset, eINPUT_CLASSIFICATION InputSlotClass, uint32 InstanceDataStepRate);

			RZ_EXP void ClearElementList();
			RZ_EXP bool CreateInputLayout(VertexShader& VShader, GfxCore* gfx);
			RZ_EXP uint32 GetElementSize();

			RZ_EXP void SetThisInputLayout(GfxCore* gfx);

			ID3D11InputLayout* m_InputLayout;
			Vector<InputLayoutElement> m_VectorElements;

		private:
		};

	}
};