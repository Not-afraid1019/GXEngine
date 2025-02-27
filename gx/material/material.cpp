#include "material.h"
#include "../tools/identity.h"
#include "../global/eventDispatcher.h"

namespace gx {

    Material::Material() noexcept {
        mID = Identity::generateID();
        mType = MaterialName::Material;
        mIsMaterial = true;
    }

    Material::~Material() noexcept {
        EventBase::Ptr e = EventBase::create("materialDispose");
        e->mTarget = this;
        EventDispatcher::getInstance()->dispatchEvent(e);
    }
}