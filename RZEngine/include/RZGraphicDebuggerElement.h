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
			//if (!m_Active) return;

			if (UseIndex)
			{
				gfx->SetPrimitiveTopology(Gfx::eTOPOLOGY::TOPO_LINELIST);

				m_VertexBuffer.SetThisVertexBuffer(gfx, 0);
				m_IndexBuffer.SetThisIndexBuffer(gfx);
			
				gfx->DrawIndexed(m_IndexBuffer.GetSize(), 0, 0);
			}
			else
			{
				gfx->SetPrimitiveTopology(Gfx::eTOPOLOGY::TOPO_LINESTRIP);

				m_VertexBuffer.SetThisVertexBuffer(gfx, 0);

				gfx->Draw(m_VertexBuffer.GetSize(), 0);
			}
		};

		void Update(float deltaTime)
		{
			m_TimeToExist -= deltaTime;
			if (m_TimeToExist < 0.0f && m_TimeToExist > -0.5f)
			{
				//Destroy();
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

		FORCEINLINE void Active(bool active) { m_Active = active; };

	protected:
		Color m_Color;
		float m_TimeToExist = 0.0f;
		bool m_Remove = false;

		bool m_Active = true;

		Gfx::VertexBuffer<Vector3> m_VertexBuffer;

		bool UseIndex = false;
		Gfx::IndexBuffer m_IndexBuffer;
	};
}