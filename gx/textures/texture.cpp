#include "texture.h"
#include "../tools/identity.h"
#include "../global/eventDispatcher.h"

namespace gx {

    Texture::Texture(
            const uint32_t &width,
            const uint32_t &height,
            const gx::DataType &dataType,
            const gx::TextureWrapping &wrapS,
            const gx::TextureWrapping &wrapT,
            const gx::TextureWrapping &wrapR,
            const gx::TextureFilter &magFilter,
            const gx::TextureFilter &minFilter,
            const gx::TextureFormat &format
            ) noexcept {
        mID = Identity::generateID();
        mWidth = width;
        mHeight = height;
        mDataType = dataType;
        mWrapS = wrapS;
        mWrapR = wrapR;
        mWrapT = wrapT;
        mMagFilter = magFilter;
        mMinFilter = minFilter;
        mFormat = format;
        mTextureType = TextureType::Texture2D;
    }

    Texture::~Texture() noexcept {
        // 消亡的时候通过dispatcher向外发出本texture消亡的消息
        EventBase::Ptr e = EventBase::create("textureDispose");
        e->mTarget = this;
        EventDispatcher::getInstance()->dispatchEvent(e);

        if (mSource) {
            EventBase::Ptr e = EventBase::create("sourceRelease");
            e->mTarget = mSource.get();
            EventDispatcher::getInstance()->dispatchEvent(e);
        }
    }

    Texture::Ptr Texture::clone() noexcept {
        return Texture::create(mWidth, mHeight, mDataType, mWrapS, mWrapT, mWrapR, mMagFilter, mMinFilter, mFormat);
    }
}