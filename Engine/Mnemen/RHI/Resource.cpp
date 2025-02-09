//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 10:44:01
//

#include <RHI/Resource.hpp>
#include <RHI/Utilities.hpp>

#include <Core/UTF.hpp>
#include <Core/Assert.hpp>
#include <Core/Profiler.hpp>

Resource::Resource(Device::Ref device)
    : mParentDevice(device), mResource(nullptr), mLayout(ResourceLayout::Common)
{
}

Resource::~Resource()
{
    if (mShouldFree) {
        D3DUtils::Release(mResource);
    }
    Profiler::PopResource(mUUID);
}

void Resource::SetName(const String& string)
{
    mName = string;
    mResource->SetName(UTF::AsciiToWide(string).data());

    mUUID = Profiler::PushResource(mAllocSize, mName);
}

void Resource::Tag(ResourceTag tag)
{
    mTags.push_back(tag);
    Profiler::TagResource(mUUID, tag);
}

void Resource::CreateResource(D3D12_HEAP_PROPERTIES* heapProps, D3D12_RESOURCE_DESC* resourceDesc, D3D12_RESOURCE_STATES state)
{
    mLayout = ResourceLayout(state);
    HRESULT result = mParentDevice->GetDevice()->CreateCommittedResource(heapProps, D3D12_HEAP_FLAG_NONE, resourceDesc, state, nullptr, IID_PPV_ARGS(&mResource));
    ASSERT(SUCCEEDED(result), "Failed to allocate resource!");

    mParentDevice->GetDevice()->GetCopyableFootprints(resourceDesc, 0, resourceDesc->MipLevels, 0, nullptr, nullptr, nullptr, &mAllocSize);
}
