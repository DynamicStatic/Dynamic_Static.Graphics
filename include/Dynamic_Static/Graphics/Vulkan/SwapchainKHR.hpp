
/*
================================================================================

  MIT License

  Copyright (c) 2017 Dynamic_Static

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

================================================================================
*/

#pragma once

#include "Dynamic_Static/Graphics/Vulkan/Defines.hpp"
#include "Dynamic_Static/Graphics/Vulkan/Object.hpp"

#include <memory>
#include <vector>

namespace Dynamic_Static {
namespace Graphics {
namespace Vulkan {

    /**
     * Provides high level control over a Vulkan SwapchainKHR.
     */
    class SwapchainKHR final
        : public Object<VkSwapchainKHR>
    {
        friend class Device;

    private:
        VkFormat mFormat;
        VkExtent2D mExtent;
        std::shared_ptr<Device> mDevice { nullptr };
        std::shared_ptr<SurfaceKHR> mSurface { nullptr };
        std::vector<std::unique_ptr<Image>> mImages;

    private:
        SwapchainKHR(
            const std::shared_ptr<Device>& device,
            const std::shared_ptr<SurfaceKHR>& surface
        );

    public:
        /**
         * TODO : Documentation.
         */
        Device& device();

        /**
         * TODO : Documentation.
         */
        const Device& device() const;

        /**
         * TODO : Documentation.
         */
        SurfaceKHR& surface();

        /**
         * TODO : Documentation.
         */
        const SurfaceKHR& surface() const;

        /**
         * Gets this SwapchainKHR's Images.
         * @return This SwapchainKHR's Images
         */
        const std::vector<std::unique_ptr<Image>>& images() const;

        /**
         * TODO : Documentation.
         */
        VkFormat format() const;

        /**
         * TODO : Documentation.
         */
        VkExtent2D extent() const;

    private:
        void on_surface_resized();
    };

} // namespace Vulkan
} // namespace Graphics
} // namespace Dynamic_Static
