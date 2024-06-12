#pragma once
#include "BaseGame.h"
#include "Transform.h"
#include <stack>
#include <vector>
#include "SoundEffect.h"
#include "SoundStream.h"
#include <Texture.h>
#include <SDL_image.h>

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void DrawLevel1() const;
	void DrawLevel2() const;
	void DrawPlayer() const;
	void CheckCollisionLevel1();
	void CheckCollisionLevel2();
	void SetGameMode();

	float m_SizePlayer;
	bool m_Level1Completed;
	bool m_Level2Completed;
	bool m_Level3Completed;
	bool m_Minijumpscare;
	bool m_Minijumpscare2;
	bool m_Escaped;
	float m_Velocity;
	bool m_IsGameModeChanged;

	Transform m_PlayerTransform{};
	Transform m_LevelTransform{};
	SoundStream* m_pMusic1;
	SoundStream* m_pMusic2;
	SoundStream* m_pMusic3;
	SoundEffect* m_pLaugh;
	SoundEffect* m_pJumpScare;
	Texture* m_pMinijumpscare;
	Texture* m_pMinijumpscare2;
	Texture* m_pEscaped;
	Texture* m_pChangeMode;
	
};