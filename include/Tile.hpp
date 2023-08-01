#include "../libraries/glm/glm.hpp"
#include <iostream>

namespace mlx{

enum TileType{
	eUsers,
	eChannels,
	eInfo,
	eServer,
};

class Tile{
	public:
		Tile(const Tile& tile);
		Tile(glm::ivec2 pos, glm::ivec2 size, std::string text, int col, TileType type);
		~Tile(){}

		glm::ivec2 getPos() const;
		glm::ivec2 getSize() const;
		std::string getText() const;
		int getCol() const;
		TileType getType() const;
		bool	isTouching(glm::ivec2 point);

	private:
		glm::ivec2 pos;
		glm::ivec2 size;
		std::string text;
		int col;
		TileType type;
		std::vector<Tile> *tiles;
};



}