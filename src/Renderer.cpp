
#include "../include/Renderer.hpp"

#define TEXT_COL 0xF5F5F5
#define BACKGROUND_COL 0xF1F6F9
#define MAINBODY_COL 0x9BA4B4
#define BODY_COL 0x526D82

RenderEngine::RenderEngine(sd::ServerData *serverData)
{
	this->serverData = serverData;
	t_pipelineCreateInfo info = {};
	info.width = 1920;
	info.height = 1080;
	initMlx(&mlx, (char *)"IRC", &info);

	winSize.x = 1920;
	winSize.y = 1080;
	indexs = {};

	bmlx_create_img(mlx.mlx, &image, winSize.x, winSize.y);

	glm::vec2 size = {winSize.x / 3, winSize.y};

	mlx::Tile users({0,0}, size,"Users", MAINBODY_COL, mlx::TileType::eUsers);
	mlx::Tile channels({winSize.x / 3, 0}, size, "Channels", MAINBODY_COL, mlx::TileType::eChannels);
	mlx::Tile information({winSize.x / 3 * 2, 0}, size, "Info", MAINBODY_COL,mlx::TileType::eInfo);
	tiles.push_back(users);
	tiles.push_back(channels);
	tiles.push_back(information);

	std::cout << "Users: pos" << tiles[0].getPos().x << ',' << tiles[0].getPos().y << " size: " << tiles[0].getSize().x << ',' << tiles[0].getSize().y << '\n';
	std::cout << "Channels: pos" << tiles[1].getPos().x << ',' << tiles[1].getPos().y << " size: " << tiles[1].getSize().x << ',' << tiles[1].getSize().y << '\n';
}

RenderEngine::~RenderEngine()
{
	
}

void	RenderEngine::render()
{
	mlx_loop_hook(mlx.mlx, renderFrameWrapper, reinterpret_cast<void *>(this));
	mlx_hook(mlx.win, 2, 1L << 0, eventspressWrapper,  reinterpret_cast<void *>(this));
	mlx_hook(mlx.win, 3, 1L << 0, eventsrealeseWrapper,  reinterpret_cast<void *>(this));
	//mlx_hook(win, 17, 1L << 5, windowClosedWrapper,reinterpret_cast<void *>(this));
	//mlx_hook(win, 4, 1L << 0, mouseEventWrapper,  reinterpret_cast<void *>(this));
	mlx_mouse_hook(mlx.win, mouseEventWrapper,  reinterpret_cast<void *>(this));
	mlx_loop(mlx.mlx);
}

void printFPS() {
	static int frameCount = 0;
	static auto startTime = std::chrono::steady_clock::now();
	auto endTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	
	if (elapsedTime >= 1000) {
		std::cout << "FPS: " << frameCount << std::endl;
		frameCount = 0;
		startTime = endTime;
	}
	frameCount++;
}

void RenderEngine::renderFrame()
{
	for (auto tile : tiles)
	{
		renderTile(tile);
	}
	mlx_put_image_to_window(mlx.mlx, mlx.win, image.img, 0,0);
	for (auto tile : tiles)
	{
		renderText(tile);
	}
}

void RenderEngine::renderTile(const mlx::Tile& tile)
{
	drawRectWithPer(&image, tile.getPos().x, tile.getPos().y, tile.getSize().x, tile.getSize().y, 2, tile.getCol(), BACKGROUND_COL);
	if (mlx::TileType::eUsers == tile.getType())
	{
		renderClients(tile);
	}
	else if (mlx::TileType::eChannels == tile.getType())
	{
		renderChannels(tile);
	}
}

void RenderEngine::renderText(const mlx::Tile& tile)
{
	int stringSize = tile.getText().size();
	mlx_string_put(mlx.mlx, mlx.win, tile.getPos().x + tile.getSize().x / 2 - stringSize, tile.getPos().y + 15, TEXT_COL, strdup(tile.getText().c_str()));

	if (mlx::TileType::eUsers == tile.getType())
	{
		renderClientsTexts(tile);
	}
	else if (mlx::TileType::eChannels == tile.getType())
	{
		renderChannelsTexts(tile);
	}
}

void RenderEngine::renderClients(const mlx::Tile& tile)
{
	uint32_t numClients = serverData->getNumOfClients();
	uint32_t height = 20;
	for (uint32_t i = 1; i < numClients; i++)
	{
		drawRectWithPer(&image, tile.getPos().x + 2, tile.getPos().y + i * height, tile.getSize().x - 4, height, 2, BODY_COL, MAINBODY_COL);
	}
}

