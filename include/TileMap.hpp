#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include "./ForwardDeclarations.hpp"
#include "./TileMapBase.hpp"
#include "./Ops/BinaryOperations.hpp"
#include "./Ops/UnaryOperations.hpp"
#include "./Ops/RuntimeOperations.hpp"
#include "./Ops/TileMapViewOperations.hpp"
#include <stealthutil>
#include <vector>
#include <iostream>
#include <thread>

namespace StealthTileMap {
    constexpr int NUM_THREADS = 8;

    namespace internal {
        template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
            typedef type ScalarType;
            static constexpr int width = widthAtCompileTime, length = lengthAtCompileTime, height = heightAtCompileTime,
                area = areaAtCompileTime, size = sizeAtCompileTime;
            static constexpr bool isTemporary = false;
        };
    } /* internal */

    template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        int areaAtCompileTime, int sizeAtCompileTime>
    class TileMap : public TileMapBase<TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
        public:
            typedef type ScalarType;

            static constexpr bool isTemporary = false;

            constexpr TileMap() noexcept : tiles(sizeAtCompileTime) { }

            // Copy
            template <typename OtherTileMap>
            constexpr TileMap(const OtherTileMap& other) noexcept : tiles(sizeAtCompileTime) {
                copyMultithreaded(other);
            }

            constexpr TileMap(const TileMap& other) noexcept {
                tiles = other.tiles;
            }

            // Move
            constexpr TileMap(TileMap&& other) noexcept = default;

            // Assignment
            constexpr TileMap& operator=(TileMap&& other) noexcept = default;

            constexpr TileMap& operator=(const TileMap& other) noexcept {
                tiles = other.tiles;
                return *this;
            }

            template <typename OtherTileMap>
            constexpr TileMap& operator=(const OtherTileMap& other) noexcept {
                copyMultithreaded(other);
                return *this;
            }

            // Accessors
            constexpr ScalarType& operator()(int x, int y, int z) {
                return tiles[this -> area() * z + this -> width() * y + x];
            }

            constexpr const ScalarType& operator()(int x, int y, int z) const {
                return tiles[this -> area() * z + this -> width() * y + x];
            }

            constexpr ScalarType& operator()(int x, int y) {
                return tiles[this -> width() * y + x];
            }

            constexpr const ScalarType& operator()(int x, int y) const {
                return tiles[this -> width() * y + x];
            }

            constexpr ScalarType& operator()(int x) {
                return tiles[x];
            }

            constexpr const ScalarType& operator()(int x) const {
                return tiles[x];
            }

            constexpr const ScalarType* data() const noexcept {
                return tiles.data();
            }

            constexpr ScalarType* data() noexcept {
                return tiles.data();
            }

            constexpr typename std::vector<ScalarType>::iterator begin() noexcept {
                return tiles.begin();
            }

            constexpr typename std::vector<ScalarType>::const_iterator cbegin() const noexcept {
                return tiles.cbegin();
            }

            constexpr typename std::vector<ScalarType>::iterator end() noexcept {
                return tiles.end();
            }

            constexpr typename std::vector<ScalarType>::const_iterator cend() const noexcept {
                return tiles.cend();
            }

            constexpr TileMap& eval() {
                return (*this);
            }
        private:
            std::vector<ScalarType> tiles;
            std::array<std::thread, NUM_THREADS> copyThreads;

            template <typename OtherTileMap>
            constexpr void copyPortion(const OtherTileMap* other, int id) {
                // Copy elements for a single portion of the TileMap.
                constexpr int portionSize = ceilDivide(this -> size(), NUM_THREADS);
                const int start = id * portionSize;
                const int end = std::min(start + portionSize, this -> size());
                for (int i = start; i < end; ++i) {
                    tiles[i] = other -> operator()(i);
                }
            }

            template <typename OtherTileMap>
            constexpr void copyMultithreaded(const OtherTileMap& other) {
                // Make sure dimensions are compatible
                static_assert(internal::traits<OtherTileMap>::length == this -> length() && internal::traits<OtherTileMap>::width
                    == this -> width() && internal::traits<OtherTileMap>::height == this -> height(), "Cannot copy incompatible TileMaps");
                // Create threads
                for (int i = 0; i < NUM_THREADS; ++i) {
                    copyThreads[i] = std::thread{&TileMap::copyPortion<OtherTileMap>, this, &other, i};
                }
                // Wait for all threads to finish
                for (auto& thread : copyThreads) {
                    thread.join();
                }
            }
    };

    // Specialization for boolean maps
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, int areaAtCompileTime,
        int sizeAtCompileTime>
    class TileMap<bool, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>
        : public TileMapBase<TileMap<bool, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
        public:
            constexpr TileMap() : tiles(sizeAtCompileTime) { }

            // Copy
            template <typename OtherTileMap>
            constexpr TileMap(const OtherTileMap& other) : tiles(sizeAtCompileTime) {
                copyMultithreaded(other);
            }

            constexpr TileMap(const TileMap& other) {
                tiles = other.tiles;
            }

            // Move
            constexpr TileMap(TileMap&& other) noexcept = default;

            // Assignment
            constexpr void operator=(const TileMap& other) noexcept {
                tiles = other.tiles;
            }

            template <typename OtherTileMap>
            constexpr void operator=(const OtherTileMap& other) noexcept {
                copyMultithreaded(other);
            }

            // Accessors
            constexpr bool operator()(int x, int y, int z) const {
                return tiles[this -> area() * z + this -> width() * y + x];
            }

            constexpr bool operator()(int x, int y) const {
                return tiles[this -> width() * y + x];
            }

            constexpr bool operator()(int x) const {
                return tiles[x];
            }

            constexpr typename std::vector<bool>::iterator begin() noexcept {
                return tiles.begin();
            }

            constexpr typename std::vector<bool>::const_iterator cbegin() const noexcept {
                return tiles.cbegin();
            }

            constexpr typename std::vector<bool>::iterator end() noexcept {
                return tiles.end();
            }

            constexpr typename std::vector<bool>::const_iterator cend() const noexcept {
                return tiles.cend();
            }

            constexpr TileMap& eval() {
                return (*this);
            }
        private:
            std::vector<bool> tiles;
            std::array<std::thread, NUM_THREADS> copyThreads;

            template <typename OtherTileMap>
            constexpr void copyPortion(const OtherTileMap* other, int id) {
                // Copy elements for a single portion of the TileMap.
                constexpr int portionSize = ceilDivide(this -> size(), NUM_THREADS);
                const int start = id * portionSize;
                const int end = std::min(start + portionSize, this -> size());
                for (int i = start; i < end; ++i) {
                    tiles[i] = other -> operator()(i);
                }
            }

            template <typename OtherTileMap>
            constexpr void copyMultithreaded(const OtherTileMap& other) {
                // Make sure dimensions are compatible
                static_assert(internal::traits<OtherTileMap>::length == this -> length() && internal::traits<OtherTileMap>::width
                    == this -> width() && internal::traits<OtherTileMap>::height == this -> height(), "Cannot copy incompatible TileMaps");
                // Create threads
                for (int i = 0; i < NUM_THREADS; ++i) {
                    copyThreads[i] = std::thread{&TileMap::copyPortion<OtherTileMap>, this, &other, i};
                }
                // Wait for all threads to finish
                for (auto& thread : copyThreads) {
                    thread.join();
                }
            }
    };

    template <typename T>
    constexpr std::string to_string(const T& i) {
        return std::to_string(i);
    }

    template <>
    inline std::string to_string(const std::string& tile) {
        return tile;
    }

    template <typename TileMapType>
    constexpr void display(const TileMapType& tileMap, const std::string& title = "") {
        std::cout << title << (title != "" ? '\n' : '\0');
        for (int k = 0; k < tileMap.height(); ++k) {
            std::cout << "Layer " << k << '\n';
            for (int j = 0; j < tileMap.length(); ++j) {
                for (int i = 0; i < tileMap.width(); ++i) {
                    std::cout << to_string(tileMap(i, j, k)) << " ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    template <int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapF = TileMap<float, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapD = TileMap<double, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;
} /* StealthTileMap */

#endif
