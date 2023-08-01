#include "../include/Tile.hpp"

namespace mlx{

Tile::Tile(glm::ivec2 pos, glm::ivec2 size, std::string text, int col, TileType type)
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

glm::ivec2 Tile::getPos() const
{
	return pos;
}
glm::ivec2 Tile::getSize() const
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

bool Tile::isTouching(glm::ivec2 point)
{
	if (point.x > pos.x && point.x < pos.x + size.x 
		&& point.y > pos.y && point.y < pos.y + size.y)
		return true;
	return false;
}

}