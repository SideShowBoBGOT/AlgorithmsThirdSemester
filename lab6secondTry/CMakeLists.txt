cmake_minimum_required(VERSION 3.23)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

add_executable(
untitled1
main.cpp
GameSingletons/TGame.cpp
GameSingletons/TGame.h
States/TMainMenu.cpp
States/TMainMenu.h
GameSingletons/TGameStateMachine.cpp
GameSingletons/TGameStateMachine.h
Patterns/TSingleton.h
VisualModels/Enums/NNState.h
VisualModels/Enums/NNAlign.h
VisualModels/IControl.h
VisualModels/TControl.cpp
VisualModels/TControl.h
VisualModels/TParent.cpp
VisualModels/TParent.h
VisualModels/TAutoAlignArea.cpp
VisualModels/TAutoAlignArea.h
GameSingletons/TTextureManager.cpp
GameSingletons/TTextureManager.h
GameSingletons/TInputHandler.cpp
GameSingletons/TInputHandler.h
States/TSettings.cpp
States/TSettings.h
States/TBoardScreen.cpp
States/TBoardScreen.h
Other/NNFileSystem.cpp
Other/NNFileSystem.h
BoardScreen/TVisualCard.cpp
BoardScreen/TVisualCard.h
GameSingletons/TLogic.cpp
GameSingletons/TLogic.h
Logic/TSession.cpp
Logic/TSession.h
Logic/TPlayer.cpp
Logic/TPlayer.h
Logic/TCard.cpp
Logic/TCard.h
Logic/NNCardType.h
Logic/NNCardValue.h
Logic/NNDifficulty.h)

target_link_libraries(untitled1 SDL2main SDL2 GL)