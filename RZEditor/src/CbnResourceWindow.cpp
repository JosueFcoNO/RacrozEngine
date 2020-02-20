#include <RZEditorPCH.h>

namespace rczEngine
{
	void ResourceWindow::Render()
	{
		if (!m_ActiveResource.expired())
			m_ActiveResource.lock()->RenderResourceGUI();
	}
}