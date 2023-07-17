#include "../libraries/glm/glm.hpp"
#include <iostream>

namespace mlx{

enum TileType{
	eUsers,
	eChannels,
	eInfo,
};



class Tile{
	public:
		Tile(const Tile& tile);
		Tile(glm::uvec2 pos, glm::uvec2 size, std::string text, int col, TileType type);
		~Tile(){}

		glm::uvec2 getPos() const;
		glm::uvec2 getSize() const;
		std::string getText() const;
		int getCol() const;
		TileType getType() const;

	private:
		glm::uvec2 pos;
		glm::uvec2 size;
		std::string text;
		int col;
		TileType type;
		std::vector<Tile> *tiles;
};



}