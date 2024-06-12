#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h> 
#include <chrono>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_PlayerTransform.Position.x = 0.f;
	m_PlayerTransform.Position.y = -300.f;
	m_SizePlayer = 5.f;

	m_Velocity = 100.f;

	m_LevelTransform.Position.x = 0.f;
	m_LevelTransform.Position.y = 50.f;

	m_Level1Completed = false;
	m_Level2Completed = false;
	m_Level3Completed = false;

	m_pMusic1 = new SoundStream{ "The Caretaker - Its just a burning memory (2016).mp3" };
	m_pMusic2 = new SoundStream{ "Daisy Bell.mp3" };
	m_pMusic3 = new SoundStream{ "Daisy Bell (Reversed).mp3" };
	m_pLaugh = new SoundEffect{ "laugh.wav" };
	m_pJumpScare = new SoundEffect{ "jumpscare_sound-39630.mp3" };

	m_pMusic1->Play(true);
	m_pMusic1->SetVolume(10);

	m_pMinijumpscare = new Texture{ "maxresdefault-87308970.jpg" };
	m_pMinijumpscare2 = new Texture{ "maxresdefault-3133430762.jpg" };
	m_pEscaped = new Texture{"escaped-3557651552.png"};
	m_pChangeMode = new Texture{"ChangeMode.png"};

	m_Minijumpscare = false;
	m_Minijumpscare2 = false;
	m_Escaped = false;

	
}

void Game::Cleanup( )
{
	delete m_pMusic1;
	delete m_pMusic2;
	delete m_pMusic3;
	delete m_pLaugh;
	delete m_pJumpScare;

	delete m_pMinijumpscare;
	delete m_pMinijumpscare2;
	delete m_pEscaped;
}

void Game::Update( float elapsedSec )
{
	SetGameMode();

	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	if ( pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_PlayerTransform.Position.x += m_Velocity * elapsedSec;
	}
	if ( pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_PlayerTransform.Position.x -= m_Velocity * elapsedSec;
	}
	if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
	{
		m_PlayerTransform.Position.y += m_Velocity * elapsedSec;
	}
	if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		m_PlayerTransform.Position.y -= m_Velocity * elapsedSec;
	}

	if (!m_Level1Completed)
	{
		CheckCollisionLevel1();
	}
	else if (m_Level1Completed)
	{
		CheckCollisionLevel2();
	}

	if (m_Minijumpscare)
	{
		static float minijumpscareTimer = 0.0f; // Define a static timer variable
		minijumpscareTimer += elapsedSec; // Increment the timer with elapsed time

		if (minijumpscareTimer >= 1.0f) // Check if 5 seconds have passed
		{
			m_Minijumpscare = false; // Set m_Minijumpscare to false
			minijumpscareTimer = 0.0f; // Reset the timer for future use
		}

	}

	if (m_Minijumpscare2)
	{
		static float minijumpscareTimer = 0.0f; // Define a static timer variable
		minijumpscareTimer += elapsedSec; // Increment the timer with elapsed time

		if (minijumpscareTimer >= 1.0f) // Check if 5 seconds have passed
		{
			m_Minijumpscare2 = false; // Set m_Minijumpscare to false
			minijumpscareTimer = 0.0f; // Reset the timer for future use
		}

	}

	if (m_Escaped)
	{
	    static float escapedTimer = 0.0f; // Define a static timer variable
		escapedTimer += elapsedSec; // Increment the timer with elapsed time

		if (escapedTimer >= 2.0f) // Check if 5 seconds have passed
		{

			SDL_Quit();
			escapedTimer = 0.0f; // Reset the timer for future use
		}
	}

}

void Game::Draw( ) const
{
	ClearBackground();
	
	if (!m_Level1Completed)
	{
		DrawLevel1();
		m_pChangeMode->Draw();
	}
	else if (m_Level1Completed)
	{
		DrawLevel2();
	}
	DrawPlayer();

	if (m_Minijumpscare)
	{
		m_pMinijumpscare->Draw();
	}
	else if (m_Minijumpscare2)
	{
		m_pMinijumpscare2->Draw();
	}
	else if (m_Escaped)
	{
		m_pEscaped->Draw();
	}
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
	utils::SetColor(Color4f(0, 0, 1, 1));
	m_PlayerTransform.ApplyTransformation();
	utils::FillEllipse(GetViewPort().width / 2, GetViewPort().height / 2, m_SizePlayer, m_SizePlayer);
	m_PlayerTransform.ResetTransformation();
}