void RenderEngine::renderClientsTexts(const mlx::Tile& tile)
{
	uint32_t numClients = serverData->getNumOfClients();
	uint32_t height = 20;
	for (uint32_t i = 1; i < numClients; i++)
	{
		//drawRectWithPer(&image, 2, tile.getPos().y + (i + 1) * height, tile.getSize().x - 4, height, 2, tile.getCol(), 0xF1F6F9);
		int stringSize = (*serverData)[(sd::clientIt)i].getNickname().size();
		mlx_string_put(mlx.mlx, mlx.win, tile.getPos().x + tile.getSize().x / 2 - stringSize, tile.getPos().y + i * height + 15, TEXT_COL, strdup((*serverData)[(sd::clientIt)i].getNickname().c_str()));
	}
}

void RenderEngine::renderChannels(const mlx::Tile& tile)
{
	uint32_t numChann = serverData->getNumOfChannels();
	uint32_t height = 20;
	for (uint32_t i = 1; i < numChann; i++)
	{
		drawRectWithPer(&image, tile.getPos().x + 2, tile.getPos().y + i * height, tile.getSize().x - 4, height, 2, BODY_COL, MAINBODY_COL);
	}
}

void RenderEngine::renderChannelsTexts(const mlx::Tile& tile)
{
	uint32_t numChann = serverData->getNumOfChannels();
	uint32_t height = 20;
	for (uint32_t i = 1; i < numChann; i++)
	{
		//drawRectWithPer(&image, 2, tile.getPos().y + (i + 1) * height, tile.getSize().x - 4, height, 2, tile.getCol(), 0xF1F6F9);
		int stringSize = (*serverData)[(sd::channIt)i].getName().size();
		mlx_string_put(mlx.mlx, mlx.win, tile.getPos().x + tile.getSize().x / 2 - stringSize, tile.getPos().y + i * height + 15, TEXT_COL, strdup((*serverData)[(sd::channIt)i].getName().c_str()));
	}
}


uint32_t RenderEngine::getPosition(glm::vec2 pos)
{
	(void)pos;
	return 0;
}

void	RenderEngine::eventpress(int keycode)
{
	(void)keycode;
	//if (keycode == 13)
	//	keyboard.up = true;
	//else if (keycode == 0)
	//	keyboard.left = true;
	//else if (keycode == 1)
	//	keyboard.down = true;
	//else if (keycode == 2)
	//	keyboard.right = true;
}

void	RenderEngine::eventrealese(int keycode)
{
	(void)keycode;
	//if (keycode == 13)
	//	keyboard.up = false;
	//else if (keycode == 0)
	//	keyboard.left = false;
	//else if (keycode == 1)
	//	keyboard.down = false;
	//else if (keycode == 2)
	//	keyboard.right = false;
}


void	RenderEngine::mouseEvent(int keycode, int x, int y)
{
	(void)x;
	(void)y;
	if (keycode == 1)
	{
		glm::ivec2 pos;
		//std::cout << "self =" << this << "\n";
		//std::cout << "[" << mlx << "]\n";
		mlx_mouse_get_pos(mlx.win, &pos.x, &pos.y);
	}
}

void	RenderEngine::windowClosed()
{
	//END PROGRAM
}

int RenderEngine::renderFrameWrapper(void *param)
{
	RenderEngine *renderer = static_cast<RenderEngine *>(param);
	renderer->renderFrame();
	return 0;
}

int RenderEngine::eventspressWrapper(int keycode, void *param)
{
	RenderEngine *renderer = static_cast<RenderEngine *>(param);
	renderer->eventpress(keycode);
	return 0;
}

int RenderEngine::eventsrealeseWrapper(int keycode, void *param)
{
	RenderEngine *renderer = static_cast<RenderEngine *>(param);
	renderer->eventrealese(keycode);
	return 0;
}

int RenderEngine::mouseEventWrapper(int mousecode, int x, int y, void *param)
{
	RenderEngine *renderer = static_cast<RenderEngine *>(param);
	renderer->mouseEvent(mousecode, x, y);
	return 0;
}

int windowClosedWrapper(void *param)
{
	RenderEngine *renderer = static_cast<RenderEngine *>(param);
	renderer->windowClosed();
	return 0;
}