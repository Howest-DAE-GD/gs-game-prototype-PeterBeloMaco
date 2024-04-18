#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>

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
	m_PlayerTransform.Position.x = m_Radius;
	m_PlayerTransform.Position.y = m_Radius;

	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	if ( pStates[SDL_SCANCODE_RIGHT] )
	{
		m_PlayerTransform.Position.x += 200.f * elapsedSec;
		std::cout << "Right arrow key is down\n";
	}
	if ( pStates[SDL_SCANCODE_LEFT])
	{
		m_PlayerTransform.Position.x -= 200.f * elapsedSec;
		std::cout << "Left and up arrow keys are down\n";
	}
	if (pStates[SDL_SCANCODE_UP])
	{
		m_PlayerTransform.Position.y += 200.f * elapsedSec;
		std::cout << "Left and up arrow keys are down\n";
	}
	if (pStates[SDL_SCANCODE_DOWN])
	{
		m_PlayerTransform.Position.y -= 200.f * elapsedSec;
		std::cout << "Left and up arrow keys are down\n";
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	utils::SetColor(Color4f(1, 0, 0, 1));
	m_PlayerTransform.ApplyTransformation();
	utils::FillEllipse(GetViewPort().width / 2, GetViewPort().height / 2, m_Radius, m_Radius);
	m_PlayerTransform.ResetTransformation();

	utils::SetColor(Color4f(0, 0, 0, 1));
	m_MazeTransform.ApplyTransformation();
	utils::DrawRect(50, 50, 700, 400);
	m_MazeTransform.ResetTransformation();

	utils::SetColor(Color4f(0, 0, 1, 1));
	m_MazeTransform.ApplyTransformation();
	utils::DrawLine(50, 200, 100, 200);
	utils::DrawLine(100, 250, 100, 400);
	utils::DrawLine(100, 400, 150, 400);
	utils::DrawLine(100, 250, 300, 250);
	utils::DrawLine(200, 250, 200, 400);
	utils::DrawLine(150, 100, 150, 250);
	utils::DrawLine(300, 100, 300, 250);
	m_MazeTransform.ResetTransformation();


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

void Game::ClearBackground( ) const
{
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
