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

	m_pMusic[0] = new SoundStream{"The Caretaker - Its just a burning memory (2016).mp3"};
	m_pMusic[1] = new SoundStream{"Daisy Bell.mp3"};
	m_pMusic[2] = new SoundStream{"Daisy Bell (Reversed).mp3"};
	m_pMusic[3] = new SoundStream{"Scary.wav"};

	m_pLaughSound = new SoundEffect{ "laugh.wav" };
	m_pJumpScareSound = new SoundEffect{ "jumpscare_sound-39630.mp3" };
	m_pGlitchSound = new SoundEffect{ "Glitch sound.wav" };
	m_pBlueScreenSound[0] = new SoundEffect{"shortest-blue-screen-of-death-sound-effect.mp3"};
	m_pBlueScreenSound[1] = new SoundEffect{"blue-screen.mp3"};
	m_pNoSignalSound = new SoundEffect{"NoSignal.mp3"};

	m_pMusic[0]->Play(true);
	m_pMusic[0]->SetVolume(10);

	m_pCam = new Texture{"Camera.png"};
	m_pMinijumpscare[0] = new Texture{ "maxresdefault-87308970.jpg" };
	m_pMinijumpscare[1] = new Texture{ "maxresdefault-3133430762.jpg" };
	m_pBlueScreen = new Texture{"Blue screen.png"};
	m_pNoSignal = new Texture{"No signal.jpg"};
	m_pEscaped = new Texture{"escaped-3557651552.png"};
	m_pChangeMode = new Texture{"ChangeMode.png"};
	m_pGlitch[0] = new Texture{"Glitch level1.png"};
	m_pGlitch[1] = new Texture{"Glitch level2.png"};
	m_pGlitch[2] = new Texture{"Glitch level3.png"};

	m_Minijumpscare = false;
	m_Minijumpscare2 = false;
	m_Escaped = false;
	m_NewLevel = false;
	m_BlueScreen = false;
	m_NoSignal = false;

	m_Level3EndGlitch = 1.f;
	
}

void Game::Cleanup( )
{
	delete m_pMusic[0];
	delete m_pMusic[1];
	delete m_pMusic[2];
	delete m_pMusic[3];
	delete m_pLaughSound;
	delete m_pJumpScareSound;

	delete m_pCam;
	delete m_pMinijumpscare[0];
	delete m_pMinijumpscare[1];
	delete m_pEscaped;
	delete m_pBlueScreen;
	delete m_pChangeMode;
	delete m_pGlitch[0];
	delete m_pGlitch[1];
	delete m_pGlitch[2];
}

