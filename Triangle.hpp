#pragma once
#define GLFW_INCLUDE_VULKAN

#ifdef NDEBUG
const bool enableValidationLayers = false;

#else
const bool enableValidationLayers = true;
#endif // NDEBUG


#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

class Triangle
{
public:
	Triangle();
	~Triangle();

	void run();

private:
	void initWindow();
	void initVulkan();
	void setupDebugMessenger();
	void mainLoop();
	void cleanUp();
	void createInstance();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	GLFWwindow* mWindow;

	VkInstance mInstance;
	VkDebugUtilsMessengerEXT mDebugMessenger;
};

