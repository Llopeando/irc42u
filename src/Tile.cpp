#include "../include/Tile.hpp"

namespace mlx{

Tile::Tile(glm::uvec2 pos, glm::uvec2 size, std::string text, int col, TileType type)
{
	tiles = nullptr;
	this->pos = pos;
	this->size = size;
	this->text = text;
	this->col = col;
	this->type = type;
}

Tile::Tile(const Tile& tile)
{
	pos = tile.getPos();
	size = tile.getSize();
	text = tile.getText();
	col = tile.getCol();
	type = tile.getType();
}

glm::uvec2 Tile::getPos() const
{
	return pos;
}
glm::uvec2 Tile::getSize() const
{
	return size;
}
std::string Tile::getText() const
{
	return text;
}
int Tile::getCol() const
{
	return col;
}

TileType Tile::getType()const
{
	return type;
}

}