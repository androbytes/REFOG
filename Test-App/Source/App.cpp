#include "OpenGLTest.hpp"
#include "VulkanTest.hpp"

int main() {
	REFOG::Logger::Init();

	/*OpenGLTest testApp;
	testApp.Run();*/

	VulkanTest testApp;
	testApp.Run();

	return 0;
}