void Game::Update( float elapsedSec )
{
	SetGameMode();

	std::cout << "					Player position: " << m_PlayerTransform.Position.x << ", " << m_PlayerTransform.Position.y << std::endl;
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
	else if (m_Level1Completed and !m_Level2Completed)
	{
		CheckCollisionLevel2();
	}
	else if (m_Level2Completed)
	{
		CheckCollisionLevel3();
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

		if (escapedTimer >= 3.0f) // Check if 5 seconds have passed
		{

			SDL_Quit();
			escapedTimer = 0.0f; // Reset the timer for future use
		}
	}

	if (m_BlueScreen)
	{
		static float escapedTimer = 0.0f;
		escapedTimer += elapsedSec;

		if (escapedTimer >= 5.0f)
		{
			SDL_Quit();
			escapedTimer = 0.0f;
		}
	}

	if (m_NoSignal)
	{
		static float escapedTimer = 0.0f;
		escapedTimer += elapsedSec;
		m_Velocity = 0.f;

		if (escapedTimer >= 4.0f)
		{
			m_Velocity = 100.f;
			m_NoSignal = false;
			escapedTimer = 0.0f;
			m_pMusic[0]->Play(true);
			m_pMusic[0]->SetVolume(10);
			m_pNoSignalSound->StopAll();
		}
	}

	if (m_NewLevel)
	{
		static float escapedTimer = 0.0f;
		escapedTimer += elapsedSec;

		if (escapedTimer >= 0.5f)
		{
			m_NewLevel = false;
			escapedTimer = 0.0f;
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
	else if (m_Level1Completed and !m_Level2Completed)
	{
		DrawLevel2();
	}
	else if (m_Level2Completed)
	{
		DrawLevel3();
	}

	if (m_NewLevel and !m_Level1Completed)
	{
		m_pGlitch[0]->Draw();
	}
	else if (m_NewLevel and m_Level1Completed and !m_Level2Completed)
	{
		m_pGlitch[1]->Draw();
	}
	else if (m_NewLevel and m_Level2Completed and !m_Level3Completed)
	{
		m_pGlitch[2]->Draw();
	}
	m_pCam->Draw();

	DrawPlayer();

	if (m_Minijumpscare)
	{
		m_pMinijumpscare[0]->Draw();
	}
	else if (m_Minijumpscare2)
	{
		m_pMinijumpscare[1]->Draw();
	}
	else if (m_Escaped)
	{
		m_pEscaped->Draw();
	}
	else if (m_BlueScreen)
	{
		m_pBlueScreen->Draw();
	}
	else if (m_NoSignal)
	{
		m_pNoSignal->Draw();
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

void Game::DrawEnemy() const
{

}

void Game::DrawLevel1() const
{
	utils::SetColor(Color4f(1.f, 0.88f, 0.22f, 1.f));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(490.f, 0.f, 300.f, 700.f);
	m_LevelTransform.ResetTransformation();
;
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(490.f, 650.f, 600.f, 50.f);
	m_LevelTransform.ResetTransformation();

	//End of level 1

	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 650.f, 60.f, 50.f);
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
		m_pMusic[1]->Play(true);
		m_pMusic[1]->SetVolume(10);

		m_NewLevel = true;
		m_pGlitchSound->Play(false);

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
			m_NewLevel = true;
			m_pGlitchSound->Play(false);
			std::cout << "Out Level 1\n";
		}
	}
}

void Game::DrawLevel2() const
{
	utils::SetColor(Color4f(1.f, 0.88f, 0.22f, 1.f));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 650.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 50.f, 50.f, 600.f);
	m_LevelTransform.ResetTransformation();

	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 450.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 250.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	m_LevelTransform.ApplyTransformation();
	utils::FillRect(150.f, 50.f, 940.f, 50.f);
	m_LevelTransform.ResetTransformation();

	//Ends of level 2

	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));
	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 450.f, 60.f, 50.f);
	m_LevelTransform.ResetTransformation();

	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 250.f, 60.f, 50.f);
	m_LevelTransform.ResetTransformation();

	m_LevelTransform.ApplyTransformation();
	utils::FillRect(1030.f, 50.f, 60.f, 50.f);
	m_LevelTransform.ResetTransformation();
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
		m_NewLevel = true;
		m_pGlitchSound->Play(false);

		m_pMusic[3]->Play(true);
		m_pMusic[3]->SetVolume(100);
		std::cout << "Level 2 completed\n";
	}
	else if (playerAtEnd1OfLevel2)
	{
		m_Level2Completed = false;
		m_Level1Completed = false;
		m_PlayerTransform.Position.x = 0.f;
		m_PlayerTransform.Position.y = -300.f;
		CheckCollisionLevel1();
		m_pMusic[0]->Play(true);
		m_pMusic[0]->SetVolume(10);

		m_pLaughSound->Play(false);
		m_pLaughSound->Play(false);
		m_pLaughSound->Play(false);
		m_pLaughSound->Play(false);
		m_pLaughSound->Play(false);

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
		m_pMusic[2]->Play(true);

		m_pJumpScareSound->Play(false);
		m_pJumpScareSound->Play(false);
		m_pJumpScareSound->Play(false);
		m_pJumpScareSound->Play(false);
		m_pJumpScareSound->Play(false);

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
			std::cout << "In Level 2\n";
		}
		else
		{
			m_Level2Completed = false;
			m_Level1Completed = false;
			m_PlayerTransform.Position.x = 0.f;
			m_PlayerTransform.Position.y = -300.f;
			CheckCollisionLevel1();
			m_pMusic[0]->Play(true);
			m_pMusic[0]->SetVolume(10);
			std::cout << "Out Level 2\n";

			m_NewLevel = true;
			m_pGlitchSound->Play(false);
		}
	}
}

