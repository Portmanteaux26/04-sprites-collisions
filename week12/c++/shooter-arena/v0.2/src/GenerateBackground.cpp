#include <SFML/Graphics.hpp>
#include "Arena.h"

int generateBackground(VertexArray& refVA, IntRect arena)
{

	// size for tile, texture
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTICES_QUAD = 4;

	// size for game world
	int gameWidth = arena.width / TILE_SIZE;
	int gameHeight = arena.height / TILE_SIZE;

	// set primitive for refVA
	refVA.setPrimitiveType(Quads);

	// set vertex array size
	refVA.resize(gameWidth * gameHeight * VERTICES_QUAD);

	// set starting point for vertex array
	int currentVA = 0;

	// prepare vertex array
	for (int w = 0; w < gameWidth; w++) {
		// inner - prepare relative to height
		for (int h = 0; h < gameHeight; h++) {
			// set vertex posn in current quad
			refVA[currentVA + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			refVA[currentVA + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			refVA[currentVA + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			refVA[currentVA + 3].position = Vector2f((w * TILE_SIZE), (h * TILE_SIZE) + TILE_SIZE);

			// define texture posn for quad in current iteration
			if (h == 0 || h == gameHeight - 1 || w == 0 || w == gameWidth - 1) {
				// use wall texture
				refVA[currentVA + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				refVA[currentVA + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				refVA[currentVA + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				refVA[currentVA + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			} else {
				// pick a random tile
				srand((int)time(0) + h * w - h);
				int randTile = (rand() % TILE_TYPES);
				int vertOffset = randTile * TILE_SIZE;

				refVA[currentVA + 0].texCoords = Vector2f(0, 0 + vertOffset);
				refVA[currentVA + 1].texCoords = Vector2f(TILE_SIZE, 0 + vertOffset);
				refVA[currentVA + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + vertOffset);
				refVA[currentVA + 3].texCoords = Vector2f(0, TILE_SIZE + vertOffset);
			}

			// update current point for vertex array, ready for next set of vertices
			currentVA = currentVA + VERTICES_QUAD;
		}
	}

	return TILE_SIZE;
}
