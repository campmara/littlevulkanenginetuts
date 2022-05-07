#ifndef BUFFER_H
#define BUFFER_H

#include "Device.h"

namespace XIV {
    class Buffer {
    public:
        Buffer(Device &device,
               VkDeviceSize instanceSize,
               u32 instanceCount,
               VkBufferUsageFlags usageFlags,
               VkMemoryPropertyFlags memoryPropertyFlags,
               VkDeviceSize minOffsetAlignment = 1);
        ~Buffer();
        Buffer(const Buffer &) = delete;
        Buffer &operator=(const Buffer &) = delete;

        VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void Unmap();

        void WriteToBuffer(void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE,
                                              VkDeviceSize offset = 0);
        VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void WriteToIndex(void *data, int index);
        VkResult FlushIndex(int index);
        VkDescriptorBufferInfo DescriptorInfoForIndex(int index);
        VkResult InvalidateIndex(int index);

        void *Mapped = nullptr;
        VkBuffer VulkanBuffer = VK_NULL_HANDLE;
        VkDeviceSize BufferSize;
        u32 InstanceCount;
        VkDeviceSize InstanceSize;
        VkDeviceSize AlignmentSize;
        VkBufferUsageFlags UsageFlags;
        VkMemoryPropertyFlags MemoryPropertyFlags;

    private:
        static VkDeviceSize GetAlignment(VkDeviceSize instanceSize,
                                         VkDeviceSize minOffsetAlignment);

        Device &device;
        VkDeviceMemory memory = VK_NULL_HANDLE;
    };

} // namespace XIV

#endif