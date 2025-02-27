#pragma once
#include "../global/base.h"
#include "../global/constant.h"
#include "../textures/source.h"
#include "../global/eventDispatcher.h"

namespace gx {

    // 从中读取已经保存过的source
    // 将新的source缓存到这个类里面
    // source在被其它对象放弃使用权的时候能够知晓,并且没有人使用的时候析构
    class Cache {
    public:
        static Cache* getInstance();

        ~Cache() noexcept;

        Source::Ptr getSource(const std::string& path) noexcept;

        void cacheSource(const std::string& path, Source::Ptr source) noexcept;

        void onSourceRelease(const EventBase::Ptr& e);

    private:
        Cache() noexcept;
    private:
        static Cache* mInstance;

        // hashType = size_t
        std::unordered_map<HashType, Source::Ptr> mSources{};
    };
}