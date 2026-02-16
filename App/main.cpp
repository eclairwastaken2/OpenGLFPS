#include "application/application.h"
#include "game/gameLayer.h"
#include <gl/shaderManager.h>

int main()
{
	ShaderManager::initialize();
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Tes5";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;

	Core::Application application(appSpec);
	application.PushLayer<GameLayer>();
	application.Run();
}