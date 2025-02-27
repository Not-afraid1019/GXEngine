#include "driverObjects.h"


namespace gx {

    DriverObjects::DriverObjects(
            const DriverGeometries::Ptr &geometries,
            const DriverAttributes::Ptr &attributes,
            const DriverInfo::Ptr &info
            ) noexcept {
        mGeometries = geometries;
        mAttributes = attributes;
        mInfo = info;
    }

    DriverObjects::~DriverObjects() noexcept {}

    // 调用object的geometry之update
    // 不同的object可能会共享同一个geometry
    // 得在这里保证每个geometry每一帧只update一次
    Geometry::Ptr DriverObjects::update(const RenderableObject::Ptr &object) noexcept {
        // 1 拿到当前到了第几帧
        const auto frame = mInfo->mRender.mFrame;

        // 2 拿出geometry并且在get里面做相关的数据记录
        const auto geometry = mGeometries->get(object->getGeometry());

        // update once per frame, multi-objects-one geometry

        // key:geometry的id
        // value:frameNumber
        auto iter = mUpdateMap.find(geometry->getID());

        // 如果当前frame是5
        // geometry3是第一次进入，geometry就会被DriverGeometry给update
        // mUpdate里面就会加入(3,5)
        // 如果在本帧之内，有另一个object也使用了geometry3
        // 当前3号geometry所对应的frame是5，就会直接不处理

        // 如果此时进入到frame=6的情况
        // geometry3再次被进行寻找，找到了一个键值对(3,5)，当前frame与5不相等
        // geometry3就得到了依次update的机会，并且mUpdateMap里面的键值对就会被更新成(3, 6)

        if (iter == mUpdateMap.end() || mUpdateMap[geometry->getID()] != frame) {
            mGeometries->update(geometry);
            mUpdateMap[geometry->getID()] = frame;
        }

        return geometry;
    }
}