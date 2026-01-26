#include "application/application.h"
#include "game/gameLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Architecture";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;

	Core::Application application(appSpec);
	application.PushLayer<GameLayer>();
	application.Run();
}