void Game::DrawLevel1() const
{
	utils::SetColor(Color4f(1, 0.88f, 0.22f, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(490.f, 0.f, 300.f, 700.f);
	m_LevelTransform.ResetTransformation();

	utils::SetColor(Color4f(1, 0.88f, 0.22f, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(490.f, 650.f, 600.f, 50.f);
	m_LevelTransform.ResetTransformation();

	//End of level 1

	utils::SetColor(Color4f(1, 0, 0, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 650.f, 60.f, 50.f);
	m_LevelTransform.ResetTransformation();
}

void Game::DrawLevel2() const
{
	utils::SetColor(Color4f(1, 0.88f, 0.22f, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 650.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	utils::SetColor(Color4f(1, 0.88f, 0.22f, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 50.f, 50.f, 600.f);
	m_LevelTransform.ResetTransformation();

	utils::SetColor(Color4f(1, 0.88f, 0.22f, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 450.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	utils::SetColor(Color4f(1, 0.88f, 0.22f, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 250.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	utils::SetColor(Color4f(1, 0.88f, 0.22f, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 50.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	//Ends of level 2

	utils::SetColor(Color4f(1, 0, 0, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 450.f, 60.f, 50.f);
	m_LevelTransform.ResetTransformation();

	utils::SetColor(Color4f(1, 0, 0, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 250.f, 60.f, 50.f);
	m_LevelTransform.ResetTransformation();

	utils::SetColor(Color4f(1, 0, 0, 1));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 50.f, 60.f, 50.f);
	m_LevelTransform.ResetTransformation();
}

void Game::CheckCollisionLevel1()
{
	float playerX = m_PlayerTransform.Position.x;
	float playerY = m_PlayerTransform.Position.y;

	// The boundaries of the first rectangle
    float rect1X = -155.f;
    float rect1Y = -350.f;
    float rect1Width = 300.f;
    float rect1Height = 700.f;

    // The boundaries of the second rectangle
    float rect2X = -155.f;
    float rect2Y = 300.f;
    float rect2Width = 600.f;
    float rect2Height = 50.f;

	// Check if the player is at the end of the level
	float rect3X = 380.f;
	float rect3Y = 300.f;
	float rect3Width = 60.f;
	float rect3Height = 50.f;

	bool playerAtEndOfLevel1 = playerX >= rect3X && playerX <= rect3X + rect3Width && playerY >= rect3Y && playerY <= rect3Y + rect3Height;

	if (playerAtEndOfLevel1)
	{
		// Player is at the end of the level 1
		m_Level1Completed = true;
		std::cout << "Level 1 completed\n";
		m_pMusic2->Play(true);
		m_pMusic2->SetVolume(10);

	}
	else if (!m_Level1Completed)
	{
		
		if (playerX >= rect1X && playerX <= rect1X + rect1Width && playerY >= rect1Y && playerY <= rect1Y + rect1Height || 
			playerX >= rect2X && playerX <= rect2X + rect2Width && playerY >= rect2Y && playerY <= rect2Y + rect2Height)
		{
			// Player is inside the level, do nothing
			std::cout << "In Level 1\n";
		}
		else
		{
			m_PlayerTransform.Position.x = 0.f;
			m_PlayerTransform.Position.y = -300.f;
			std::cout << "Out Level 1\n";
		}
	}
}

void Game::CheckCollisionLevel2()
{

	float playerX = m_PlayerTransform.Position.x;
	float playerY = m_PlayerTransform.Position.y;

	// The boundaries of the first rectangle
	float rect1X = -495.f;
	float rect1Y = 300.f;
	float rect1Width = 940.f;
	float rect1Height = 50.f;

	// The boundaries of the second rectangle
	float rect2X = -495.f;
	float rect2Y = -300.f;
	float rect2Width = 50.f;
	float rect2Height = 600.f;

	// The boundaries of the third rectangle
	float rect3X = -495.f;
	float rect3Y = 100.f;
	float rect3Width = 940.f;
	float rect3Height = 50.f;

	// The boundaries of the forth rectangle
	float rect4X = -495.f;
	float rect4Y = -100.f;
	float rect4Width = 940.f;
	float rect4Height = 50.f;

	// The boundaries of the fifth rectangle
	float rect5X = -495.f;
	float rect5Y = -300.f;
	float rect5Width = 940.f;
	float rect5Height = 50.f;

	// Check if the player is at the end of the level
	float rectEnd1X = 380.f;
	float rectEnd1Y = 100.f;
	float rectEnd1Width = 60.f;
	float rectEnd1Height = 50.f;

	float rectEnd2X = 380.f;
	float rectEnd2Y = -100.f;
	float rectEnd2Width = 60.f;
	float rectEnd2Height = 50.f;

	float rectEnd3X = 380.f;
	float rectEnd3Y = -300.f;
	float rectEnd3Width = 60.f;
	float rectEnd3Height = 50.f;

	bool playerAtEnd1OfLevel2 = playerX >= rectEnd1X && playerX <= rectEnd1X + rectEnd1Width && playerY >= rectEnd1Y && playerY <= rectEnd1Y + rectEnd1Height;
	bool playerAtEnd2OfLevel2 = playerX >= rectEnd2X && playerX <= rectEnd2X + rectEnd2Width && playerY >= rectEnd2Y && playerY <= rectEnd2Y + rectEnd2Height;
	bool playerAtEnd3OfLevel2 = playerX >= rectEnd3X && playerX <= rectEnd3X + rectEnd3Width && playerY >= rectEnd3Y && playerY <= rectEnd3Y + rectEnd3Height;

	if (playerAtEnd2OfLevel2)
	{
		m_Level2Completed = true;

		m_Escaped = true;

		std::cout << "Level 2 completed\n";
	}
	else if (playerAtEnd1OfLevel2)
	{
		m_Level2Completed = false;
		m_Level1Completed = false;
		m_PlayerTransform.Position.x = 0.f;
		m_PlayerTransform.Position.y = -300.f;
		CheckCollisionLevel1();
		m_pMusic1->Play(true);

		m_pLaugh->Play(false);
		m_pLaugh->Play(false);
		m_pLaugh->Play(false);
		m_pLaugh->Play(false);
		m_pLaugh->Play(false);

		
		std::cout << "Jumpscare" << std::endl;

		m_Minijumpscare = true;

	}
	else if (playerAtEnd3OfLevel2)
	{
		m_Level2Completed = false;
		m_Level1Completed = false;
		m_PlayerTransform.Position.x = 0.f;
		m_PlayerTransform.Position.y = -300.f;
		CheckCollisionLevel1();
		m_pMusic3->Play(true);

		m_pJumpScare->Play(false);
		m_pJumpScare->Play(false);
		m_pJumpScare->Play(false);
		m_pJumpScare->Play(false);
		m_pJumpScare->Play(false);

		std::cout << "Jumpscare 2" << std::endl;

		m_Minijumpscare2 = true;
	}
	else if (!m_Level2Completed)
	{
		if (playerX >= rect1X && playerX <= rect1X + rect1Width && playerY >= rect1Y && playerY <= rect1Y + rect1Height || 
			playerX >= rect2X && playerX <= rect2X + rect2Width && playerY >= rect2Y && playerY <= rect2Y + rect2Height || 
			playerX >= rect3X && playerX <= rect3X + rect3Width && playerY >= rect3Y && playerY <= rect3Y + rect3Height ||
			playerX >= rect4X && playerX <= rect4X + rect4Width && playerY >= rect4Y && playerY <= rect4Y + rect4Height ||
			playerX >= rect5X && playerX <= rect5X + rect5Width && playerY >= rect5Y && playerY <= rect5Y + rect5Height)
		{
			// Player is inside the level, do nothing
			//std::cout << "In Level 2\n";
		}
		else
		{
			m_Level2Completed = false;
			m_Level1Completed = false;
			m_PlayerTransform.Position.x = 0.f;
			m_PlayerTransform.Position.y = -300.f;
			CheckCollisionLevel1();
			m_pMusic1->Play(true);
			//std::cout << "Out Level 2\n";
		}
	}


}

void Game::SetGameMode()
{
	static int lastKeyPressTime = 0;
	int currentTime = SDL_GetTicks();

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_TAB] and not m_IsGameModeChanged and currentTime - lastKeyPressTime > 200)
	{
		lastKeyPressTime = currentTime;
		m_Velocity = 100.f;
		m_IsGameModeChanged = true;
	}
	else if (pStates[SDL_SCANCODE_TAB] and m_IsGameModeChanged and currentTime - lastKeyPressTime > 200)
	{
		lastKeyPressTime = currentTime;
		m_Velocity = 600.f;
		m_IsGameModeChanged = false;
	}


}





