
#if 0
/*
==========================================
    Copyright (c) 2017 Dynamic_Static
    Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#include "Dynamic_Static/Graphics/Vulkan/DebugReport.hpp"
#include "Dynamic_Static/Core/ToString.hpp"
#include "Dynamic_Static/Graphics/Vulkan/Instance.hpp"

#include <iostream>
#include <string>

namespace Dynamic_Static {
namespace Graphics {
namespace Vulkan {

    constexpr VkDebugReportCallbackCreateInfoEXT DebugReport::CreateInfo;

    void debug_report_message(const std::string& label, const char* layerPrefix, const int32_t& messageCode, const char* message);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report_callback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* layerPrefix,
        const char* message,
        void* userData
    );

    DebugReport::DebugReport(Instance& instance, VkDebugReportFlagsEXT flags)
        : mInstance { &instance }
    {
        mInstance->get_function_pointer("vkDebugReportMessageEXT", vkDebugReportMessageEXT);
        mInstance->get_function_pointer("vkCreateDebugReportCallbackEXT", vkCreateDebugReportCallbackEXT);
        mInstance->get_function_pointer("vkDestroyDebugReportCallbackEXT", vkDestroyDebugReportCallbackEXT);

        auto info = CreateInfo;
        info.flags = flags;
        info.pfnCallback = debug_report_callback;
        info.pUserData = this;
        validate(vkCreateDebugReportCallbackEXT(*mInstance, &info, nullptr, &mHandle));
        name("DebugReport");
    }

    DebugReport::~DebugReport()
    {
        if (mHandle) {
            vkDestroyDebugReportCallbackEXT(*mInstance, mHandle, nullptr);
        }
    }

    const Instance& DebugReport::instance() const
    {
        return *mInstance;
    }

    bool DebugReport::standard_error_enabled() const
    {
        return mStandardErrorEnabled;
    }

    void DebugReport::standard_error_enabled(bool standardErrorEnabled)
    {
        mStandardErrorEnabled = standardErrorEnabled;
    }

    void debug_report_message(const std::string& tlStr, const char* layerPrefix, const int32_t& messageCode, const char* message)
    {
        // if (debugReport.standard_error_enabled()) {
        std::cout << "{ " + tlStr + " } - " + std::string(layerPrefix) + "[" + dst::to_string(messageCode) + "]-" + std::string(message) << std::endl;
        // }
    }

    #if defined(DYNAMIC_STATIC_MSVC)
    #pragma warning(push)
    #pragma warning(disable : 4100) // NOTE : unreferenced formal parameter
    #pragma warning(disable : 4189) // NOTE : local variable is initialized but not referenced
    #endif

    VKAPI_ATTR VkBool32 VKAPI_CALL debug_report_callback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* layerPrefix,
        const char* message,
        void* userData
    )
    {
        const auto& debugReport = *static_cast<DebugReport*>(userData);
        thread_local std::string tlStr;
        tlStr.clear();
        if (flags) {
            if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
                debug_report_message("Information", layerPrefix, messageCode, message);
            }

            if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
                debug_report_message("Debug", layerPrefix, messageCode, message);
            }

            if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
                debug_report_message("Performance", layerPrefix, messageCode, message);
                #if DYNAMIC_STATIC_MSVC
                __debugbreak();
                #endif
            }

            if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
                debug_report_message("Warning", layerPrefix, messageCode, message);
                #if DYNAMIC_STATIC_MSVC
                __debugbreak();
                #endif
            }

            if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
                debug_report_message("Error", layerPrefix, messageCode, message);
                #if DYNAMIC_STATIC_MSVC
                __debugbreak();
                #endif
            }
        } else {
            std::cout << "----" << std::endl;
        }

        return VK_FALSE;
    }

    #if defined(DYNAMIC_STATIC_MSVC)
    #pragma warning(pop)
    #endif

} // namespace Vulkan
} // namespace Graphics
} // namespace Dynamic_Static
#endif
