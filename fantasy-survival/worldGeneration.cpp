#include <SFML/Graphics.hpp>
#include "tilemap.cpp"
#include <iostream>
#include <vector>
#include <FastNoise/FastNoise.h>

const unsigned int mapRenderDistance = 3;
const int mapSize = 256;

const unsigned int chunkLoaderSize = 64;
const unsigned int chunkLoaderArea = chunkLoaderSize * chunkLoaderSize;

const int chunkSize = 24;
const unsigned int chunkArea = chunkSize * chunkSize;

const unsigned int tileSize = 16;

struct chunk {
	int biome;
	bool generated = false;
};

std::vector<sf::Vector2i> mapPositions;

TileMap tilemapTiles;
TileMap tilemapObjects;

std::vector<std::vector<chunk>> saveData(mapRenderDistance* mapRenderDistance, std::vector<chunk>(mapSize* mapSize));

chunk getChunk(sf::Vector2i chunkPosition, sf::Vector2i mapPosition);
int cantorPairing(int a, int b);

// -------------------------- GENERATE WORLD --------------------------
int generateWorld() {
	sf::Vector2i mapPosition;
	FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("DQAEAAAAAAAAQAkAAAAAAD8AAAAAAA==");

	for (size_t i = 0; i < mapRenderDistance * mapRenderDistance; i++) {
		mapPosition = sf::Vector2i(i / mapRenderDistance, i % mapRenderDistance) - sf::Vector2i(mapRenderDistance / 2, mapRenderDistance / 2);
		mapPositions.push_back(mapPosition);

		std::vector<float> noiseOutput(mapSize * mapSize);
		fnGenerator->GenUniformGrid2D(noiseOutput.data(), mapPosition.x * mapSize, mapPosition.y * mapSize, mapSize, mapSize, 0.005f, 1);
		std::cout << "Map loaded: (" << mapPosition.x << " " << mapPosition.y << ")" << std::endl;

		for (size_t j = 0; j < mapSize * mapSize; j++) {
			if (noiseOutput[j] > 0) {
				saveData[i][j].biome = 1;
			}
			else {
				saveData[i][j].biome = 0;
			}
		}
	}

	/*
	for (size_t i = 0; i < saveData.size(); i++)
	{
		for (size_t j = 0; j < saveData[i].size(); j++)
		{
			std::cout << saveData[i][j].biome;
		}
		std::cout << std::endl;
	}

	*/
	return 0;
}

std::vector<sf::Vector2i> regionArea(chunkLoaderSize* chunkLoaderSize);

// initializes the region around the camera
int initializeRegion() {
	sf::Vector2i cellPosition;
	for (size_t i = 0; i < chunkLoaderSize * chunkLoaderSize; i++) {
		cellPosition = sf::Vector2i(i / chunkLoaderSize, i % chunkLoaderSize) - sf::Vector2i(chunkLoaderSize / 2, chunkLoaderSize / 2);
		regionArea[i] = cellPosition;
	}

	std::cout << "Chunks initialized" << std::endl;
	return 0;
}

// loads the area around the camera
int updateRegion(sf::Vector2i centralPosition) {
	sf::Vector2i globalCellPosition;
	sf::Vector2i cellPosition;
	sf::Vector2i chunkPosition;
	sf::Vector2i mapPosition;

	chunk chunkPtr;
	unsigned int random;

	int tiles[chunkLoaderArea];
	int objects[chunkLoaderArea];

	for (size_t i = 0; i < chunkLoaderSize * chunkLoaderSize; i++)
	{
		globalCellPosition = centralPosition + regionArea[i];

		chunkPosition.x = floor(globalCellPosition.x / (float)chunkSize);
		chunkPosition.y = floor(globalCellPosition.y / (float)chunkSize);

		mapPosition.x = floor(chunkPosition.x / (float)mapSize);
		mapPosition.y = floor(chunkPosition.y / (float)mapSize);

		chunkPosition.x = chunkPosition.x % mapSize;
		chunkPosition.x = chunkPosition.x < 0 ? chunkPosition.x + mapSize : chunkPosition.x;
		chunkPosition.y = chunkPosition.y % mapSize;
		chunkPosition.y = chunkPosition.y < 0 ? chunkPosition.y + mapSize : chunkPosition.y;

		cellPosition.x = globalCellPosition.x % chunkSize;
		cellPosition.x = globalCellPosition.x < 0 ? globalCellPosition.x + chunkSize : globalCellPosition.x;
		cellPosition.y = globalCellPosition.y % chunkSize;
		cellPosition.y = globalCellPosition.y < 0 ? globalCellPosition.y + chunkSize : globalCellPosition.y;

		chunkPtr = getChunk(chunkPosition, mapPosition);

		srand(cantorPairing(globalCellPosition.x, globalCellPosition.y));

		tiles[i] = rand() % 9;

		if (chunkPtr.biome == 1) {
			random = rand() % 3;
			if (random == 0) {
				objects[i] = 197 + rand() % 16;
			}
		}
	}

	tilemapTiles.load("Assets/Tileset/tileset.png", sf::Vector2u(tileSize, tileSize), tiles, chunkLoaderSize, chunkLoaderSize);
	tilemapObjects.load("Assets/Tileset/tileset.png", sf::Vector2u(tileSize, tileSize), objects, chunkLoaderSize, chunkLoaderSize);

	return 0;
}

sf::Vector2f getScreenCentre() {
	return sf::Vector2f(chunkLoaderSize * tileSize * 0.5, chunkLoaderSize * tileSize * 0.5);
}

chunk getChunk(sf::Vector2i chunkPosition, sf::Vector2i mapPosition) {
	int chunkID = chunkPosition.x * chunkSize + chunkPosition.y;
	for (size_t i = 0; i < mapPositions.size(); i++)
	{
		if (mapPositions[i] == mapPosition) {
			return saveData[i][chunkID];
		}
	}
}

int cantorPairing(int a, int b) {
	int A = a >= 0 ? 2 * a : -2 * a - 1;
	int B = b >= 0 ? 2 * b : -2 * b - 1;
	return (A + B) * (A + B + 1) / 2 + A * A;
}