#pragma once
#include "BaseGame.h"
#include "Transform.h"
#include <stack>
#include <vector>
#include "SoundEffect.h"
#include "SoundStream.h"
#include <Texture.h>
#include <utils.h>


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
	void Draw() const override;

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

	//Player
	void DrawPlayer() const;
	Circlef m_PlayerTransform{};
	float m_SizePlayer;
	Vector2f m_Velocity;
	Vector2f m_Pos;
	
	//Points
	void DrawPoints() const;
	Circlef m_PointsTransform{};
	float m_SizePoints;
	int m_Score;
	int m_HighScore;
	float m_Timer;
	SoundEffect* m_pSoundEffects[2];
	
	//Text
	void DrawTxt() const;
};
	

