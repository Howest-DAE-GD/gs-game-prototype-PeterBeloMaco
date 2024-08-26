#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <random>
#include <Texture.h>
#include <iomanip> 
#include <sstream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	m_IsGameStarted = false;

	//Player
	m_PlayerTransform.center.x = GetViewPort().width / 2.f;
	m_PlayerTransform.center.y = GetViewPort().height / 2.f;
	m_SizePlayer = 10.f;
	m_Pos.x = m_PlayerTransform.center.x;
	m_Pos.y = m_PlayerTransform.center.y;
	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;

	//Enemy
	m_numOfEnemies = 0;
	m_EnemyTransform.push_back(Circlef{});
	m_EnemyTransform[0].center.x = -100;
	m_EnemyTransform[0].center.y = -100;
	m_SizeEnemy = 10.f;
	m_EnemyActive = false;
	m_EnemyVelocity = 0.9f;

	//Points
	m_Score = 0;
	m_HighScore = 0;
	m_Timer = 5.f;
	float PointsrandomX{ static_cast<float>(rand() % 601 + 100) };
	float PointsrandomY{ static_cast<float>(rand() % 601 + 100) };
	m_PointsTransform.center.x = PointsrandomX;
	m_PointsTransform.center.y = PointsrandomY;
	m_SizePoints = 20.f;
	m_HighScoreColor = Color4f{ 1, 1, 1, 1 };

	//PowerUp
	m_PowerUpSpawnTimer = 0.f;
	m_PowerUpFreezeTimer = 0.f;
	m_SizePowerUp = 20.f;
	m_PowerUpActive = false;
	m_pPowerUpTexture = new Texture{ "8294672-1714046858.png" };
	m_PowerUpTransform.center.x = -100;
	m_PowerUpTransform.center.y = -100;

	//Sounds
	m_pSoundEffects[0] = new SoundEffect{ "old-fashioned-clock-sound-37729.mp3" };
	m_pSoundEffects[1] = new SoundEffect{ "old-mechanic-alarm-clock-140410.wav" };
	m_pSoundEffects[2] = new SoundEffect{ "Freeze time.wav" };
	m_pSoundEffects[3] = new SoundEffect{ "coin-upaif-14631.mp3" };
	m_pSoundEffects[4] = new SoundEffect{ "ouch-116112.mp3" };
	m_pMusic[0] = new SoundStream{ "BackgroundMusic.mp3" };
	m_pMusic[0]->Play(true);
	m_pMusic[0]->SetVolume(50);
	
	m_Alarm = false;
	m_AlarmTimer = 0.f;
	
}

void Game::Cleanup()
{
	for (int i = 0; i < 5; i++)
	{
		delete m_pSoundEffects[i];
	}
	delete m_pMusic[0];
	delete m_pPowerUpTexture;
}

