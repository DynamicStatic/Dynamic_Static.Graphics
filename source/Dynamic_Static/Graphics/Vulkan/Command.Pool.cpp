
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

#include "Dynamic_Static/Graphics/Vulkan/Command.Pool.hpp"
#include "Dynamic_Static/Graphics/Vulkan/Device.hpp"

namespace Dynamic_Static {
namespace Graphics {
namespace Vulkan {

    Command::Pool::Pool(const std::shared_ptr<Device>& device, const Info& info)
        : mDevice { device }
    {
        assert(mDevice);
        Validate(vkCreateCommandPool(*mDevice, &info, nullptr, &mHandle));
        name("Dynamic_Static::Vulkan::Command::Pool");
    }

    Command::Pool::~Pool()
    {
        if (mHandle) {
            vkDestroyCommandPool(device(), mHandle, nullptr);
        }
    }

    Device& Command::Pool::device()
    {
        assert(mDevice);
        return *mDevice;
    }

    const Device& Command::Pool::device() const
    {
        assert(mDevice);
        return *mDevice;
    }

} // namespace Vulkan
} // namespace Graphics
} // namespace Dynamic_Static