#pragma once
#define GLFW_INCLUDE_VULKAN
#include "lve_window.hpp"
#include "vulkan/vulkan.h"
#include "glfw/glfw3.h"
#include <vector>
#include <array>
#include "stdexcept"
#include "iostream"
#include <fstream>
#include "log.h"
#include "glm/glm.hpp"

#include "optional"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::string MODEL_PATH = "models/viking_room.obj";
const std::string TEXTURE_PATH = "textures/viking_room.png";


const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif // NDEBUG

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	//可以使用比较运算符来过滤处理一定级别以上的调试信息
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
	}

	return VK_FALSE;
}

static std::vector<char>readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
		return attributeDescriptions;
	}

	bool operator==(const Vertex& other)const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}
namespace lve {

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	//const std::vector<Vertex> vertices = {
	//	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	//	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	//	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	//};
	/*const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};*/
	//const std::vector<Vertex> vertices = {
	//{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	//{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	//{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	//{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	//};
	//const std::vector<Vertex> vertices = {
	//{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	//{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	//{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	//{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	//};
//	const std::vector<Vertex> vertices = {
//{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
//
//{ { -0.5f, -0.5f, -0.5f }, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
//{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
//	};
//	const std::vector<uint16_t> indices = {
//		0, 1, 2, 2, 3, 0,
//		4, 5, 6, 6, 7, 4
//	};


	class FirstApp {
	public:

		void run();

	private:
		void initVulkan();
		void initWindow();
		void mainLoop();
		void createInstance();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSurface();
		void createSwapChain();
		void recreateSwapChain();
		void createImageViews();
		void createGraphicsPipeline();
		VkShaderModule createShaderModule(const std::vector<char>& code);
		void createRenderPass();
		void createFramebuffers();
		void createCommandPool();
		void createDepthResources();
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags propertices, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		/// <summary>
		/// 缓冲之间复制对象
		/// </summary>
		/// <param name="srcBuffer"></param>
		/// <param name="destBuffer"></param>
		/// <param name="size"></param>
		void copyBuffer(VkBuffer srcBuffer, VkBuffer destBuffer, VkDeviceSize size);
		void createDescriptorSetLayout();
		void loadModel();
		void createVertexBuffer();
		void createIndexBuffer();
		void createUniformBuffers();
		void createCommandBuffers();
		void createDescriptorPool();
		void createTextureImage();
		void createTextureImageView();
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,  uint32_t mipLevels);
		void createTextureSampler();

		void createDescriptorSets();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void createSyncObjects();
		void updateUniformBuffer(uint32_t currentImage);
		void drawFrame();
		void cleanupSwapChain();
		void cleanup();

		bool checkValidationLayerSupport();
		void setupDebugMessenger();
		std::vector<const char*> getRequiredExtensions();
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags porperties);
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	private:
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void transitionImageLayout(VkImage image, VkFormat format,
			VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat() {
			return findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		}


		bool hasStencilComponent(VkFormat format) {
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		}
		void generateMipmaps(VkImage iamge,VkFormat imageFormat, int32_t texWidth,int32_t texHeight,uint32_t mipLevel);
	public:
		bool framebufferResized = false;
	private:
		VkInstance instance;
		GLFWwindow* window;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkSurfaceKHR surface;
		VkQueue presentQueue;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		VkRenderPass renderPass;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		size_t currentFrame = 0;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;
		uint32_t mipLevels;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;
		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	
		
	};
}