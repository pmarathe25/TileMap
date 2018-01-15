#ifndef TILE_MAP_VIEW_H
#define TILE_MAP_VIEW_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <size_t widthAtCompileTime, size_t lengthAtCompileTime, size_t heightAtCompileTime, typename InternalTileMap, typename dat, typename writable>
        struct traits<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, dat, writable>> {
            typedef typename internal::traits<InternalTileMap>::ScalarType ScalarType;
            // Dimensions
            static constexpr size_t length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height;
            typedef dat containsData;
            typedef writable isWritable;
        };
    } /* internal */

    // Writable view
    template <size_t widthAtCompileTime, size_t lengthAtCompileTime, size_t heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(InternalTileMap& tileMap, size_t minX = 0, size_t minY = 0, size_t minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset1D{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offset2D{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr auto& operator()(size_t x, size_t y, size_t z) {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr const auto& operator()(size_t x, size_t y, size_t z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr auto& operator()(size_t x, size_t y) {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr const auto& operator()(size_t x, size_t y) const {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr auto& operator()(size_t x) {
                return tileMap(x + offset1D);
            }

            constexpr const auto& operator()(size_t x) const {
                return tileMap(x + offset1D);
            }

            constexpr const auto& operator[](size_t x) const {
                size_t y = x / this -> width();
                size_t z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }

            constexpr auto& operator[](size_t x) {
                size_t y = x / this -> width();
                size_t z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }

            constexpr const auto* data() const noexcept {
                return &(this -> operator[](0));
            }

            constexpr auto* data() noexcept {
                return &(this -> operator[](0));
            }
        private:
            InternalTileMap& tileMap;
            const size_t minX, minY, minZ;
            const size_t offset1D, offset2D;
    };

    // Const view
    template <size_t widthAtCompileTime, size_t lengthAtCompileTime, size_t heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const InternalTileMap& tileMap, size_t minX = 0, size_t minY = 0, size_t minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset1D{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offset2D{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr const auto& operator()(size_t x, size_t y, size_t z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr const auto& operator()(size_t x, size_t y) const {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr const auto& operator()(size_t x) const {
                return tileMap(x + offset1D);
            }

            constexpr const auto& operator[](size_t x) const {
                size_t y = x / this -> width();
                size_t z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }

            constexpr const auto* data() const noexcept {
                return &(this -> operator[](0));
            }
        private:
            const InternalTileMap& tileMap;
            const size_t minX, minY, minZ;
            const size_t offset1D, offset2D;
    };

    // A view of a temporary object. Cannot be modified.
    template <size_t widthAtCompileTime, size_t lengthAtCompileTime, size_t heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const InternalTileMap& tileMap, size_t minX = 0, size_t minY = 0, size_t minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset1D{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offset2D{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr auto operator()(size_t x, size_t y, size_t z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr auto operator()(size_t x, size_t y) const {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr auto operator()(size_t x) const {
                return tileMap(x + offset1D);
            }

            constexpr auto operator[](size_t x) const {
                size_t y = x / this -> width();
                size_t z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }
        private:
            const InternalTileMap& tileMap;
            const size_t minX, minY, minZ;
            const size_t offset1D, offset2D;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
