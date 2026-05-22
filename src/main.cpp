/********************/
// main.cpp			//
// 10/05/2026		//
/********************/

/// TODO:
/// 
/// - Implement curved snake sprites for direction changes. Make more if necessary /// DONE
/// - Implement Apples, i.e. the level-up system /// DONE
/// - Fix snack spawning in snake on init /// DONE
/// - Get animation system working (if possible, implement inside gameLoop()) /// DONE
/// - Test: Does animation unload properly? /// DONE
/// - Implement game over screen /// DONE
/// - Fix the Snack spawn function (Game::moveSnack()). Maybe keep Tiles marked as empty in a separate vector? /// DONE
/// 

/// 15/05/2026 - The game is in a playable state
/// 19/05/2026 - Implemented basic main menu
/// 19/05/2026 - Implemented basic pause functionality & game restart
/// 21/05/2026 - Implemented 'Game Over' menu / rework
/// 21/05/2026 - Implemented score
/// 22/05/2026 - Configured project with CMake and created GitHub repo


#include "Game.hpp"

int main()
{
	// Create a 12*9 tile playing field (60p^2 per tile)
	constexpr Vector2 win_size{ 720.f, 540.f };

	try
	{
		Game game{ win_size, "Lunar Snake" };
		
		while (!WindowShouldClose())
		{
			switch (game.getGameState())
			{
			case Game_state::MAIN_MENU:
				game.menuLoop();
				break;
			
			case Game_state::PLAY:
				game.gameLoop();
				break;
			
			case Game_state::PAUSE:
				game.pauseLoop();
				break;

			case Game_state::GAME_OVER:
				game.gameOverLoop();
				break;
			}
		}
	}
	catch (Exit_game_exception err)
	{
		std::stringstream ss;
		ss << "Exited game! " << err.what();

		snakeLog(ss);

		CloseWindow();
		return 0;
	}
	catch (std::exception err)
	{
		std::cerr << "SNAKE: RUNTIME ERROR: " << err.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "SNAKE: UNKNOWN ERROR" << std::endl;
		return 2;
	}

	CloseWindow();
	return 0;
}
