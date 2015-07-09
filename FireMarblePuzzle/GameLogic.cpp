// =======================================
// "GameLogic.h"
// Handles logical updates to game objects
// =======================================

#include "stdafx.h"
#include "GameLogic.h"
#include "Game.h"
#include "Input.h"
#include "Sprite.h"
#include <random>
#include <algorithm>
#include <sstream>

CGameLogic::CGameLogic()
{
}

CGameLogic::~CGameLogic()
{
	// Free sprite resources
	for (auto spr : m_sprites) {
		if (spr != nullptr) {
			delete spr;
			spr = nullptr;
		}
	}
	m_sprites.clear();
}

// Just a quick helper function
BOOL LoadSprite(std::wstring path, std::wstring filename, CD2D *renderer, CSprite **ppRet)
{
	std::wostringstream ss;
	ss << path.c_str() << L"\\" << filename.c_str();
	CSprite *spr = new CSprite();
	auto bRet = spr->Load(ss.str(), renderer);
	if (!bRet) {
		std::wostringstream ssMsg(L"Failed to load '");
		ssMsg << ss.str().c_str() << "'!";
		MessageBox(NULL, ssMsg.str().c_str(), L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	*ppRet = spr;
	return TRUE;
}

// This is where the bones of the project are forged
// (loading sprites, for example)
BOOL CGameLogic::Init(CGame *game)
{
	// Initialize the combination
	m_combo = CreateCombination();
	m_nodes = m_combo;
	for (unsigned i = 0; i < m_nodes.size(); i++)
		m_nodes[i] = i;

	// Load sprites
	auto path = game->GetExecutablePath();

	// Slider (Mid-section)
	CSprite *spr = nullptr;
	if (!LoadSprite(path, L"sliderMid.png", game->GetRenderer(), &spr))
		return FALSE;
	m_sprites.push_back(spr);
	spr->SetPosition(0, 0);

	// Slider (nodes)
	for (auto i = 0; i < 5; i++) {
		spr = nullptr;
		if (!LoadSprite(path, L"sliderNode.png", game->GetRenderer(), &spr))
			return FALSE;
		m_sprites.push_back(spr);
	}

	return TRUE;
}

// This is where the meat sits on the bone, so to speak
// (checking input & updating sprites, for example)
VOID CGameLogic::Update(CGame *game, CInput *input)
{
	// static variables for now, because I'm lazy
	static bool bSpaceKey = false;
	static bool bReturnKey = false;
	static bool bMouseLB = false;
	static bool bMouseRB = false;
	static POINT ptLast = { -1, -1 };
	static int nSelected = -1;
	static std::vector<int> savedNodes;

	// Allow the user to press ESC to exit
	if (input->IsKeyDown(DIK_ESCAPE)) {
		game->Quit();
		return;
	}

	// Allow the user to press SPACE to create a new combo
	if (!bSpaceKey && input->IsKeyDown(DIK_SPACE)) {
		m_combo = CreateCombination();
		bSpaceKey = true;
	} else if (bSpaceKey && !input->IsKeyDown(DIK_SPACE)) {
		bSpaceKey = false;
	}

	// Allow the user to press ENTER to check if they have the right combo
	if (!bReturnKey && input->IsKeyDown(DIK_RETURN)) {
		bReturnKey = true;
	} else if (bReturnKey && !input->IsKeyDown(DIK_RETURN)) {
		bReturnKey = false;
		CheckCombo();
	}

	// Calculate position data
	auto pt = input->GetMousePosition();
	auto fPosX = (float)pt.x / m_sprites[0]->GetWidth();
	
	// Handle mouse button data
	auto bLeftMouse = input->IsMouseDown(0);
	auto bRightMouse = input->IsMouseDown(1);

	// Left mouse button
	if (!bMouseLB && bLeftMouse)
	{
		bMouseLB = true;
		// Check to see if we've clicked within the slider area
		auto p = m_sprites[0]->GetPosition();
		auto w = m_sprites[0]->GetWidth();
		auto h = m_sprites[0]->GetHeight();
		bool bSliderCtrl = (pt.x >= p.x && pt.y >= p.y && pt.x <= p.x + w && pt.y <= p.y + h);

		if (bSliderCtrl) {
			// Check to see if a valid node has been selected
			auto index = int(25.0f * fPosX);
			for (auto c : m_nodes) {
				if (c == index) {
					nSelected = index;
					break;
				}
			}
		}
	} else if (bMouseLB && !bLeftMouse) {
		bMouseLB = false;
		nSelected = -1;
	}

	// Right mouse button (for cheaters!)
	if (!bMouseRB && bRightMouse) {
		bMouseRB = true;
		savedNodes = m_nodes;
		for (unsigned i = 0; i < m_nodes.size(); i++)
			m_nodes[i] = m_combo[i];
	} else if(bMouseRB && !bRightMouse) {
		bMouseRB = false;
		m_nodes = savedNodes;
	}
	
	// Handle mouse dragging data
	if (bMouseLB && bLeftMouse && pt.x != ptLast.x)
	{
		auto nIndex = int(25.0f * fPosX);
		if (nSelected > -1 && nIndex != nSelected && nIndex > -1 && nIndex < 25)
		{
			// Check to see if another slider node exists at the newly desired index
			auto exists = false;
			for (auto c : m_nodes) {
				if (c == nIndex) {
					exists = true;
					break;
				}
			}
			
			// If no node exists, adjust the position of this node
			if (!exists) {
				auto n = -1;
				for (unsigned i = 0; i < m_nodes.size(); i++) {
					if (m_nodes[i] == nSelected) {
						n = i;
						break;
					}
				}
				if (n > -1)
				{
					// Disallow nodes to surpass eachother
					if (n == 0) {
						if (m_nodes[n + 1] > nIndex) {
							m_nodes[n] = nIndex;
							nSelected = nIndex;
						}
					}
					if (n > 0 && n < int(m_nodes.size() - 1)) {
						if (m_nodes[n - 1] < nIndex && m_nodes[n + 1] > nIndex) {
							m_nodes[n] = nIndex;
							nSelected = nIndex;
						}
					}
					if (n == m_nodes.size() - 1) {
						if (m_nodes[n - 1] < nIndex) {
							m_nodes[n] = nIndex;
							nSelected = nIndex;
						}
					}
				}
			}
		}

		// Update the last recorded mouse position
		ptLast = pt;
	}

	// Prepare node sprite positions for rendering
	for (unsigned i = 0; i < m_sprites.size() - 1; i++) {
		auto spr = m_sprites[i + 1];
		auto w = spr->GetWidth();
		auto x = (w * m_nodes[i]);
		spr->SetPosition(x, 4);
	}
}

// Just as it says, returns our sprite collection
std::vector<CSprite*> CGameLogic::GetSprites() {
	return m_sprites;
}

// This function calculates and returns a new combination
// for the fire marble dome slider puzzle, which consists
// of 5 consecutive sliders between the range of 1 and 25
std::vector<int> CGameLogic::CreateCombination()
{
	// We will use the C++11 standard to utilize a PRNG here
	const int nMax = 25;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	
	// Generate a vector of random numbers between 0 and 24
	std::vector<int> combo;
	while (combo.size() < 5)
	{
		auto num = static_cast<int>(dist(mt) * nMax);
		bool exists = false;
		for (auto n : combo) {
			if (n == num) {
				exists = true;
				break;
			}
		}
		if (!exists)
			combo.push_back(num);
	}

	// Sort the vector
	std::sort(combo.begin(), combo.end());
	return combo;
}

// I'm pretty sure you can guess what this is meant to do
VOID CGameLogic::CheckCombo()
{
	// Check if we have the right combo
	auto bMatch = true;
	for (unsigned i = 0; i < m_combo.size(); i++) {
		if (m_nodes[i] != m_combo[i]) {
			bMatch = false;
			break;
		}
	}

	// If we do, tell the user, and reset to a new combo.
	// Otherwise, set the nodes to the first 5 notches
	if (bMatch) {
		MessageBox(NULL, L"You have the correct combination!", L"Hooray!", MB_OK | MB_ICONINFORMATION);
		m_combo = CreateCombination();
		for (unsigned i = 0; i < m_nodes.size(); i++)
			m_nodes[i] = i;
	}
	else {
		MessageBox(NULL, L"Nope! Try again...", L"Sorry!", MB_OK | MB_ICONEXCLAMATION);
		// Having trouble guessing? Read the standard output...
		std::ostringstream ss;
		ss << "Combo: ";
		for (unsigned i = 0; i < m_nodes.size(); i++) {
			m_nodes[i] = i;
			ss << m_combo[i] << " ";
		}
		ss << std::endl;
		OutputDebugStringA(ss.str().c_str());
	}
}