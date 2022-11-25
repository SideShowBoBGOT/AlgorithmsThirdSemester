cmake_minimum_required(VERSION 3.23)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled1 main.cpp GameSingletons/TGame.cpp GameSingletons/TGame.h Models/TGameState.h States/TMainMenu.cpp States/TMainMenu.h GameSingletons/TGameStateMachine.cpp GameSingletons/TGameStateMachine.h Patterns/TSingleton.h Models/TVisualObject.cpp Models/TVisualObject.h GameSingletons/TTextureManager.cpp GameSingletons/TTextureManager.h GameSingletons/TInputHandler.cpp GameSingletons/TInputHandler.h States/TSettings.cpp States/TSettings.h)

target_link_libraries(untitled1 SDL2main SDL2 GL)