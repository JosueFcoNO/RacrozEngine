#pragma once

namespace rczEngine
{
	class RZ_EXP ResourceWindow : public BaseWindow
	{
	public:

		FORCEINLINE void SetResource(WeakPtr<Resource> resource) { m_ActiveResource = resource; };

	protected:
		virtual void Render() override;

		WeakPtr<Resource> m_ActiveResource;
	};
}