void Game::DrawLevel3() const
{
	utils::SetColor(Color4f(1.f, 0.88f, 0.22f, 1.f));

	utils::FillRect(890.f, 285.f, 200.f, 80.f); //1

	utils::FillRect(500.f, 300.f, 500.f, 50.f); //2

	utils::FillRect(500.f, 300.f, 50.f, 200.f); //3

	utils::FillRect(500.f, 500.f, 500.f, 50.f); //4

	utils::FillRect(700.f, 500.f, 10.f, 200.f); //5 

	utils::FillRect(300.f, 700.f, 500.f, 50.f); //6

	utils::FillRect(300.f, 500.f, 10.f, 200.f); //7

	utils::FillRect(500.f, 650.f, 10.f, 50.f); //8

	utils::FillRect(100.f, 550.f, 200.f, 50.f); //9

	utils::FillRect(100.f, 500.f, 200.f, 10.f); //10

	utils::FillRect(100.f, 300.f, 50.f, 200.f); //11

	//Ends of level 3
	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));
	utils::FillRect(750.f, 700.f, 50.f, 50.f);
	utils::FillRect(970.f, 500.f, 50.f, 50.f);

	utils::SetColor(Color4f(1.f, 0.f, 0.f, m_Level3EndGlitch));
	utils::FillRect(100.f, 550.f, 50.f, 50.f);

	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.0f));
	utils::FillRect(100.f, 300.f, 50.f, 50.f);
}

