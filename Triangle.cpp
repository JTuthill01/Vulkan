#include "Triangle.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = 
{ "VK_LAYER_KHRONOS_validation"};

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const
	VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const
	VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT*
	pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

Triangle::Triangle() : mWindow(nullptr), mInstance(nullptr), mDebugMessenger(nullptr)
{
}

Triangle::~Triangle()
{
}

void Triangle::run()
{
	initWindow();

	initVulkan();

	mainLoop();

	cleanUp();
}

void Triangle::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulcan Test", 
		nullptr, nullptr);
}

void Triangle::initVulkan()
{
	createInstance();

	setupDebugMessenger();
}

void Triangle::setupDebugMessenger()
{
	if (!enableValidationLayers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;

	populateDebugMessengerCreateInfo(createInfo);
	
	if (createDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr,
		&mDebugMessenger) != VK_SUCCESS)
		throw std::runtime_error("failed to setup debug messenger");
}

void Triangle::mainLoop()
{
	while (!glfwWindowShouldClose(mWindow))
		glfwPollEvents();
}

void Triangle::cleanUp()
{
	vkDestroyInstance(mInstance, nullptr);

	glfwDestroyWindow(mWindow);

	glfwTerminate();
}

void Triangle::createInstance()
{
	VkApplicationInfo appInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

	appInfo.pApplicationName = "Hello World Triangle";

	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	appInfo.pEngineName = "No Engine";

	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;

	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;

	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);

	if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance");

	if (enableValidationLayers)
	{
		createInfo.enabledExtensionCount =
			static_cast<uint32_t>(validationLayers.size());

		createInfo.ppEnabledLayerNames = validationLayers.data();
	}

	else
		createInfo.enabledLayerCount = 0;

	uint32_t extensionCount = 0;

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
		extensions.data());

	std::cout << "available extensions" << '\n';

	for (const auto& extension : extensions)
		std::cout << "\t" << extension.extensionName << '\n';

	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error("validation layers not available");
}

void Triangle::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};

	createInfo.sType =
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = debugCallback;
}

bool Triangle::checkValidationLayerSupport()
{
	uint32_t layerCount;

	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);

	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;

				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

std::vector<const char*> Triangle::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;

	const char** glfwExtensions;

	glfwExtensions =
		glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);


	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Triangle::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	VkDebugUtilsMessageTypeFlagsEXT messageType, 
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