void Game::Update(float elapsedSec)
{
	float TimerChange{};

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	Vector2f acceleration{ 0.f, 0.f };

	float maxVelocity = 400.f;
	const float drag = -2.f;

	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		acceleration.x = 1000.f;
	}
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		acceleration.x -= 1000.f;
	}
	if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
	{
		acceleration.y = 1000.f;
	}
	if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		acceleration.y -= 1000.f;
	}

	m_Velocity += drag * m_Velocity * elapsedSec;

	if (m_Velocity.Length() > maxVelocity)
	{
		m_Velocity = m_Velocity.Normalized() * maxVelocity;
	}

	m_Velocity += acceleration * elapsedSec;

	m_Pos += m_Velocity * elapsedSec;

	m_PlayerTransform.center.x = m_Pos.x;
	m_PlayerTransform.center.y = m_Pos.y;

	// Spawn the PowerUp randomly
	m_PowerUpSpawnTimer += elapsedSec;

	if (m_PowerUpSpawnTimer >= 10.f and m_IsGameStarted)
	{
		int chance = rand() % 100;
		if (chance < 60)
		{
			float PowerUprandomX{ static_cast<float>(rand() % 601 + 100) };
			float PowerUprandomY{ static_cast<float>(rand() % 601 + 100) };
			m_PowerUpTransform.center.x = PowerUprandomX;
			m_PowerUpTransform.center.y = PowerUprandomY;
		}

		m_PowerUpSpawnTimer = 0.f;
	}

	if (std::abs(m_PlayerTransform.center.x - m_PowerUpTransform.center.x) <= (m_SizePlayer + m_SizePowerUp) / 1.f &&
		std::abs(m_PlayerTransform.center.y - m_PowerUpTransform.center.y) <= (m_SizePlayer + m_SizePowerUp) / 1.f)
	{
		m_PowerUpActive = true;
		m_PowerUpTransform.center.x = -100;
		m_PowerUpTransform.center.y = -100;

		if (m_pSoundEffects[0]->IsLoaded())
		{
			m_pSoundEffects[0]->StopAll();
		}

		m_pSoundEffects[2]->Play(false);
		m_pSoundEffects[4]->ResumeAll;
	}

	if (m_PowerUpActive)
	{
		m_EnemyVelocity = 0.2f;
		m_PowerUpFreezeTimer += elapsedSec;

		if (m_PowerUpFreezeTimer >= 5.f)
		{
			m_PowerUpActive = false;
			m_PowerUpFreezeTimer = 0.f;;
			m_pSoundEffects[0]->Play(false);
			m_EnemyVelocity = 0.9f;
		}

	}
	else if (m_IsGameStarted and not m_PowerUpActive)
	{
		m_Timer -= elapsedSec;
	}

	if (m_Alarm)
	{
		m_AlarmTimer += elapsedSec;

		if (m_AlarmTimer >= 1.f)
		{
			m_pSoundEffects[1]->StopAll();
			m_Alarm = false;
			m_AlarmTimer = 0.f;
		}
	}

	if (m_Timer <= 0.f)
	{
		m_Score = 0;
		m_Timer = 5.f;
		m_IsGameStarted = false;
		m_Alarm = true;
		m_EnemyActive = false;
		m_numOfEnemies = 0;
		for (int i = 0; i < m_numOfEnemies; i++)
		{
			m_EnemyTransform[i].center.x = -100;
			m_EnemyTransform[i].center.y = -100;

		}
		float randomX{ static_cast<float>(rand() % 601 + 100) };
		float randomY{ static_cast<float>(rand() % 601 + 100) };
		m_PointsTransform.center.x = randomX;
		m_PointsTransform.center.y = randomY;

		if (m_pSoundEffects[0]->IsLoaded())
		{
			m_pSoundEffects[0]->StopAll();
		}
		m_pSoundEffects[1]->Play(false);
		m_HighScoreColor = Color4f{ 1, 1, 1, 1 };
	}

	if (std::abs(m_PlayerTransform.center.x - m_PointsTransform.center.x) <= (m_SizePlayer + m_SizePoints) / 1.f &&
		std::abs(m_PlayerTransform.center.y - m_PointsTransform.center.y) <= (m_SizePlayer + m_SizePoints) / 1.f)
	{
		m_Score++;
		m_IsGameStarted = true;
		if (not m_PowerUpActive)
		{
			m_pSoundEffects[0]->Play(false);
			if (m_pSoundEffects[0]->IsLoaded())
			{
				m_pSoundEffects[0]->StopAll();
				m_pSoundEffects[0]->Play(false);
			}
		}

		m_pSoundEffects[3]->Play(false);

		if (m_Score >= 0 && m_Score < 10)
		{
			m_EnemyTransform.clear();
			TimerChange = 5.f;
			m_numOfEnemies = 0;
		}
		if (m_Score >= 10 && m_Score < 20)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 5.f;
			m_numOfEnemies = 1;
		}
		if (m_Score >= 20 && m_Score < 30)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 4.f;
			m_numOfEnemies = 2;
		}
		if (m_Score >= 30 && m_Score < 40)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 4.f;
			m_numOfEnemies = 3;
		}
		if (m_Score >= 40 && m_Score < 50)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 3.5f;
			m_numOfEnemies = 4;
		}
		if (m_Score >= 50 && m_Score < 60)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 3.5f;
			m_numOfEnemies = 5;
		}
		if (m_Score >= 60 && m_Score < 70)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 3.5f;
			m_numOfEnemies = 6;
		}
		if (m_Score >= 70 && m_Score < 80)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 3.5f;
			m_numOfEnemies = 7;
		}
		if (m_Score >= 80 && m_Score < 90)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 3.5f;
			m_numOfEnemies = 8;
		}
		if (m_Score >= 90 && m_Score < 100)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 3.5f;
			m_numOfEnemies = 9;
		}
		if (m_Score >= 100)
		{
			m_EnemyTransform.push_back(Circlef{});
			TimerChange = 3.5f;
			m_numOfEnemies = 10;
		}

		m_Timer = TimerChange;

		float randomX{ static_cast<float>(rand() % 601 + 100) };
		float randomY{ static_cast<float>(rand() % 601 + 100) };
		m_PointsTransform.center.x = randomX;
		m_PointsTransform.center.y = randomY;

		for (int i = 0; i < m_numOfEnemies; i++)
		{
			float EnemyrandomX{ static_cast<float>(rand() % 601 + 100) };
			float EnemyrandomY{ static_cast<float>(rand() % 601 + 100) };
			m_EnemyTransform[i].center.x = EnemyrandomX;
			m_EnemyTransform[i].center.y = EnemyrandomY;
		}
		m_EnemyActive = true;
	}

	if (m_Score > m_HighScore)
	{
		m_HighScore = m_Score;
		m_HighScoreColor = Color4f{ 0, 1, 0, 1 };
	}

	// Check if the player is out of bounds
	if (m_Pos.x < 0) {
		m_Pos.x = GetViewPort().width - m_SizePlayer;
	}
	else if (m_Pos.x > GetViewPort().width) {
		m_Pos.x = 0 + m_SizePlayer;
	}

	if (m_Pos.y < 0) {
		m_Pos.y = GetViewPort().height - m_SizePlayer;
	}
	else if (m_Pos.y > GetViewPort().height) {
		m_Pos.y = 0 + m_SizePlayer;
	}
	
	for (int i = 0; i < m_numOfEnemies; i++)
	{
		if (m_EnemyActive)
		{
			if (m_PlayerTransform.center.x > m_EnemyTransform[i].center.x)
			{
				m_EnemyTransform[i].center.x += m_EnemyVelocity;
			}
			if (m_PlayerTransform.center.x < m_EnemyTransform[i].center.x)
			{
				m_EnemyTransform[i].center.x -= m_EnemyVelocity;
			}
			if (m_PlayerTransform.center.y > m_EnemyTransform[i].center.y)
			{
				m_EnemyTransform[i].center.y += m_EnemyVelocity;
			}
			if (m_PlayerTransform.center.y < m_EnemyTransform[i].center.y)
			{
				m_EnemyTransform[i].center.y -= m_EnemyVelocity;
			}

			if (std::abs(m_PlayerTransform.center.x - m_EnemyTransform[i].center.x) <= (m_SizePlayer + m_SizeEnemy) / 1.f &&
				std::abs(m_PlayerTransform.center.y - m_EnemyTransform[i].center.y) <= (m_SizePlayer + m_SizeEnemy) / 1.f)
			{
				m_pSoundEffects[4]->Play(false);
				m_EnemyTransform[i].center.x = -100;
				m_EnemyTransform[i].center.y = -100;

				m_Score -= 1;
				if (m_Score < 0)
				{
					m_Score = 0;
				}
				m_Timer -= 0.30f;
			}
		}
	}
}

