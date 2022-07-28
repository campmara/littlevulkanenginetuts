#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include "Device.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace XIV::Render {
    class DescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(Device &device) : device{device} {}

            Builder &AddBinding(u32 binding,
                                VkDescriptorType descriptorType,
                                VkShaderStageFlags stageFlags,
                                u32 count = 1);
            std::unique_ptr<DescriptorSetLayout> Build() const;

        private:
            Device &device;
            std::unordered_map<u32, VkDescriptorSetLayoutBinding> bindings{};
        };

        DescriptorSetLayout(Device &device,
                            std::unordered_map<u32, VkDescriptorSetLayoutBinding> bindings);
        ~DescriptorSetLayout();
        DescriptorSetLayout(const DescriptorSetLayout &) = delete;
        DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

        VkDescriptorSetLayout VulkanDescriptorSetLayout;

    private:
        Device &device;
        std::unordered_map<u32, VkDescriptorSetLayoutBinding> bindings;

        friend class DescriptorWriter;
    };

    class DescriptorPool {
    public:
        class Builder {
        public:
            Builder(Device &device) : device{device} {}

            Builder &AddPoolSize(VkDescriptorType descriptorType, u32 count);
            Builder &SetPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &SetMaxSets(u32 count);
            std::unique_ptr<DescriptorPool> Build() const;

        private:
            Device &device;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            u32 maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        DescriptorPool(Device &device,
                       u32 maxSets,
                       VkDescriptorPoolCreateFlags poolFlags,
                       const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~DescriptorPool();
        DescriptorPool(const DescriptorPool &) = delete;
        DescriptorPool &operator=(const DescriptorPool &) = delete;

        bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
                                VkDescriptorSet &descriptor) const;

        void FreeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void ResetPool();

    private:
        Device &device;
        VkDescriptorPool descriptorPool;

        friend class DescriptorWriter;
    };

    class DescriptorWriter {
    public:
        DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool);

        DescriptorWriter &WriteBuffer(u32 binding, VkDescriptorBufferInfo *bufferInfo);
        DescriptorWriter &WriteImage(u32 binding, VkDescriptorImageInfo *imageInfo);

        bool Build(VkDescriptorSet &set);
        void Overwrite(VkDescriptorSet &set);

    private:
        DescriptorSetLayout &setLayout;
        DescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

} // namespace XIV::Render

#endif