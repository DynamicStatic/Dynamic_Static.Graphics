
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "dynamic_static/core/string.hpp"
#include "dynamic_static/cpp-generator.hpp"
#include "dynamic_static/vk-xml-parser.hpp"

#include <set>
#include <string_view>

namespace dst {
namespace vk {
namespace cppgen {

#if 0
inline bool vk_handle_requires_manual_implementation(const std::string& structureName)
{
    static const std::set<std::string> sHandlesRequiringManualImplementation {
        "VkCommandBuffer",
        "VkDeferredOperationKHR",
        "VkDescriptorSet",
        "VkInstance",
    };
    return sHandlesRequiringManualImplementation.count(structureName);
}
#endif

inline std::string strip_vk(const std::string& str)
{
    return string::remove(string::remove(string::remove(str, "VK_"), "Vk"), "vk");
}

inline std::vector<std::string> get_custom_handle_ctors(const std::string& handleType)
{
    static const std::unordered_map<std::string, std::vector<std::string>> sCustomHandleCtors {
        #if 0
        { "VkImageView", { "VkResult create(const Managed<VkImage>& image, const VkAllocationCallbacks* pAllocator, Managed<VkImageView>* pImageView)" }},
        #endif
    };
    auto itr = sCustomHandleCtors.find(handleType);
    return itr != sCustomHandleCtors.end() ? itr->second : std::vector<std::string> { };
}

inline std::vector<std::string> get_custom_handle_fields(const std::string& handleType)
{
    static const std::unordered_map<std::string, std::vector<std::string>> sCustomHandleFields {
        { "VkInstance", { "std::vector<Managed<VkPhysicalDevice>>" }},
        { "VkDevice", { "std::vector<Managed<VkQueue>>" }},
        { "VkQueue", { "Managed<VkDeviceQueueCreateInfo>" }},
        { "VkSwapchainKHR", { "std::vector<Managed<VkImage>>" }},
        { "VkBuffer", { "Managed<VkDeviceMemory>", "Managed<VkBindBufferMemoryInfo>" }},
        { "VkImage", { "Managed<VkSwapchainKHR>", "Managed<VkDeviceMemory>", "Managed<VkBindImageMemoryInfo>" }},
        { "VkBufferView", { "Managed<VkBuffer>" }},
        { "VkImageView", { "Managed<VkImage>" }},
        { "VkFramebuffer", { "std::vector<Managed<VkImageView>>" }},
    };
    auto itr = sCustomHandleFields.find(handleType);
    return itr != sCustomHandleFields.end() ? itr->second : std::vector<std::string> { };
}

inline const std::set<std::string>& get_manually_implemented_structures(const xml::Manifest& xmlManifest)
{
    static const std::set<std::string> sManuallyImplementedStructures =
    [&xmlManifest]()
    {
        std::set<std::string> manuallyImplementedStructures {
            // Structures with special case arrays
            "VkPipelineMultisampleStateCreateInfo",
            "VkShaderModuleCreateInfo",
            "VkTransformMatrixKHR",

            // Unions
            "VkClearColorValue",
            "VkClearValue",
            "VkPerformanceCounterResultKHR",
            "VkPerformanceValueDataINTEL",
            "VkPipelineExecutableStatisticValueKHR",

            // Non pNext void*
            "VkDebugMarkerObjectTagInfoEXT",
            "VkDebugUtilsObjectTagInfoEXT",
            "VkPipelineCacheCreateInfo",
            "VkPipelineExecutableInternalRepresentationKHR",
            "VkSpecializationInfo",
            "VkValidationCacheCreateInfoEXT",
            "VkWriteDescriptorSetInlineUniformBlockEXT",
        };
        for (auto structureItr : xmlManifest.structures) {
            const auto& structure = structureItr.second;
            static const std::string VkAccelerationPrefix = "VkAcceleration";
            if (structure.alias.empty() &&
                VkAccelerationPrefix.size() < structure.name.size() &&
                std::string_view(structure.name.c_str(), VkAccelerationPrefix.size()) == VkAccelerationPrefix) {
                manuallyImplementedStructures.insert(structure.name);
            }
        }
        return manuallyImplementedStructures;
    }();
#if 0
        // Structures with special case arrays
        "VkPipelineMultisampleStateCreateInfo",
        "VkShaderModuleCreateInfo",
        "VkTransformMatrixKHR",

        // Unions
        "VkClearColorValue",
        "VkClearValue",
        "VkPerformanceCounterResultKHR",
        "VkPerformanceValueDataINTEL",
        "VkPipelineExecutableStatisticValueKHR",

        // Non pNext void*
        "VkDebugMarkerObjectTagInfoEXT",
        "VkDebugUtilsObjectTagInfoEXT",
        "VkPipelineCacheCreateInfo",
        "VkPipelineExecutableInternalRepresentationKHR",
        "VkSpecializationInfo",
        "VkValidationCacheCreateInfoEXT",
        "VkWriteDescriptorSetInlineUniformBlockEXT",
    };
#endif