void Game::Draw( ) const
{
	ClearBackground();
	DrawTxt();
	DrawPoints();
	DrawPlayer();
	DrawPowerUp();
	DrawEnemy();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground() const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::DrawPlayer() const
{
	utils::SetColor(Color4f(0, 0, 1, 1));;
	Circlef circle{ m_PlayerTransform.center.x, m_PlayerTransform.center.y, m_SizePlayer };
	utils::FillEllipse(circle.center.x, circle.center.y, circle.radius, circle.radius);
}

void Game::DrawEnemy() const
{
	for (int i = 0; i < m_numOfEnemies; i++)
	{
		utils::SetColor(Color4f(1, 0, 1, 1));
		Circlef circle{ m_EnemyTransform[i].center.x, m_EnemyTransform[i].center.y, m_SizeEnemy};
		utils::FillEllipse(circle.center.x, circle.center.y, circle.radius, circle.radius);
	}
}

void Game::DrawPoints() const
{
	utils::SetColor(Color4f(1, 0, 0, 1));
	Circlef circle{ m_PointsTransform.center.x, m_PointsTransform.center.y, m_SizePoints };
	utils::FillEllipse(circle.center.x, circle.center.y, circle.radius, circle.radius);
}

void Game::DrawPowerUp() const
{
	utils::SetColor(Color4f(0, 1, 0, 1));
	Circlef circle{ m_PowerUpTransform.center.x, m_PowerUpTransform.center.y, m_SizePowerUp };
	utils::FillEllipse(circle.center.x, circle.center.y, circle.radius, circle.radius);
	Point2f PosPowerUp{ m_PowerUpTransform.center.x - 29.f, m_PowerUpTransform.center.y - 29.f };

	m_pPowerUpTexture->Draw(PosPowerUp);
}

void Game::DrawTxt() const
{
	Color4f TimeColor { 1, 1, 1, 1 };

	if (m_PowerUpActive)
	{
		TimeColor = Color4f{ 0, 0, 1, 1 };
	}

	std::ostringstream oss{};
	oss << std::setprecision(2) << std::fixed << std::setw(5) << m_Timer;
	std::string time = oss.str();
	Point2f PosHighScore{ 0.f, 750.f };
	Point2f PosTimer{ 460.f, 750.f };
	Texture Score{ "Score: " + std::to_string(m_Score), "VCR_OSD_MONO_1.001.ttf", 50, Color4f{ 1, 1, 1, 1 } };
	Texture HighScore{ "High Score: " + std::to_string(m_HighScore), "VCR_OSD_MONO_1.001.ttf", 50, m_HighScoreColor };
	Texture Timer{ "Timer:" + time, "VCR_OSD_MONO_1.001.ttf", 50, TimeColor };
	Score.Draw();
	HighScore.Draw(PosHighScore);
	Timer.Draw(PosTimer);
}