#pragma once

extern "C" {
#include "../libraries/mlx/betterMlx.h"
}

#include "../libraries/includes/header.h"
#include "Tile.hpp"
#include <chrono>
#include <mutex>
#include <vector>

struct Keyboard{
	bool up;
	bool down;
	bool left;
	bool right;
};

#include "ServerData.hpp"

class RenderEngine{
	public:
		RenderEngine(sd::ServerData *serverData);
		~RenderEngine();
		
		void		render();
		void		renderFrame();
		
		void		sendEntity(Entity &entity);
		
		void		eventpress(int keycode);
		void		eventrealese(int keycode);
		void		mouseEvent(int mousecode, int x, int y);
		void		windowClosed();

		static int	windowClosedWrapper(void *param);
		static int	renderFrameWrapper(void *param);
		static int	eventspressWrapper(int keycode, void *param);
		static int	eventsrealeseWrapper(int keycode, void *param);
		static int	mouseEventWrapper(int mousecode, int x, int y, void *param);
		
		void		createWindow();
		void		updatePosition();
		void		initEntityAtlas();
		
		uint32_t	getPosition(glm::vec2 pos);
		void		renderTile(const mlx::Tile& tile);
		void		renderText(const mlx::Tile& tile);

		void		renderClients(const mlx::Tile& tile);
		void		renderClientsTexts(const mlx::Tile& tile);
		void		renderChannels(const mlx::Tile& tile);
		void		renderChannelsTexts(const mlx::Tile& tile);
	private:
		t_mlx mlx;
		t_image image;
		std::vector<mlx::Tile> tiles;
		glm::uvec2 winSize;
		sd::ServerData *serverData;
		glm::ivec3 indexs;//user, chann, info
};


