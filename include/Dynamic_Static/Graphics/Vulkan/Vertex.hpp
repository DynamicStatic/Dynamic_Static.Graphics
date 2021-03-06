
#if 0
/*
==========================================
    Copyright (c) 2017 Dynamic_Static
    Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "Dynamic_Static/Graphics/Vulkan/Defines.hpp"
#include "Dynamic_Static/Graphics/Vulkan/Pipeline.hpp"

#include <array>

namespace Dynamic_Static {
namespace Graphics {
namespace Vulkan {

    /**
     * TODO : Documentation.
     */
    template <typename VertexType>
    inline VkVertexInputBindingDescription binding_description(uint32_t binding = 0)
    {
        VkVertexInputBindingDescription description;
        description.binding = binding;
        description.stride = sizeof(VertexType);
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return description;
    }

    /**
     * TODO : Documentation.
     */
    template <typename ...VertexAttributeTypes>
    inline std::array<VkVertexInputAttributeDescription, sizeof...(VertexAttributeTypes)>
        create_attribute_descriptions(uint32_t binding = 0)
    {
        size_t offset = 0;
        std::array<size_t, sizeof...(VertexAttributeTypes)> sizes { sizeof(VertexAttributeTypes)... };
        std::array<VkVertexInputAttributeDescription, sizeof...(VertexAttributeTypes)> descriptions { };
        for (size_t i = 0; i < descriptions.size(); ++i) {
            descriptions[i].binding = binding;
            descriptions[i].location = static_cast<uint32_t>(i);
            descriptions[i].offset = static_cast<uint32_t>(offset);
            switch (sizes[i]) {
                case sizeof(float) : descriptions[i].format = VK_FORMAT_R32_SFLOAT; break;
                case sizeof(glm::vec2) : descriptions[i].format = VK_FORMAT_R32G32_SFLOAT; break;
                case sizeof(glm::vec3) : descriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT; break;
                case sizeof(glm::vec4) : descriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT; break;
            }

            offset += sizes[i];
        }

        return descriptions;
    }

    /**
     * TODO : Documentation.
     */
    template <typename T, size_t Size>
    struct VertexAttribute final
    {
    };

    /**
     * TODO : Documentation.
     */
    template <typename ...VertexAttributeTypes>
    inline std::array<VkVertexInputAttributeDescription, sizeof...(VertexAttributeTypes)>
        create_attribute_descriptions_ex(uint32_t binding = 0)
    {
        // TODO : All uses of create_attribute_descriptions() need to be replaced with _ex().
        size_t offset = 0;
        std::array<size_t, sizeof...(VertexAttributeTypes)> sizes { sizeof(VertexAttributeTypes)... };
        std::array<VkVertexInputAttributeDescription, sizeof...(VertexAttributeTypes)> descriptions { };
        for (size_t i = 0; i < descriptions.size(); ++i) {
            descriptions[i].binding = binding;
            descriptions[i].location = static_cast<uint32_t>(i);
            descriptions[i].offset = static_cast<uint32_t>(offset);
            switch (sizes[i]) {
                case sizeof(float) : descriptions[i].format = VK_FORMAT_R32_SFLOAT; break;
                case sizeof(glm::vec2) : descriptions[i].format = VK_FORMAT_R32G32_SFLOAT; break;
                case sizeof(glm::vec3) : descriptions[i].format = VK_FORMAT_R32G32B32_SFLOAT; break;
                case sizeof(glm::vec4) : descriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT; break;
            }

            offset += sizes[i];
        }

        return descriptions;
    }

    /**
     * TODO : Documentation.
     */
    template <typename VertexType>
    inline auto attribute_descriptions(uint32_t binding = 0)
    {
        #if DYNAMIC_STATIC_MSVC
        // TODO : Why is this triggering for ImDrawVert on gcc?
        static_assert(false, "attribute_descriptions() must be specialized for the given vertex type.");
        #endif
    }

} // namespace Vulkan
} // namespace Graphics
} // namespace Dynamic_Static
#endif
