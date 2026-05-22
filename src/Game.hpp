/********************/
// Game.hpp			//
// 10/05/2026		//
/********************/

#ifndef GAME_HPP
#define GAME_HPP

#include "Snake.hpp"
#include "Tile.hpp"
#include "Snack.hpp"
#include "Animation.hpp"

#include <raylib.h>

#include <iostream>
#include <vector>
#include <string>
#include <chrono>


using Stopwatch = std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point>;

/*----------------------------------------*\
| Enum                                     |
\*----------------------------------------*/
enum class Game_state
{
	MAIN_MENU,
	PLAY,
	PAUSE,
	GAME_OVER
};


/*----------------------------------------*\
| Classes                                  |
\*----------------------------------------*/
class Game
{
public:

	explicit Game(const Vector2 &win_size, const std::string &win_name);
	~Game();

	void menuLoop();
	void gameLoop();
	void pauseLoop();
	void gameOverLoop();

	void exit(const std::string &arg_str = "");

	const Game_state &getGameState() const;


private:

	void init(const Vector2 &win_size, const std::string &win_name);

	void initFloorTiles();
	void drawFloorTiles();

	void resetGame();

	void setGameState(const Game_state &arg_game_state);

	bool shouldSnakeMove() const;

	void moveSnake();
	void moveSnack();

	void drawSnake();
	void drawBodySegment(const Texture2D &arg_texture, const Tile &arg_tile, const Direction &arg_dir) const;
	void drawSnack();
	void drawScore() const;

	void drawAnimationFrame(Animation &arg_anim);

	auto getAdjacentTile(const Vector2 &arg_pos, const Direction &arg_dir) -> const Tile &;

	const Tile &getTileFromMatrix(const Vector2 &arg_pos) const;
	void setTileStateInMatrix(const Vector2 &arg_pos, const Tile_state &arg_state);

	void handleOtherInput(const int &arg_key);

	int m_score;

	Stopwatch m_move_timer;
	std::chrono::milliseconds m_game_speed;

	Snake m_snake;

	Snack m_snack;

	std::vector<Tile> m_tile_matrix;

	Game_state m_game_state;
	
	Image m_window_icon_01;

	Font m_score_font;

	Texture2D m_title_screen_01;
	Texture2D m_pause_menu_01;
	Texture2D m_game_over_01;
	Texture2D m_floor_tile_01;
	Texture2D m_snack_01;
	Texture2D m_snake_head_01;
	Texture2D m_snake_body_01;
	Texture2D m_snake_body_02;
	Texture2D m_snake_body_03;
	Texture2D m_snake_tail_01;

	Texture2D m_snack_eaten_sprite_sheet;
	Texture2D m_game_over_sprite_sheet;

	Animation m_snack_eaten_anim;
	Animation m_game_over_anim;
};


/*
std::vector<Tile> m_tile_matrix[107]
	{
					x: 0 - 11
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  y: 0 - 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
*/


class Exit_game_exception : public std::exception
{
public:

	Exit_game_exception(const std::string &msg);

	// Override what() method, marked noexcept for modern C++
	const char *what() const noexcept override;


private:

	std::string message;
};



#endif // !GAME_HPP