void Game::CheckCollisionLevel3()
{
	float playerX = m_PlayerTransform.Position.x;
	float playerY = m_PlayerTransform.Position.y;

	// The boundaries of the first rectangle
	float rect1X = 250.f;
	float rect1Y = -110.f;
	float rect1Width = 200.f;
	float rect1Height = 80.f;

	// The boundaries of the second rectangle
	float rect2X = -145.f;
	float rect2Y = -100.f;
	float rect2Width = 500.f;
	float rect2Height = 50.f;

	// The boundaries of the third rectangle
	float rect3X = -145.f;
	float rect3Y = -100.f;
	float rect3Width = 50.f;
	float rect3Height = 200.f;

	// The boundaries of the fourth rectangle
	float rect4X = -145.f;
	float rect4Y = 100.f;
	float rect4Width = 500.f;
	float rect4Height = 50.f;

	// The boundaries of the fifth rectangle
	float rect5X = 55.f;
	float rect5Y = 100.f;
	float rect5Width = 10.f;
	float rect5Height = 200.f;

	// The boundaries of the sixth rectangle
	float rect6X = -345.f;
	float rect6Y = 300.f;
	float rect6Width = 500.f;
	float rect6Height = 50.f;

	// The boundaries of the seventh rectangle
	float rect7X = -345.f;
	float rect7Y = 100.f;
	float rect7Width = 10.f;
	float rect7Height = 200.f;

	// The boundaries of the eighth rectangle
	float rect8X = -145.f;
	float rect8Y = 250.f;
	float rect8Width = 10.f;
	float rect8Height = 50.f;

	// The boundaries of the ninth rectangle
	float rect9X = -545.f;
	float rect9Y = 150.f;
	float rect9Width = 200.f;
	float rect9Height = 50.f;

	// The boundaries of the tenth rectangle
	float rect10X = -545.f;
	float rect10Y = 100.f;
	float rect10Width = 200.f;
	float rect10Height = 10.f;

	// The boundaries of the eleventh rectangle
	float rect11X = -545.f;
	float rect11Y = -100.f;
	float rect11Width = 50.f;
	float rect11Height = 200.f;

	// Check if the player is at the end of the level
	float rectEnd1X = 325.f;
	float rectEnd1Y = 100.f;
	float rectEnd1Width = 50.f;
	float rectEnd1Height = 50.f;

	float rectEnd2X = 105.f;
	float rectEnd2Y = 300.f;
	float rectEnd2Width = 50.f;
	float rectEnd2Height = 50.f;

	float rectEnd3X = -545.f;
	float rectEnd3Y = 150.f;
	float rectEnd3Width = 50.f;
	float rectEnd3Height = 50.f;

	float rectEnd4X = -545.f;
	float rectEnd4Y = -100.f;
	float rectEnd4Width = 50.f;
	float rectEnd4Height = 50.f;


	bool playerAtEnd1OfLevel3 = playerX >= rectEnd1X && playerX <= rectEnd1X + rectEnd1Width && playerY >= rectEnd1Y && playerY <= rectEnd1Y + rectEnd1Height;
	bool playerAtEnd2OfLevel3 = playerX >= rectEnd2X && playerX <= rectEnd2X + rectEnd2Width && playerY >= rectEnd2Y && playerY <= rectEnd2Y + rectEnd2Height;
	bool playerAtEnd3OfLevel3 = playerX >= rectEnd3X && playerX <= rectEnd3X + rectEnd3Width && playerY >= rectEnd3Y && playerY <= rectEnd3Y + rectEnd3Height;
	bool playerAtEnd4OfLevel3 = playerX >= rectEnd4X && playerX <= rectEnd4X + rectEnd4Width && playerY >= rectEnd4Y && playerY <= rectEnd4Y + rectEnd4Height;

	if (playerAtEnd4OfLevel3)
	{
		// Player is at the end of level 3
		m_Level3Completed = true;
		std::cout << "Level 3 completed\n";
		m_Escaped = true;
		m_Velocity = 0.f;
	}
	else if (playerAtEnd3OfLevel3 and m_Level3EndGlitch == 1.f)
	{
		m_NewLevel = true;
		m_pGlitchSound->Play(false);
		m_Level3EndGlitch = 0.f;
		std::cout << "Level 3 failed\n";
	}
	else if(playerAtEnd2OfLevel3)
	{
		m_pMusic[3]->Stop();
		m_NoSignal = true;
		m_pNoSignalSound->SetVolume(20);
		m_pNoSignalSound->Play(false);

		m_Level2Completed = false;
		m_Level1Completed = false;
		m_PlayerTransform.Position.x = 0.f;
		m_PlayerTransform.Position.y = -300.f;
		CheckCollisionLevel1();
		
		std::cout << "Level 3 failed\n";
	}
	else if (playerAtEnd1OfLevel3)
	{
		m_Velocity = 0.f;
		m_pMusic[3]->Stop();
		m_BlueScreen = true;
		m_pBlueScreenSound[0]->Play(false);
		m_pBlueScreenSound[1]->Play(false);
		std::cout << "Level 3 failed\n";
	}
	else if (!m_Level3Completed)
	{
		if ((playerX >= rect1X && playerX <= rect1X + rect1Width && playerY >= rect1Y && playerY <= rect1Y + rect1Height) ||
			(playerX >= rect2X && playerX <= rect2X + rect2Width && playerY >= rect2Y && playerY <= rect2Y + rect2Height) || 
			playerX >= rect3X && playerX <= rect3X + rect3Width && playerY >= rect3Y && playerY <= rect3Y + rect3Height || 
			playerX >= rect4X && playerX <= rect4X + rect4Width && playerY >= rect4Y && playerY <= rect4Y + rect4Height || 
			playerX >= rect5X && playerX <= rect5X + rect5Width && playerY >= rect5Y && playerY <= rect5Y + rect5Height ||
			playerX >= rect6X && playerX <= rect6X + rect6Width && playerY >= rect6Y && playerY <= rect6Y + rect6Height || 
			playerX >= rect7X && playerX <= rect7X + rect7Width && playerY >= rect7Y && playerY <= rect7Y + rect7Height || 
			playerX >= rect8X && playerX <= rect8X + rect8Width && playerY >= rect8Y && playerY <= rect8Y + rect8Height || 
			playerX >= rect9X && playerX <= rect9X + rect9Width && playerY >= rect9Y && playerY <= rect9Y + rect9Height || 
			playerX >= rect10X && playerX <= rect10X + rect10Width && playerY >= rect10Y && playerY <= rect10Y + rect10Height || 
			playerX >= rect11X && playerX <= rect11X + rect11Width && playerY >= rect11Y && playerY <= rect11Y + rect11Height)
		{
			// Player is inside the level, do nothing
			std::cout << "In Level 3\n";
		}
		else
		{
			m_PlayerTransform.Position.x = 400.f;
			m_PlayerTransform.Position.y = -75.f;
			std::cout << "Out Level 3\n";

			m_NewLevel = true;
			m_pGlitchSound->Play(false);
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





