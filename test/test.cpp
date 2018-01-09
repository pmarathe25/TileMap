#include "TileMap/TileMap.hpp"

float doubleVal(const float& other) {
    return other * 2.0f;
}

int main() {
    StealthTileMap::TileMapF<5, 5, 2> test{};
    StealthTileMap::TileMapF<5, 5, 2> test2{};

    test(0, 0) = 1.5f;
    test(0, 1) = 1.5f;
    test(1, 1) = 1.5f;
    test(0, 0, 1) = 3.1f;
    test(0, 1, 1) = 3.1f;
    test(1, 1, 1) = 3.1f;

    // test2(0, 0, 1) = 3.0f;
    // StealthTileMap::TileMapF<5, 5, 2> test3 = (test + test2) + test;
    // StealthTileMap::display(test3);

    // auto testLayer = StealthTileMap::layer(test, 0);
    // StealthTileMap::display(testLayer);
    // StealthTileMap::display(StealthTileMap::layer(test + test2, 1));

    // Changing a value in the view actually changes the TileMap
    StealthTileMap::block<2, 2>(test)(0, 0, 1) = 2.4f;
    // Construct a separate TileMap from the view.
    auto testView = StealthTileMap::block<2, 2>(test, 0, 1).eval();

    StealthTileMap::display(test, "Original");
    StealthTileMap::display(testView, "TestView");
    StealthTileMap::display(test && test2, "Test && Test2");
    StealthTileMap::display(test || test2, "Test || Test2");
    StealthTileMap::display(!test, "!Test");
    StealthTileMap::display(StealthTileMap::transform<float, float, doubleVal>(test), "Doubled Test");

    // Creating TileMap from array
    std::array<float, 4> init{{0, 1, 2, 3}};
    StealthTileMap::TileMapF<2, 2> testInit{init};
    StealthTileMap::display(testInit, "Initializer List Test");
}
