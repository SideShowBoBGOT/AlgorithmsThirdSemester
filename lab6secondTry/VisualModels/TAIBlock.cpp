//
// Created by choleraplague on 02.12.22.
//

#include "TAIBlock.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../Other/NNFileSystem.h"


static int constexpr s_iObjectWidth = 158;
static int constexpr s_iObjectHeight = 35;
static int constexpr s_iCardWidth = 101;
static int constexpr s_iCardHeight = 144;
static const std::string s_sBoardScreenPath = "BoardScreen/";


TAIBlock::TAIBlock(std::string name) {
	auto width = 3*s_iCardWidth;
	auto height = s_iObjectHeight + 10 + s_iCardHeight;
	
	Width(width);
	Height(height);

	#define INIT_TEXTURE(xx, name) \
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + name+"/"+"Normal").c_str(), name+"Normal", TGame::Get()->Renderer());\
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + name + "/"+"Over").c_str(), name+"Over", TGame::Get()->Renderer());\
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + name+"/"+"Selected").c_str(), name+"Selected", TGame::Get()->Renderer());\
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + name+"/"+"OverSelected").c_str(), name+"OverSelected", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, name + "Normal");\
		xx->StateTexture(NState::Over, name + "Over");\
		xx->StateTexture(NState::Selected, name + "Selected");\
		xx->StateTexture(NState::OverSelected, name + "OverSelected");
	
	#define INIT(xx, parent, type, dx, dy, sx, sy, w, h) \
		xx = parent->CreateObject<type>();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(sx);\
		xx->Sy(sy);\
		xx->Width(w);\
		xx->Height(h);\
		xx->Renderer(TGame::Get()->Renderer()); \
	
		INIT(LabelArea, this, TAutoAlignArea, 0, 0, 0, 0, width, s_iObjectHeight);
		INIT(Cards, this, TAutoAlignArea, 0, s_iObjectHeight + 10, 0, 0, width, s_iCardHeight);
		
		#define INIT_LABEL(xx) \
			INIT(xx, LabelArea, TControl, 0, 0, 0, 0, s_iObjectWidth, s_iObjectHeight); \
			INIT_TEXTURE(xx, name);
	
			INIT_LABEL(Label);
		#undef INIT_LABEL
	#undef INIT
	#undef INIT_TEXTURE
	
	LabelArea->ALignObjects();
}