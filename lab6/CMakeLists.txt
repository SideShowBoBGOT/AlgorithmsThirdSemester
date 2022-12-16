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
VisualModels/TAIBlock.cpp
VisualModels/TAIBlock.h
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
VisualModels/TVisualCard.cpp
VisualModels/TVisualCard.h
GameSingletons/TLogic.cpp
GameSingletons/TLogic.h
Logic/TSession.cpp
Logic/TSession.h
Logic/TPlayer.cpp
Logic/TPlayer.h
Logic/TCard.cpp
Logic/TCard.h
        Logic/Enums/NNCardType.h
        Logic/Enums/NNCardValue.h
        Logic/Enums/NNDifficulty.h VisualModels/TAnimationControl.h VisualModels/TAnimationControl.cpp Other/TTimer.h Other/TTimer.cpp Logic/Notify/INotify.h Logic/AI/TAI.h Logic/AI/TAI.cpp Logic/Notify/SNextTurnNotify.h Logic/Notify/SNextTurnNotify.cpp Logic/Notify/SAIEndTurn.h Logic/Notify/SAIEndTurn.cpp Other/TThreadSafeQueue.h Other/TPermutator.h Logic/Tests/TTakeTest.h Logic/Tests/TTakeTest.cpp Logic/Tests/ITest.h)

target_link_libraries(untitled1 SDL2main SDL2 GL)