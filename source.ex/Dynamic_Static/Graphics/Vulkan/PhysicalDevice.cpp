
/*
==========================================
  Copyright (c) 2016-2018 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#include "Dynamic_Static/Graphics/Vulkan/PhysicalDevice.hpp"
#include "Dynamic_Static/Graphics/Vulkan/Instance.hpp"

namespace Dynamic_Static {
namespace Graphics {
namespace Vulkan {

    PhysicalDevice::PhysicalDevice(
        Instance* instance,
        VkPhysicalDevice handle
    )
        : InstanceChild(instance)
    {
        mHandle = handle;
        vkGetPhysicalDeviceFeatures(mHandle, &mFeatures);
        vkGetPhysicalDeviceProperties(mHandle, &mProperties);
        set_name(mProperties.deviceName);
        vkGetPhysicalDeviceMemoryProperties(mHandle, &mMemoryProperties);
        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount, nullptr);
        mQueueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount, mQueueFamilyProperties.data());
    }

    const VkPhysicalDeviceFeatures& PhysicalDevice::get_features() const
    {
        return mFeatures;
    }

    const VkPhysicalDeviceProperties& PhysicalDevice::get_properties() const
    {
        return mProperties;
    }

    const VkPhysicalDeviceMemoryProperties& PhysicalDevice::get_memory_properties() const
    {
        return mMemoryProperties;
    }

    std::vector<uint32_t> PhysicalDevice::get_queue_families(VkQueueFlags queueFlags) const
    {
        std::vector<uint32_t> queueFamilyIndices;
        for (uint32_t i = 0; i < mQueueFamilyProperties.size(); ++i) {
            if (mQueueFamilyProperties[i].queueFlags & queueFlags &&
                mQueueFamilyProperties[i].queueCount > 0) {
                queueFamilyIndices.push_back(i);
            }
        }
        return queueFamilyIndices;
    }

} // namespace Vulkan
} // namespace Graphics
} // namespace Dynamic_Static
