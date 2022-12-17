#include "GameSingletons/TGame.h"
#include "GameSingletons/TInputHandler.h"
#include "GameSingletons/TTextureManager.h"
#include "GameSingletons/TLogic.h"
#include "Logic/Tests/TTakeTest.h"
int main() {
//	TTakeTest().Test();
	auto stateMachine = TGameStateMachine::Get();
	auto logic = TLogic::Get();
	auto texture = TTextureManager::Get();
	auto input = TInputHandler::Get();
	auto game = TGame::Get();
	if(not game->Init("LabSix", 100, 100, 1600, 720, 0)) return 1;
	
	while(game->IsRunning()) {
		game->Clean();
		game->HandleEvents();
		game->Render();
	}
	game->Clean();
	
	delete game;
	delete input;
	delete texture;
	delete logic;
	delete stateMachine;
	return 0;
}
