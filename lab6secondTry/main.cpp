#include "GameSingletons/TGame.h"
int main() {
	auto game = TGame::Get();
	if(not game->Init("LabSix", 100, 100, 1280, 720, 0)) return 1;
	
	while(game->IsRunning()) {
		game->Clean();
		game->HandleEvents();
		game->Render();
	}
	game->Clean();
	
	delete game;
	return 0;
}