    #if 0
    // TODO : Look into these...
    "VkAllocationCallbacks",
    "VkCheckpointDataNV",
    "VkDebugMarkerObjectTagInfoEXT",
    "VkDebugReportCallbackCreateInfoEXT",
    "VkDebugUtilsMessengerCreateInfoEXT",
    "VkDebugUtilsObjectTagInfoEXT",
    "VkImportMemoryHostPointerInfoEXT",
    "VkInitializePerformanceApiInfoINTEL",

    static const std::set<std::string> sManuallyImplementedStructures {
        // Structures with special case arrays
        "VkPipelineMultisampleStateCreateInfo",
        "VkShaderModuleCreateInfo",
        "VkTransformMatrixKHR",

        // Unions
        "VkClearColorValue",
        "VkClearValue",
        "VkPerformanceCounterResultKHR",
        "VkPerformanceValueDataINTEL",
        "VkPipelineExecutableStatisticValueKHR",

        // Non pNext void*
        "VkDebugMarkerObjectTagInfoEXT",
        "VkDebugUtilsObjectTagInfoEXT",
        "VkPipelineCacheCreateInfo",
        "VkPipelineExecutableInternalRepresentationKHR",
        "VkSpecializationInfo",
        "VkValidationCacheCreateInfoEXT",
        "VkWriteDescriptorSetInlineUniformBlockEXT",
    };
#endif
    return sManuallyImplementedStructures;
}

inline bool is_manually_implemented(const xml::Manifest& xmlManifest, const xml::Structure& structure)
{
#if 0
    if (get_manually_implemented_structures().count(structure.name)) {
        return true;
    }
    // TODO : I'm not exactly sure why most of the VkAcceleration structures have a
    //  bunch of problems with to_tuple<>().  Maybe an issued with xml parsing?
    static const std::string VkAccelerationPrefix = "VkAcceleration";
    if (VkAccelerationPrefix.size() < structure.name.size()) {
        auto b = std::string_view(structure.name.c_str(), VkAccelerationPrefix.size()) == VkAccelerationPrefix;
        if (b) {
            int bi = 0;
        }
        return std::string_view(structure.name.c_str(), VkAccelerationPrefix.size()) == VkAccelerationPrefix;
    }
#endif
    return get_manually_implemented_structures(xmlManifest).count(structure.name);
}

inline const std::set<std::string>& get_manually_implemented_handles(const xml::Manifest& xmlManifest)
{
    static const std::set<std::string> sManuallyImplementedHandles =
    [&xmlManifest]()
    {
        std::set<std::string> manuallyImplementedHandles {
            // TODO : VkCommandBuffer and VkDescriptorSet are manually implemented because
            //  they're the only two with a parent pool...
            "VkCommandBuffer",
            "VkDescriptorSet",
            // TODO : ...VkBufferView, VkFramebuffer, and VkImageView also take references
            //  to handles defined in their create infos.  This should all be handled in the
            //  generated code...
            "VkBufferView",
            "VkFramebuffer",
            "VkImageView",
        };
        return manuallyImplementedHandles;
    }();
    return sManuallyImplementedHandles;
}

inline bool is_manually_implemented(const xml::Manifest& xmlManifest, const xml::Handle& handle)
{
    return get_manually_implemented_handles(xmlManifest).count(handle.name);
}

inline const std::set<std::string>& get_unimplemented_handles(const xml::Manifest& xmlManifest)
{
    static const std::set<std::string> sUnimplementedHandles =
    [&xmlManifest]()
    {
        std::set<std::string> unimplementedHandles {
        };
        return unimplementedHandles;
    }();
    return sUnimplementedHandles;
}

inline bool is_unimplemented(const xml::Manifest& xmlManifest, const xml::Handle& handle)
{
    return get_unimplemented_handles(xmlManifest).count(handle.name);
}

inline std::vector<std::string> get_handle_compile_guards(const xml::Handle& handle)
{
    std::vector<std::string> compileGuards;
    if (!handle.compileGuard.empty()) {
        compileGuards.push_back(handle.compileGuard);
    }
    return compileGuards;
}

inline std::vector<std::string> get_structure_compile_guards(const xml::Structure& structure)
{
    std::vector<std::string> compileGuards;
    if (!structure.compileGuard.empty()) {
        compileGuards.push_back(structure.compileGuard);
    }
    return compileGuards;
}

inline std::vector<std::string> get_function_compile_guards(const xml::Function& function)
{
    std::vector<std::string> compileGuards;
    if (!function.compileGuard.empty()) {
        compileGuards.push_back(function.compileGuard);
    }
    return compileGuards;
}

inline dst::cppgen::Condition get_variable_type_condition(const xml::Manifest& xmlManifest, const xml::Parameter& variable)
{
    if (variable.name == "pNext") {
        return { "PNEXT", true };
    } else
    if (variable.flags & xml::Parameter::Array) {
        if (variable.flags & xml::Parameter::StringArray) {
            return { "DYNAMIC_STRING_ARRAY", true };
        } else 
        if (variable.flags & xml::Parameter::StaticArray) {
            if (variable.flags & xml::Parameter::String) {
                return { "STATIC_STRING", true };
            } else {
                return { "STATIC_ARRAY", true };
            }
        }
        if (variable.flags & xml::Parameter::DynamicArray) {
            if (variable.flags & xml::Parameter::String) {
                return { "DYNAMIC_STRING", true };
            } else {
                return { "DYNAMIC_ARRAY", true };
            }
        }
    } else
    if (variable.flags & xml::Parameter::Pointer && xmlManifest.structures.count(variable.unqualifiedType)) {
        return { "STRUCTURE_POINTER", true };
    } else
    if (xmlManifest.structures.count(variable.unqualifiedType)) {
        return { "STRUCTURE", true };
    }
    return { "POD", true };
}

inline dst::cppgen::SourceBlock get_manually_implemented_structure_source_blocks(const xml::Manifest& xmlManifest)
{
    using namespace dst::cppgen;
    return SourceBlock("MANUALLY_IMPLEMENTED_STRUCTURES", get_manually_implemented_structures(xmlManifest),
        [&](const std::string& manuallyImpelementedStructure) -> std::vector<SourceBlock>
        {
            auto structureItr = xmlManifest.structures.find(manuallyImpelementedStructure);
            if (structureItr != xmlManifest.structures.end() && structureItr->second.alias.empty()) {
                const auto& structure = structureItr->second;
                return {
                    SourceBlock("STRUCTURE_NAME", structure.name),
                    SourceBlock("COMPILE_GUARDS", get_structure_compile_guards(structure),
                        [&](const std::string& compileGuard) -> std::vector<SourceBlock>
                        {
                            return { SourceBlock("COMPILE_GUARD", compileGuard) };
                        }
                    ),
                };
            }
            return { };
        }
    );
}

inline dst::cppgen::SourceBlock get_automatically_implemented_structure_source_blocks(const xml::Manifest& xmlManifest)
{
    // TODO : DRY
    using namespace dst::cppgen;
    using namespace dst::vk::xml;
    return SourceBlock("STRUCTURES", xmlManifest.structures,
        [&](const std::pair<std::string, Structure>& structureItr) -> std::vector<SourceBlock>
        {
            const auto& structure = structureItr.second;
            if (structure.alias.empty() && !is_manually_implemented(xmlManifest, structure)) {
                return {
                    SourceBlock("STRUCTURE_NAME", structure.name),
                    SourceBlock("COMPILE_GUARDS", get_structure_compile_guards(structure),
                        [&](const std::string& compileGuard) -> std::vector<SourceBlock>
                        {
                            return { SourceBlock("COMPILE_GUARD", compileGuard) };
                        }
                    ),
                    SourceBlock("MEMBERS", structure.members,
                        [&](const Parameter& member) -> std::vector<SourceBlock>
                        {
                            return {
                                get_variable_type_condition(xmlManifest, member),
                                SourceBlock("MEMBER_TYPE", member.type),
                                SourceBlock("MEMBER_NAME", member.name),
                                SourceBlock("MEMBER_LENGTH", member.length)
                            };
                        }
                    )
                };
            }
            return { };
        }
    );
}

inline dst::cppgen::SourceBlock get_unfiltered_structure_source_blocks(const xml::Manifest& xmlManifest)
{
    // TODO : DRY
    using namespace dst::cppgen;
    using namespace dst::vk::xml;
    return SourceBlock("UNFILTERED_STRUCTURES", xmlManifest.structures,
        [&](const std::pair<std::string, Structure>& structureItr) -> std::vector<SourceBlock>
        {
            const auto& structure = structureItr.second;
            if (structure.alias.empty()) {
                return {
                    SourceBlock("STRUCTURE_NAME", structure.name),
                    Condition("HAS_STRUCTURE_TYPE_ENUM", !structure.vkStructureType.empty()),
                    SourceBlock("STRUCTURE_TYPE_ENUM", structure.vkStructureType),
                    SourceBlock("COMPILE_GUARDS", get_structure_compile_guards(structure),
                        [&](const std::string& compileGuard) -> std::vector<SourceBlock>
                        {
                            return { SourceBlock("COMPILE_GUARD", compileGuard) };
                        }
                    ),
                    SourceBlock("MEMBERS", structure.members,
                        [&](const Parameter& member) -> std::vector<SourceBlock>
                        {
                            return {
                                get_variable_type_condition(xmlManifest, member),
                                SourceBlock("MEMBER_TYPE", member.type),
                                SourceBlock("MEMBER_NAME", member.name),
                                SourceBlock("MEMBER_LENGTH", member.length)
                            };
                        }
                    )
                };
            }
            return { };
        }
    );
}

inline std::vector<const xml::Handle*> get_dependency_ordered_handles(const xml::Manifest& xmlManifest)
{
    std::vector<const xml::Handle*> handles;
    handles.reserve(xmlManifest.handles.size());
    xmlManifest.handles.enumerate_in_dependency_order(
        [&](const xml::Handle& handle)
        {
            handles.push_back(&handle);
        }
    );
    return handles;
}

inline std::vector<dst::cppgen::SourceBlock> get_structure_source_blocks(const xml::Manifest& xmlManifest)
{
    static const std::vector<dst::cppgen::SourceBlock> sStructureSourceBlocks {
        get_manually_implemented_structure_source_blocks(xmlManifest),
        get_automatically_implemented_structure_source_blocks(xmlManifest)
    };
    return sStructureSourceBlocks;
}

inline dst::cppgen::SourceBlock get_handle_source_blocks(const xml::Manifest& xmlManifest)
{
    using namespace dst::cppgen;
    using namespace dst::vk::xml;
    return SourceBlock("HANDLES",
        #if 0
        xmlManifest.handles,
        [&](const std::pair<std::string, Handle>& handleItr) -> std::vector<SourceBlock>
        #else
        get_dependency_ordered_handles(xmlManifest),
        [&](const xml::Handle* pHandle) -> std::vector<SourceBlock>
        #endif
        {
            #if 0
            const auto& handle = handleItr.second;
            #else
            assert(pHandle);
            const auto& handle = *pHandle;
            #endif
            bool hasAllocator = false;
            for (const auto& functionName : handle.createFunctions) {
                auto createFunctionItr = xmlManifest.functions.find(functionName);
                assert(createFunctionItr != xmlManifest.functions.end());
                for (const auto& parameter : createFunctionItr->second.parameters) {
                    if (parameter.name == "pAllocator") {
                        hasAllocator = true;
                        break;
                    }
                }
                if (hasAllocator) {
                    break;
                }
            }
            if (handle.alias.empty()) {
                return {
                    SourceBlock("OPEN_MANUALLY_IMPLEMENTED_COMPILE_GUARD", is_manually_implemented(xmlManifest, handle) ? "#if 0\n// The following ControlBlock is manually implemented" : std::string()),
                    SourceBlock("CLOSE_MANUALLY_IMPLEMENTED_COMPILE_GUARD", is_manually_implemented(xmlManifest, handle) ? "#endif" : std::string()),
                    SourceBlock("COMPILE_GUARDS", get_handle_compile_guards(handle),
                        [&](const std::string& compileGuard) -> std::vector<SourceBlock>
                        {
                            return { SourceBlock("COMPILE_GUARD", compileGuard) };
                        }
                    ),
                    SourceBlock("HANDLE_TYPE_NAME", handle.name),
                    Condition("HAS_ALLOCATOR", hasAllocator),
                    // TODO : Handle needs a VkObjectType field...
                    SourceBlock("CUSTOM_FIELDS", get_custom_handle_fields(handle.name),
                        [&](const std::string& field) -> std::vector<SourceBlock>
                        {
                            return { SourceBlock("CUSTOM_FIELD", field) };
                        }
                    ),
                    SourceBlock("HANDLE_OBJECT_TYPE", "VK_OBJECT_TYPE_UNKNOWN"),
                    SourceBlock("PARENT_HANDLES", handle.parents,
                        [&](const std::string& parent) -> std::vector<SourceBlock>
                        {
                            return { SourceBlock("PARENT_HANDLE_TYPE_NAME", parent) };
                        }
                    ),
                    SourceBlock("CREATE_INFO_STRUCTURES", handle.createInfos,
                        [&](const std::string& createInfo) -> std::vector<SourceBlock>
                        {
                            auto structureItr = xmlManifest.structures.find(createInfo);
                            if (structureItr != xmlManifest.structures.end()) {
                                const auto& structure = structureItr->second;
                                return {
                                    SourceBlock("COMPILE_GUARDS", get_structure_compile_guards(structure),
                                        [&](const std::string& compileGuard) -> std::vector<SourceBlock>
                                        {
                                            return { SourceBlock("COMPILE_GUARD", compileGuard) };
                                        }
                                    ),
                                    SourceBlock("STRUCTURE_TYPE_NAME", structure.name)
                                };
                            }
                            return { };
                        }
                    ),
                    SourceBlock("CREATE_FUNCTIONS", handle.createFunctions,
                        [&](std::string functionName) -> std::vector<SourceBlock>
                        {
                            static const std::string VkCreatePrefix = "vkC";
                            auto functionItr = xmlManifest.functions.find(functionName);
                            if (functionItr != xmlManifest.functions.end() && VkCreatePrefix.size() <= functionItr->second.name.size()) {
                                const auto& function = functionItr->second;
                                functionName = function.name[VkCreatePrefix.size() - 1] == 'C' ? "create" : "allocate";
                                std::string managedHandleParameterName;
                                for (const auto& parameter : function.parameters) {
                                    if (parameter.unqualifiedType == handle.name) {
                                        managedHandleParameterName = parameter.name;
                                        break;
                                    }
                                }
                                return {
                                    // TODO : Need to chain Coniditions in cpp-generator...
                                    Condition("HAS_ALLOCATOR", hasAllocator),
                                    SourceBlock("COMPILE_GUARDS", get_function_compile_guards(function),
                                        [&](const std::string& compileGuard) -> std::vector<SourceBlock>
                                        {
                                            return { SourceBlock("COMPILE_GUARD", compileGuard) };
                                        }
                                    ),
                                    SourceBlock("MANAGED_CREATE_FUNCTION_NAME", functionName),
                                    SourceBlock("CREATE_FUNCTION_NAME", function.name),
                                    SourceBlock("MANAGED_HANDLE_PARAMETER_NAME", managedHandleParameterName),
                                    SourceBlock("PARAMETERS", function.parameters,
                                        [&](const Parameter& parameter) -> std::vector<SourceBlock>
                                        {
                                            auto managedParameter = parameter;
                                            auto vkCallArgument = parameter.name;
                                            if (parameter.unqualifiedType == handle.name) {
                                                managedParameter.type = "Managed<" + handle.name + ">*";
                                                vkCallArgument = "&vkHandle";
                                            } else
                                            if (xmlManifest.handles.count(parameter.unqualifiedType)) {
                                                managedParameter.type = "const Managed<" + managedParameter.type + ">&";
                                            }
                                            bool parentParameter = false;
                                            for (const auto& parent : handle.parents) {
                                                if (parent == parameter.type) {
                                                    parentParameter = true;
                                                    break;
                                                }
                                            }
                                            auto createInfoParameter = parameter.unqualifiedType != "VkAllocationCallbacks" && xmlManifest.structures.count(parameter.unqualifiedType);
                                            return {
                                                // TODO : cpp-generator needs to support multiple Conditions in one SourceBlock
                                                parentParameter ? Condition("PARENT_PARAMETER", true) : createInfoParameter ? Condition("CREATE_INFO_PARAMETER", true) : Condition { },
                                                SourceBlock("MANAGED_PARAMETER_UNQUALIFIED_TYPE", parameter.unqualifiedType),
                                                SourceBlock("MANAGED_PARAMETER_TYPE", managedParameter.type),
                                                SourceBlock("MANAGED_PARAMETER_NAME", parameter.name),
                                                SourceBlock("VK_CALL_ARGUMENT", vkCallArgument),
                                            };
                                        }
                                    ),
                                };
                            }
                            return { };
                        }
                    ),
                    SourceBlock("CUSTOM_CREATE_FUNCTIONS", get_custom_handle_ctors(handle.name),
                        [&](const std::string& customCreateFunction) -> std::vector<SourceBlock>
                        {
                            return { SourceBlock("CUSTOM_CREATE_FUNCTION", customCreateFunction) };
                        }
                    ),
                    SourceBlock("DESTROY_FUNCTIONS", handle.destroyFunctions,
                        [&](std::string functionName) -> std::vector<SourceBlock>
                        {
                            assert(handle.destroyFunctions.size() == 1);
                            static const std::string VkDestroyPrefix = "vkD";
                            auto functionItr = xmlManifest.functions.find(functionName);
                            if (functionItr != xmlManifest.functions.end() && VkDestroyPrefix.size() <= functionItr->second.name.size()) {
                                const auto& function = functionItr->second;
                                functionName = function.name[VkDestroyPrefix.size() - 1] == 'D' ? "destroy" : "free";
                                std::vector<const xml::Parameter*> parentParameters;
                                for (const auto& parameter : function.parameters) {
                                    if (parameter.unqualifiedType == handle.name) {
                                        break;
                                    }
                                    parentParameters.push_back(&parameter);
                                }
                                return {
                                    SourceBlock("VK_DESTROY_FUNCTION_NAME", function.name),
                                    SourceBlock("PARENT_PARAMETERS", parentParameters,
                                        [&](const xml::Parameter* pParentParameter) -> std::vector<SourceBlock>
                                        {
                                            return {
                                                SourceBlock("STRIP_VK_PARENT_HANDLE_PARAMETER_TYPE_NAME", strip_vk(pParentParameter->unqualifiedType)),
                                                SourceBlock("PARENT_HANDLE_PARAMETER_TYPE_NAME", pParentParameter->unqualifiedType),
                                            };
                                        }
                                    ),
                                    SourceBlock("PARAMETERS", function.parameters,
                                        [&](const Parameter& parameter) -> std::vector<SourceBlock>
                                        {
                                            auto vkParameter = parameter.name;
                                            if (parameter.unqualifiedType == handle.name) {
                                                vkParameter[0] = (char)toupper((int)vkParameter[0]);
                                                vkParameter = "vk" + vkParameter;
                                            } else
                                            if (xmlManifest.handles.count(parameter.unqualifiedType)) {
                                                vkParameter = strip_vk(vkParameter) + ".get<" + parameter.unqualifiedType + ">()";
                                                vkParameter[0] = (char)tolower((int)vkParameter[0]);
                                            } else
                                            if (parameter.name == "pAllocator") {
                                                vkParameter = "nullptr /* TODO : pAllocator */";
                                            }
                                            return {
                                                SourceBlock("VK_PARAMETER", vkParameter),
                                            };
                                        }
                                    )
                                };
                            }
                            return { };
                        }
                    )
                };
            }
            return { };
        }
    );
}

} // namespace cppgen
} // namespace vk
} // namespace dst
