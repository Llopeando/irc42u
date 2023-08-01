
#include "../include/Renderer.hpp"

#define TEXT_COL 0xF5F5F5
#define BACKGROUND_COL 0xF1F6F9
#define MAINBODY_COL 0x9BA4B4
#define BODY_COL 0x526D82

#define HEIGHT 20
#define HEADER 30

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
	currentInfo = mlx::eServer;
	infoIndex = 0;

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
	else if (mlx::TileType::eInfo == tile.getType())
	{
		renderInfo(tile);
	}
}
void RenderEngine::renderInfo(const mlx::Tile& tile)
{
	if (currentInfo == mlx::eServer)
	{
		renderServerInfo(tile);
	}
	else if (currentInfo == mlx::eUsers)
	{
		renderUsersInfo(tile);
	}
	else if (currentInfo == mlx::eChannels)
	{
		//renderChannelsInfo(tile);
	}
}

void RenderEngine::renderUsersInfo(const mlx::Tile& tile)
{
	if (infoIndex == 0 || infoIndex >= serverData->getNumOfClients())
	{
		std::vector<std::string> lines = {
			"Number of active users: " + std::to_string(serverData->getNumOfClients() - 1),
			"Number of unactive users: " + std::to_string(serverData->getNumOfAwayClients() - 1),
		};
		printLines(tile, lines);
	}
	else if (infoIndex < serverData->getNumOfClients())
	{
		Client temp = Client((*serverData)[sd::clientIt(infoIndex)]);
		renderUser(tile, temp);
	}
}

void RenderEngine::renderUser(const mlx::Tile& tile, const Client& user)
{
	
	std::vector<std::string> lines = {
		"Nickname: " + user.getNickname(),
		"Username: " + user.getUsername(),
		"Hostname: " +  user.getHostname(),
		"Role: " + std::to_string(user.getRole()),
		"Message: " + std::to_string(user.getAwayStatus()),
	};
	if (user.getAwayStatus())
		lines[3] += "MESSAGE: " + user.getAwayMsg();
	printLines(tile, lines);
}

void RenderEngine::renderServerInfo(const mlx::Tile& tile)
{
	const sd::t_serverInput& info = serverData->getConfig();
	std::vector<std::string> lines = {
		"CHANTYPES: " + info.chantypes,
		"PREFIX: " + info.prefix,
		"MODES: " + std::to_string(info.modes),
		"CHANLIMIT: " + std::to_string(info.chanlimit),
		"NICKLEN: " + std::to_string(info.nicklen),
		"USERLEN: " + std::to_string(info.userlen),

		"HOSTLEN: " + std::to_string(info.hostlen),
		"TOPICLEN: " + std::to_string(info.topiclen),
		"CHANNELLEN: " + std::to_string(info.channellen),
		"MAXUSERS: " + std::to_string(info.maxusers),
		"MAXUSERSCHAN: " + std::to_string(info.maxuserschan),
		"VERSION: " + info.version,
		"USERLEN: " + std::to_string(info.userlen),
		"VERSION_COMMENTS: " + info.versionComments,
	};
	printLines(tile, lines);
}

void RenderEngine::renderClients(const mlx::Tile& tile)
{
	uint32_t numClients = serverData->getNumOfClients();
	for (uint32_t i = 1; i < numClients; i++)
	{
		drawRectWithPer(&image, tile.getPos().x + 2, tile.getPos().y + (i - 1) * HEIGHT + HEADER, tile.getSize().x - 4, HEIGHT, 2, BODY_COL, MAINBODY_COL);
	}
}


void RenderEngine::renderClientsTexts(const mlx::Tile& tile)
{
	uint32_t numClients = serverData->getNumOfClients();
	for (uint32_t i = 1; i < numClients; i++)
	{
		//drawRectWithPer(&image, 2, tile.getPos().y + (i + 1) * height, tile.getSize().x - 4, height, 2, tile.getCol(), 0xF1F6F9);
		int stringSize = (*serverData)[(sd::clientIt)i].getNickname().size();
		mlx_string_put(mlx.mlx, mlx.win, tile.getPos().x + tile.getSize().x / 2 - stringSize, tile.getPos().y + (i - 1) * HEIGHT + HEADER + 15, TEXT_COL, strdup((*serverData)[(sd::clientIt)i].getNickname().c_str()));
	}
}

void RenderEngine::renderChannels(const mlx::Tile& tile)
{
	uint32_t numChann = serverData->getNumOfChannels();
	for (uint32_t i = 1; i < numChann; i++)
	{
		drawRectWithPer(&image, tile.getPos().x + 2, tile.getPos().y + (i - 1) * HEIGHT + HEADER, tile.getSize().x - 4, HEIGHT, 2, BODY_COL, MAINBODY_COL);
	}
}

void RenderEngine::renderChannelsTexts(const mlx::Tile& tile)
{
	uint32_t numChann = serverData->getNumOfChannels();
	for (uint32_t i = 1; i < numChann; i++)
	{
		//drawRectWithPer(&image, 2, tile.getPos().y + (i + 1) * height, tile.getSize().x - 4, height, 2, tile.getCol(), 0xF1F6F9);
		int stringSize = (*serverData)[(sd::channIt)i].getName().size();
		mlx_string_put(mlx.mlx, mlx.win, tile.getPos().x + tile.getSize().x / 2 - stringSize, tile.getPos().y + (i - 1) * HEIGHT + HEADER + 15, TEXT_COL, strdup((*serverData)[(sd::channIt)i].getName().c_str()));
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
	if (keycode != 1)
		return;
	for (int i = 0; i < 3; i++)
	{
		if (tiles[i].isTouching({x,y}))
		{
			if (tiles[i].getType() == mlx::eUsers)
			{
				if (currentInfo != mlx::eUsers)
				{
					currentInfo = mlx::eUsers;
					infoIndex = 0;
					return ;
				}
				int index = (y - HEADER) / HEIGHT;
				std::cout << "index: " << index << " y " << y << "\n";
				if (index >= 0 && index < (int)serverData->getNumOfClients() - 1)
					infoIndex = index + 1;
				else
					infoIndex = 0;
				currentInfo = mlx::eUsers;
			}
			else if (tiles[i].getType() == mlx::eChannels && currentInfo == mlx::eChannels)
			{
				if (currentInfo != mlx::eChannels)
				{
					currentInfo = mlx::eChannels;
					infoIndex = 0;
					return ;
				}
				int index = (x - HEADER) / HEIGHT;
				if (index > 0 && index < (int)serverData->getNumOfChannels())
					infoIndex = x;
				else
					infoIndex = 0;
				currentInfo = mlx::eChannels;
			}
			else if (tiles[i].getType() == mlx::eInfo)
			{
				currentInfo = mlx::eServer;
				infoIndex = 0;
				return ;
			}
		}
	}
}

void RenderEngine::printLines(const mlx::Tile& tile, const std::vector<std::string>&lines)
{
	for (uint32_t i = 0; i < lines.size(); i++)
	{
		mlx_string_put(mlx.mlx, mlx.win, tile.getPos().x + (tile.getSize().x / 2) - lines[i].size(), i * HEIGHT + HEADER, TEXT_COL, (char *)lines[i].c_str());
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