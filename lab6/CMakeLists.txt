cmake_minimum_required(VERSION 3.23)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab6 main.cpp Core/Models/TCardObject.cpp Core/Models/TCardObject.h Core/Models/NNCardValue.h Core/Models/NNCardGroup.h Core/Models/TBoardObject.cpp Core/Models/TBoardObject.h Core/Config/TConfig.cpp Core/Config/TConfig.h Core/Models/TPlayerObject.cpp Core/Models/TPlayerObject.h Core/Controllers/TInputHandler.cpp Core/Controllers/TInputHandler.h Core/Settings/TSettings.cpp Core/Settings/TSettings.h Core/Settings/NNGameSettigs.h Core/LogErrors/NNLogErrors.h Core/Patterns/TSingleton.h Core/AI/TAI.cpp Core/AI/TAI.h Core/TGameState/TGameState.cpp Core/TGameState/TGameState.h)

target_link_libraries(lab6 SDL2main SDL2 GL)
