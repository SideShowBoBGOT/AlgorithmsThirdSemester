#include "Core/TGame.h"

int main() {
	auto game = new Core::TGame();
	
	if(not game->Init("Lab6", 100, 100, 640, 480, 0)) return 1;
	
	while(game->IsRunning()) {
		game->HandleEvents();
		game->Update();
		game->Render();
	}
	
	game->Close();
	return 0;
}