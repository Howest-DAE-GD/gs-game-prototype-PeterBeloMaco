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

	//Player
	m_PlayerTransform.center.x = GetViewPort().width / 2.f;
	m_PlayerTransform.center.y = GetViewPort().height / 2.f;
	m_SizePlayer = 10.f;
	m_Pos.x = m_PlayerTransform.center.x;
	m_Pos.y = m_PlayerTransform.center.y;
	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;

	//Points
	m_Score = 0;
	m_HighScore = 0;
	m_Timer = 5.f;
	float randomX{ static_cast<float>(rand() % 601 + 100) };
	float randomY{ static_cast<float>(rand() % 601 + 100) };
	m_PointsTransform.center.x = randomX;
	m_PointsTransform.center.y = randomY;
	m_SizePoints = 20.f;
	m_pSoundEffects[0] = new SoundEffect{ "old-fashioned-clock-sound-37729.mp3" };
	m_pSoundEffects[1] = new SoundEffect{ "old-mechanic-alarm-clock-140410.mp3" };
	m_pSoundEffects[0]->Play(false);
}

void Game::Cleanup( )
{
	delete m_pSoundEffects[0];
	delete m_pSoundEffects[1];
}

void Game::Update( float elapsedSec )
{
	float TimerChange{};

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	std::cout << m_PlayerTransform.center.x << ", " << m_PlayerTransform.center.y << std::endl;
	std::cout << "								" << m_PointsTransform.center.x << ", " << m_PointsTransform.center.y << std::endl;

	Vector2f acceleration{ 0.f, 0.f };

	float maxVelocity = 1000.f;

	bool hasInput = false;

	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		acceleration.x = 1000.f;
		hasInput = true;
	}
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		acceleration.x = -1000.f;
		hasInput = true;
	}
	if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
	{
		acceleration.y = 1000.f;
		hasInput = true;
	}
	if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		acceleration.y = -1000.f;
		hasInput = true;
	}

	if (!hasInput && m_Velocity.Length() > 0)
	{
		m_Velocity += -20.f * m_Velocity * elapsedSec;
	}
	
	if (m_Velocity.Length() > maxVelocity)
	{
		m_Velocity = m_Velocity.Normalized() * maxVelocity;
	}

	m_Velocity += acceleration * elapsedSec;

	m_Pos += m_Velocity * elapsedSec;

	m_PlayerTransform.center.x = m_Pos.x;
	m_PlayerTransform.center.y = m_Pos.y;

	m_Timer -= elapsedSec;

	if (m_Timer <= 0.f)
	{
		m_Score = 0;
		float randomX{ static_cast<float>(rand() % 601 + 100) };
		float randomY{ static_cast<float>(rand() % 601 + 100) };
		m_PointsTransform.center.x = randomX;
		m_PointsTransform.center.y = randomY;

		m_pSoundEffects[0]->StopAll;
		m_pSoundEffects[1]->Play(false);
	}

	if (std::abs(m_PlayerTransform.center.x - m_PointsTransform.center.x) <= (m_SizePlayer + m_SizePoints) / 1.f &&
		std::abs(m_PlayerTransform.center.y - m_PointsTransform.center.y) <= (m_SizePlayer + m_SizePoints) / 1.f)
	{
		m_Score++;
		m_pSoundEffects[1]->StopAll;
		if (m_pSoundEffects[0]->IsLoaded())
		{
			m_pSoundEffects[0]->StopAll;
		}

		m_pSoundEffects[0]->Play(false);

		if (m_Score >= 0 && m_Score < 20)
		{
			TimerChange = 5.f;
		}
		if (m_Score >= 20 && m_Score < 50)
		{
			TimerChange = 3.f;
		}
		if (m_Score >= 50 && m_Score < 100)
		{
			TimerChange = 2.f;
		}
		if (m_Score >= 100 && m_Score < 150)
		{
			TimerChange = 1.f;
		}
		if (m_Score >= 150)
		{
			TimerChange = 0.5f;
		}

		m_Timer = TimerChange;
		float randomX{ static_cast<float>(rand() % 601 + 100) };
		float randomY{ static_cast<float>(rand() % 601 + 100) };
		m_PointsTransform.center.x = randomX;
		m_PointsTransform.center.y = randomY;
	}

	if (m_Score > m_HighScore)
	{
		m_HighScore = m_Score;
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
}

void Game::Draw( ) const
{
	ClearBackground();
	DrawTxt();
	DrawPoints();
	DrawPlayer();
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

void Game::DrawPoints() const
{
	utils::SetColor(Color4f(1, 0, 0, 1));
	Circlef circle{ m_PointsTransform.center.x, m_PointsTransform.center.y, m_SizePoints };
	utils::FillEllipse(circle.center.x, circle.center.y, circle.radius, circle.radius);
}

void Game::DrawTxt() const
{
	std::ostringstream oss{};
	oss << std::setprecision(2) << std::fixed << std::setw(5) << m_Timer;
	std::string time = oss.str();
	Point2f PosHighScore{ 0.f, 750.f };
	Point2f PosTimer{ 460.f, 750.f };
	Texture Score{ "Score: " + std::to_string(m_Score), "VCR_OSD_MONO_1.001.ttf", 50, Color4f{ 1, 1, 1, 1 } };
	Texture HighScore{ "High Score: " + std::to_string(m_HighScore), "VCR_OSD_MONO_1.001.ttf", 50, Color4f{ 1, 1, 1, 1 } };
	Texture Timer{ "Timer:" + time, "VCR_OSD_MONO_1.001.ttf", 50, Color4f{ 1, 1, 1, 1 } };
	Score.Draw();
	HighScore.Draw(PosHighScore);
	Timer.Draw(PosTimer);
}