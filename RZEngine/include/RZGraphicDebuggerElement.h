#pragma once

namespace rczEngine
{
	enum eDEBUG_ELEMENT
	{
		DEBUG_LINE,
		DEBUG_POINT,
		DEBUG_CUBE
	};

	class RZ_EXP GraphicDebuggerElement
	{
	public:
		void Render(Gfx::GfxCore* gfx)
		{
			m_VertexBuffer.SetThisVertexBuffer(gfx, 0);

			gfx->Draw(m_VertexBuffer.GetSize(), 0);
		};

		void Update(float deltaTime)
		{
			m_TimeToExist -= deltaTime;
			if (m_TimeToExist < 0.0f && m_TimeToExist > -0.5f)
			{
				Destroy();
			}
		}

		void Destroy()
		{
			m_VertexBuffer.Destroy();

			m_Remove = true;
		}

		virtual uint8 GetElementType() = 0;

		FORCEINLINE bool CanRemove() const noexcept { return m_Remove; }

		FORCEINLINE void SetTimeToExist(float time) noexcept { m_TimeToExist = time; };

		FORCEINLINE void SetColor(const Color& color) noexcept { m_Color = color; };
		FORCEINLINE const Color& GetColor() const noexcept { return m_Color; };

	protected:
		Color m_Color;
		float m_TimeToExist = 0.0f;
		bool m_Remove = false;

		Gfx::VertexBuffer<Vector3> m_VertexBuffer;
	};
}