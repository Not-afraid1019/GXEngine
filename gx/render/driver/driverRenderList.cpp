#include "driverRenderList.h"

namespace gx {

    RenderItem::RenderItem() noexcept {}

    RenderItem::~RenderItem() noexcept {}

    DriverRenderList::DriverRenderList() {}

    DriverRenderList::~DriverRenderList() {}

    // 每一帧开始的时候，渲染列表都会被清空
    void DriverRenderList::init() noexcept {
        mRenderItemIndex = 0;
        mOpaques.clear();
        mTransparents.clear();
    }

    // push进去一个可渲染物体的相关参数，以解包的方式
    // 为什么需要解包传送，有可能会有替代，例如：本来object拥有一个material，但是scene也拥有一个overrideMaterial
    // 那么就不能使用object原来的material
    void DriverRenderList::push(
            const RenderableObject::Ptr &object,
            const Geometry::Ptr &geometry,
            const Material::Ptr &material,
            const uint32_t &groupOrder,
            float z
        ) noexcept {
        // 每一帧都会重新构建renderList，所以如果有5个物体，并且不做renderItem缓存
        // 那么每一帧都需要重新new 5个renderItem
        // 所以上一帧假设已经生成了10个renderItem，那么会将其缓存在renderItemCache里面，在需要的时候
        // 就从中取出一个给到renderList使用
        const auto renderItem = getNextRenderItem(object, geometry, material, groupOrder, z);

        // 检测是否开启透明
        if (material->mTransparent) {
            mTransparents.push_back(renderItem);
        }
        else {
            mOpaques.push_back(renderItem);
        }
    }

    RenderItem::Ptr
    DriverRenderList::getNextRenderItem(const RenderableObject::Ptr &object,
                                        const Geometry::Ptr &geometry,
                                        const Material::Ptr &material,
                                        const uint32_t &groupOrder,
                                        float z
                                        ) noexcept {
        RenderItem::Ptr renderItem = nullptr;

        // 如果当前需要renderItem的总数已经大于了缓存数量，那么就重新生成
        // 否则直接从缓存当中抽取第mRenderItemIndex个item
        if (mRenderItemIndex >= mRenderItemCache.size()) {
            renderItem = RenderItem::create();
            mRenderItemCache.push_back(renderItem);
        }
        else {
            renderItem = mRenderItemCache[mRenderItemIndex];
        }

        renderItem->mID = object->getID();
        renderItem->mObject = object;
        renderItem->mGeometry = geometry;
        renderItem->mMaterial = material;
        renderItem->mGroupOrder = groupOrder;
        renderItem->mZ = z;

        mRenderItemIndex++;

        return renderItem;
    }

    void DriverRenderList::sort(
            const gx::RenderListSortFunction &opaqueSort,
            const gx::RenderListSortFunction &transparentSort) noexcept {

        if (!mOpaques.empty()) std::sort(mOpaques.begin(), mOpaques.end(), opaqueSort);

        if (!mTransparents.empty()) std::sort(mTransparents.begin(), mTransparents.end(), transparentSort);
    }

    // 如果缓存当中存在10个item，本帧只有5个物体需要渲染，那么就会使用到10个item中的5个
    // 但是！剩下的没有使用到的5个item里面，却存有object、material、geometry的智能指针，其内存无法被释放
    // finish需要检测剩下的没有使用到的item，然后依次将其智能指针的引用置空
    void DriverRenderList::finish() noexcept {
        auto listSize = mRenderItemCache.size();
        for (uint32_t i = mRenderItemIndex; i < listSize; ++i) {
            auto renderItem = mRenderItemCache[i];
            if (renderItem == nullptr) {
                break;
            }

            // 清空所有的智能指针引用以及数据
            renderItem->mID = 0;
            renderItem->mObject = nullptr;
            renderItem->mGeometry = nullptr;
            renderItem->mGroupOrder = 0;
            renderItem->mMaterial = nullptr;
            renderItem->mZ = 0;
        }
    }
}