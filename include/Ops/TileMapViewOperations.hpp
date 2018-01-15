#ifndef TILE_MAP_VIEW_OPERATIONS_H
#define TILE_MAP_VIEW_OPERATIONS_H
#include "../OpStructs/TileMapView.hpp"
#include <type_traits>
#include <cassert>

namespace StealthTileMap {
    template <typename LHS>
    constexpr auto layer(LHS&& lhs, unsigned layerNum = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return TileMapView<internal::traits<LHSRawType>::width, internal::traits<LHSRawType>::length, 1, LHSRawType>
            {std::forward<LHS&&>(lhs), 0, 0, layerNum};
    }

    template <unsigned width, unsigned length, unsigned height, typename LHS>
    constexpr auto block(LHS&& lhs, unsigned minX = 0, unsigned minY = 0, unsigned minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return TileMapView<width, length, height, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <unsigned width, unsigned length, typename LHS>
    constexpr auto block(LHS&& lhs, unsigned minX = 0, unsigned minY = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return TileMapView<width, length, internal::traits<LHSRawType>::height, LHSRawType>
            {std::forward<LHS&&>(lhs), minX, minY, 0};
    }

    template <unsigned width, typename LHS>
    constexpr auto block(LHS&& lhs, unsigned minX = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return TileMapView<width, internal::traits<LHSRawType>::length, internal::traits<LHSRawType>::height, LHSRawType>
            {std::forward<LHS&&>(lhs), minX, 0, 0};
    }
} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_OPERATIONS_H */
