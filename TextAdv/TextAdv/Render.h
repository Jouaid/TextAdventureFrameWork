#ifndef _RENDER_H_
#define _RENDER_H_
#include"Process.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include <algorithm>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <set>
#include <cstdint>
#include <array>

struct SwapChainSupportDetails;
struct QueueFamilyIndices;

//Defining the UBO
struct UniformBufferObject
{
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

struct Vertex
{
  glm::vec2 pos;
  glm::vec3 color;

  //Tell vulkan how to pass data format into vertex shader


  static VkVertexInputBindingDescription getBindingDescription()
  {
    VkVertexInputBindingDescription bindingDescription = {};
    //Only have 1 binding because we are using one array of data
    bindingDescription.binding = 0;
    //Number of bytes from one entru to the next
    bindingDescription.stride = sizeof(Vertex);
    //Can have 2 values move to the next data entry after each vertex or instace
    //We are using after each vertex
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
  {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

    //Tells vulkan which binding the per-vertex data comes
    attributeDescriptions[0].binding = 0;
    //references the location directive of the input in the vertex shader
    attributeDescriptions[0].location = 0;
    //Type of data
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    //number of bytes from the beginning of this struct
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
  }
};

//interleaving vertex attributes
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

class Render : public Process
{
private:
  GLFWwindow* window;
  int WIDTH = 800;
  int HEIGHT = 600;
#pragma region Validation Layer

#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

  bool CheckValidationLayerSupport();
  std::vector<const char*> GetRequiredExtensions();

#pragma endregion

#pragma region Debugger

  VkDebugUtilsMessengerEXT debugMessenger;


  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                      VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
  {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
  }
  void SetupDebugMessenger();
  VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
  void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                     const VkAllocationCallbacks* pAllocator);

#pragma endregion

#pragma region GPU

  //Stores the GPU that we are going to use
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  void PickPhysicalDevice();
  int RateDeviceSuitability(VkPhysicalDevice device);

  QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

#pragma endregion

#pragma region Logical Device

  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  void CreateLogicalDevice();

#pragma endregion

#pragma region Swap Chain


  SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkImageView> swapChainImageViews;

  std::vector<VkFramebuffer> swapChainFramebuffers;

  void CreateImageViews();

  void CreateSwapChain();

  void CreateFramebuffers();

  void RecreateSwapChain();

  void CleanupSwapChain();
#pragma endregion

  //Command buffers
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;
  void CreateCommandPool();
  void CreateCommandBuffers();

  //LOADING SHADERs
  static std::vector<char> readFile(const std::string& filename);
  VkShaderModule createShaderModule(const std::vector<char>& code);
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  void CreateGraphicsPipeline();
  void CreateRenderPass();

  //VERTEX BUFFERS
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  void CreateVertexBuffer();
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                    VkBuffer& buffer, VkDeviceMemory& bufferMemory);
  void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  //Index Buffer
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  void CreateIndexBuffer();

  //Uniform buffer
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  VkDescriptorSetLayout descriptorSetLayout;
  void CreateDescriptorSetLayout();
  void CreateUniformBuffers();
  void UpdateUniformBuffer(uint32_t currentImage);
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;
  void CreateDescriptorPool();
  void CreateDescriptorSets();

  //For Windows
  VkSurfaceKHR surface;
  void CreateSurface();

  //Making an instance
  VkInstance instance;
  void CreateInstance();

  bool IsDeviceSuitable(VkPhysicalDevice device);

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
  size_t currentFrame = 0;
  bool framebufferResized = false;

  static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
  {
    auto app = reinterpret_cast<Render*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
  }

  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderFinishedSemaphore;

  const int MAX_FRAMES_IN_FLIGHT = 2;


  void CreateSemaphores();
  void CreateSyncObjects();

  void DrawFrame();


public:
  void Initialise() override;
  void Abort()override;
  ProcessState Update(float deltaTime = 0)override;
  void InitWindow();
  void  InitVulkan();
  void CleanUp();
};

//GPU struct
struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool IsComplete()
  {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

//Swap Chain struct
struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};
#endif