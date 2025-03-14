#include "geometry.h"
#include "../tools/identity.h"
#include "../global/eventDispatcher.h"

namespace gx {

    Geometry::Geometry() noexcept {
        mID = Identity::generateID();
    }

    Geometry::~Geometry() noexcept {
        // 通过Dispatcher这个单例，对所有监听函数，发出Geometry消亡的消息
        EventBase::Ptr e = EventBase::create("geometryDispose");
        e->mTarget = this;
        EventDispatcher::getInstance()->dispatchEvent(e);
    }

    void Geometry::setAttribute(const std::string &name, Attributef::Ptr attribute) noexcept {
        mAttributes[name] = attribute;
    }

    Attributef::Ptr Geometry::getAttribute(const std::string &name) noexcept {
        auto iter = mAttributes.find(name);
        if (iter != mAttributes.end()) {
            return iter->second;
        }

        return nullptr;
    }

    void Geometry::setIndex(const Attributei::Ptr &index) noexcept {
        mIndexAttribute = index;
    }

    void Geometry::deleteAttribute(const std::string &name) noexcept {
        auto iter = mAttributes.find(name);
        if (iter != mAttributes.end()) {
            mAttributes.erase(iter);
        }
    }

    bool Geometry::hasAttribute(const std::string &name) noexcept {
        auto iter = mAttributes.find(name);
        if (iter != mAttributes.end()) {
            return false;
        }

        return true;
    }

    const Geometry::AttributeMap &Geometry::getAttributes() const noexcept {
        return mAttributes;
    }

    ID Geometry::getID() const noexcept {
        return mID;
    }

    void Geometry::computeBoundingBox() noexcept {
        auto position = getAttribute("position");

        if (position == nullptr) {
            std::cout << "Error: geometry has no position when computeBoundingBox" << std::endl;
            return;
        }

        if (mBoundingBox == nullptr) {
            mBoundingBox = Box3::create();
        }

        mBoundingBox->setFromAttribute(position);
    }

    void Geometry::computeBoundingSphere() noexcept {
        computeBoundingBox();
        if (mBoundingSphere == nullptr) {
            mBoundingSphere = Sphere::create(glm::vec3(0.0f), 0.0f);
        }

        // 包围球与包围盒共享了一个center
        mBoundingSphere->mCenter = mBoundingBox->getCenter();

        // find smallest sphere : inscribed sphere
        auto position = getAttribute("position");
        if (position == nullptr) {
            return;
        }

        // 找到距离当前球心最大的点
        float maxRadiusSq = 0;
        for (uint32_t i = 0; i < position->getCount(); ++i) {
            glm::vec3 point = glm::vec3(position->getX(i), position->getY(i), position->getZ(i));

            // 计算point到center的距离
            glm::vec3 radiusVector = mBoundingSphere->mCenter - point;
            maxRadiusSq = std::max(glm::dot(radiusVector, radiusVector), maxRadiusSq);
        }

        mBoundingSphere->mRadius = std::sqrt(maxRadiusSq);
    }
}