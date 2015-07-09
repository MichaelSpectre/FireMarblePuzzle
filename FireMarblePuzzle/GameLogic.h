// =======================================
// "GameLogic.h"
// Handles logical updates to game objects
// (See implementation for more info)
// =======================================
#pragma once

// Forward declarations to other classes
class CGame;
class CInput;
class CSprite;

class CGameLogic
{
public:
	CGameLogic();
	~CGameLogic();

	BOOL Init(CGame *game);
	VOID Update(CGame *game, CInput *input);
	std::vector<CSprite*> GetSprites();

private:
	VOID CheckCombo();
	std::vector<int> CreateCombination();

	std::vector<CSprite*> m_sprites;
	std::vector<int> m_nodes;
	std::vector<int> m_combo;
};

