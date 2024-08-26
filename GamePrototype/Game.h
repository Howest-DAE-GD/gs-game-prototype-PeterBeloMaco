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

	bool m_IsGameStarted;

	//Player
	void DrawPlayer() const;
	Circlef m_PlayerTransform{};
	float m_SizePlayer;
	Vector2f m_Velocity;
	Vector2f m_Pos;

	//Enemy
	void DrawEnemy() const;
	std::vector<Circlef> m_EnemyTransform;
	float m_SizeEnemy;
	bool m_EnemyActive;
	float m_EnemyVelocity;
	int m_numOfEnemies;

	//Points
	void DrawPoints() const;
	Circlef m_PointsTransform{};
	float m_SizePoints;
	int m_Score;
	int m_HighScore;
	Color4f m_HighScoreColor;
	float m_Timer;

	//PowerUp
	void DrawPowerUp() const;
	Circlef m_PowerUpTransform{};
	float m_SizePowerUp;
	float m_PowerUpFreezeTimer;
	float m_PowerUpSpawnTimer;
	bool m_PowerUpActive;
	Texture* m_pPowerUpTexture;
	
	//Sounds
	SoundEffect* m_pSoundEffects[5];
	SoundStream* m_pMusic[1];
	
	//Text
	void DrawTxt() const;

	bool m_Alarm;
	float m_AlarmTimer;
};
	

