/********************/
// Game.cpp			//
// 10/05/2026		//
/********************/

#include "Game.hpp"


				////////////
				//  Game  //
				////////////
/*----------------------------------------*\
| Constructor / Destructor                 |
\*----------------------------------------*/
Game::Game(const Vector2 &win_size, const std::string &win_name)
	:m_snake(),
	m_snack(getRandomPos(11, 8)),
	m_game_speed(300),
	m_snack_eaten_anim(),
	m_game_state(Game_state::MAIN_MENU)
{
	init(win_size, win_name);
	snakeLog("'Game' class initialized");
}

Game::~Game()
{
	UnloadImage(m_window_icon_01);

	UnloadFont(m_score_font);

	UnloadTexture(m_title_screen_01);
	UnloadTexture(m_pause_menu_01);
	UnloadTexture(m_game_over_01);
	UnloadTexture(m_floor_tile_01);
	UnloadTexture(m_snack_01);
	UnloadTexture(m_snake_head_01);
	UnloadTexture(m_snake_body_01);
	UnloadTexture(m_snake_body_02);
	UnloadTexture(m_snake_body_03);
	UnloadTexture(m_snake_tail_01);

	UnloadTexture(m_snack_eaten_sprite_sheet);
	UnloadTexture(m_game_over_sprite_sheet);

	snakeLog("'Game' class destructor done");
}



/*----------------------------------------*\
| Member Functions                         |
\*----------------------------------------*/
/// Public
void Game::menuLoop()
{
	// Define collison rectangles for the menu buttons
	static const Rectangle play_button_rect{ 242, 298, 240, 50 };
	static const Rectangle exit_button_rect{ 251, 395, 220, 50 };

	BeginDrawing();

	DrawTexture(m_title_screen_01, 0, 0, WHITE);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mouse_pos = GetMousePosition();

		// Was input inside play button?
		if (CheckCollisionPointRec(mouse_pos, play_button_rect))
			setGameState(Game_state::PLAY);

		// Was input inside exit button?
		else if (CheckCollisionPointRec(mouse_pos, exit_button_rect))
			exit("Exited from the menu.");
	}

	EndDrawing();
}

void Game::gameLoop()
{
	BeginDrawing();

	drawFloorTiles();
	drawSnack();
	
	if (!m_snack_eaten_anim.isAnimationComplete())
	{
		drawAnimationFrame(m_snack_eaten_anim);
		m_snack_eaten_anim.updateAnimationFrame();
	}

	// Returns 0 if nothing pressed, otherwise check which key
	if (int key_code = GetKeyPressed())
	{
		m_snake.handleDirectionInput(key_code);
		handleOtherInput(key_code);
	}
	

	m_move_timer.second = std::chrono::high_resolution_clock::now();
	if (shouldSnakeMove())
	{
		moveSnake();
		m_move_timer.first = std::chrono::high_resolution_clock::now();
	}

	drawSnake();
	drawScore();

	EndDrawing();
}

void Game::pauseLoop()
{
	// Define collison rectangles for the menu buttons
	static const Rectangle resume_button_rect{ 256, 256, 220, 50 };
	static const Rectangle menu_button_rect{ 243, 325, 100, 45 };
	static const Rectangle exit_button_rect{ 368, 325, 120, 45 };

	BeginDrawing();
	
	drawFloorTiles();
	drawSnack();
	drawSnake();
	drawScore();

	// Draw transparent RAYWHITE while paused
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 245, 245, 245, 63 });

	DrawTexture(m_pause_menu_01, 0, 0, WHITE);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mouse_pos = GetMousePosition();

		// Was input inside resume button?
		if (CheckCollisionPointRec(mouse_pos, resume_button_rect))
		{
			m_move_timer.first = std::chrono::high_resolution_clock::now();
			setGameState(Game_state::PLAY);

			snakeLog("Game Unpaused");
		}

		// Was input inside menu button?
		else if (CheckCollisionPointRec(mouse_pos, menu_button_rect))
		{
			resetGame();
			setGameState(Game_state::MAIN_MENU);
		}

		// Was input inside exit button?
		else if (CheckCollisionPointRec(mouse_pos, exit_button_rect))
			exit("Exited from the pause menu.");
	}

	// Returns 0 if nothing pressed, otherwise check which key
	if (uint16_t key_code = GetKeyPressed())
		handleOtherInput(key_code);
	
	
	EndDrawing();
}

void Game::gameOverLoop()
{
	// Define collison rectangles for the menu buttons
	static const Rectangle retry_button_rect{ 257, 270, 210, 50 };
	static const Rectangle menu_button_rect{ 236, 338, 105, 50 };
	static const Rectangle exit_button_rect{ 357, 342, 130, 50 };

	BeginDrawing();

	if (!m_game_over_anim.isAnimationComplete())
	{
		drawFloorTiles();
		drawSnack();
		drawSnake();
		drawScore();

		drawAnimationFrame(m_game_over_anim);
		m_game_over_anim.updateAnimationFrame();
	}
	else
		DrawTexture(m_game_over_01, 0, 0, WHITE);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mouse_pos = GetMousePosition();

		// Was input inside retry button?
		if (CheckCollisionPointRec(mouse_pos, retry_button_rect))
		{
			m_snack_eaten_anim.resetAnimation();
			m_game_over_anim.resetAnimation();
			resetGame();
			m_move_timer.first = std::chrono::high_resolution_clock::now();

			setGameState(Game_state::PLAY);

			snakeLog("Game restarted");
		}

		// Was input inside menu button?
		else if (CheckCollisionPointRec(mouse_pos, menu_button_rect))
		{
			resetGame();
			setGameState(Game_state::MAIN_MENU);
		}

		// Was input inside exit button?
		else if (CheckCollisionPointRec(mouse_pos, exit_button_rect))
			exit("Exited from the game over menu.");
	}
	
	EndDrawing();
}

void Game::exit(const std::string &arg_str)
{
	throw Exit_game_exception{ arg_str };
}

const Game_state &Game::getGameState() const
{
	return m_game_state;
}

/// Private
void Game::init(const Vector2 &win_size, const std::string &win_name)
{
	InitWindow((int)win_size.x, (int)win_size.y, win_name.c_str());
	SetWindowMonitor(0);
	SetTargetFPS(120);
	
	// Overwrite default exit key 'ESC'
	SetExitKey(KEY_ZERO);

	m_window_icon_01 = LoadImage(ASSET_PATH "window/window_icon_01.png");
	SetWindowIcon(m_window_icon_01);

	m_score_font = LoadFont(ASSET_PATH "font/pixantiqua.png");

	m_title_screen_01 = LoadTexture(ASSET_PATH "menu/title_screen_01.png");
	m_pause_menu_01 = LoadTexture(ASSET_PATH "menu/pause_menu_01.png");
	m_game_over_01 = LoadTexture(ASSET_PATH "menu/game_over_01.png");
	m_floor_tile_01 = LoadTexture(ASSET_PATH "level/floor_tile_02.png");
	m_snack_01 = LoadTexture(ASSET_PATH "level/snack_02.png");
	m_snake_head_01 = LoadTexture(ASSET_PATH "snake/snake_head_02.png");
	m_snake_body_01 = LoadTexture(ASSET_PATH "snake/snake_body_04.png");
	m_snake_body_02 = LoadTexture(ASSET_PATH "snake/snake_body_05.png");
	m_snake_body_03 = LoadTexture(ASSET_PATH "snake/snake_body_06.png");
	m_snake_tail_01 = LoadTexture(ASSET_PATH "snake/snake_tail_02.png");

	m_snack_eaten_sprite_sheet = LoadTexture(ASSET_PATH "level/animation/snack_02_anim.png");
	m_game_over_sprite_sheet = LoadTexture(ASSET_PATH "menu/animation/game_over_anim_01.png");

	m_snack_eaten_anim.init(m_snack_eaten_sprite_sheet, 6);
	m_game_over_anim.init(m_game_over_sprite_sheet, 5);
	
	initFloorTiles();

	setTileStateInMatrix(m_snake.getHead().getPos(), Tile_state::HEAD);
	setTileStateInMatrix(m_snake.getBodyVec().at(0).getPos(), Tile_state::BODY);

	// Make sure snack doesn't spawn on snake (this isn't inefficient because most of the map will be empty at this point)
	while (getTileFromMatrix(m_snack.getPos()).getState() != Tile_state::EMPTY)
		m_snack.setPos(getRandomPos(11, 8));

	setTileStateInMatrix(m_snack.getPos(), Tile_state::SNACK);

	m_score = 0;

	m_move_timer.first = std::chrono::high_resolution_clock::now();
}

/// Enter tiles into 'tile_matrix' with an x-value from 0-11 and a y-value from 0-8
void Game::initFloorTiles()
{
	if (!m_tile_matrix.empty())
		m_tile_matrix.clear();

	for (uint8_t i = 0; i < 9; ++i)
		for (uint8_t j = 0; j < 12; ++j)
			m_tile_matrix.push_back(Tile{ Vector2{ (float)j, (float)i } });

	std::stringstream ss;
	ss << "Floor tiles initialized (" << m_tile_matrix.size() << ')';

	snakeLog(ss);
}

void Game::drawFloorTiles()
{
	for (auto tile : m_tile_matrix)
		DrawTextureV(m_floor_tile_01, Vector2{ tile.getPosXTL(), tile.getPosYTL() }, WHITE);
}

void Game::resetGame()
{
	initFloorTiles();

	m_score = 0;

	m_snake.Snake::~Snake();
	m_snake.Snake::Snake();

	setTileStateInMatrix(m_snake.getHead().getPos(), Tile_state::HEAD);
	setTileStateInMatrix(m_snake.getBodyVec().at(0).getPos(), Tile_state::BODY);

	// Make sure snack doesn't spawn on snake
	m_snack.Snack::~Snack();
	m_snack.Snack::Snack(getRandomPos(11, 8));
	while (getTileFromMatrix(m_snack.getPos()).getState() != Tile_state::EMPTY)
		m_snack.setPos(getRandomPos(11, 8));

	setTileStateInMatrix(m_snack.getPos(), Tile_state::SNACK);

	std::stringstream ss;
	ss << "Snack constructed at (" << m_snack.getPos().x << ", " << m_snack.getPos().y << ')';

	snakeLog(ss);
}

void Game::setGameState(const Game_state &arg_game_state)
{
	m_game_state = arg_game_state;
}

/// Return true if >= 'm_game_speed' time has passed
bool Game::shouldSnakeMove() const
{
	auto delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(m_move_timer.second - m_move_timer.first);
	
	return (delta_t >= m_game_speed);
}

/// Returns a 'const Tile &' adjacent to the argument position
auto Game::getAdjacentTile(const Vector2 &arg_pos, const Direction &arg_dir) -> const Tile &
{
	Vector2 new_pos = directionToPos(arg_pos, arg_dir);

	// Check for error value
	if (new_pos.x == -1.f)
	{
		setGameState(Game_state::GAME_OVER);

		// Return false tile (Game will end, so this doesn't matter (it's still stupid))
		return getTileFromMatrix(Vector2{ 0.f, 0.f });
	}

	return getTileFromMatrix(new_pos);
}

void Game::moveSnake()
{
	m_snake.applyDirectionBuffer();

	// Move head to this tile next (will change Game_state if snake ran into a wall)
	const Tile &next_tile = getAdjacentTile(m_snake.getHead().getPos(), m_snake.getDir());

	// Check if the snake will eat a snack
	bool is_snack_eaten = false;
	if (next_tile.getState() == Tile_state::SNACK)
	{
		is_snack_eaten = true;
		++m_score;

		snakeLog("Snack eaten");
	}

	// Remember the head's old position and direction, which will be the new values for the next segment and so on
	Vector2 new_pos = m_snake.getHead().getPos();
	Direction new_dir = m_snake.getHead().getDir();

	const std::vector<Snake::Body> &local_body_vec = m_snake.getBodyVec();

	// Move the head
	m_snake.setHeadPos(directionToPos(new_pos, m_snake.getDir()));
	setTileStateInMatrix(new_pos, Tile_state::HEAD);

	// Move every body segment to the old position of the previous segment, do the same for direction
	for (uint8_t i = 0; i < local_body_vec.size(); ++i)
	{
		Vector2 temp_pos = local_body_vec.at(i).getPos();
		Direction temp_dir = local_body_vec.at(i).getDir();

		m_snake.setBodySegmentPos(new_pos, i);
		m_snake.setBodySegmentDir(new_dir, i);

		setTileStateInMatrix(new_pos, Tile_state::BODY);
		

		// Change tiles behind snake back to 'EMPTY'
		if (i == local_body_vec.size() - 1)
		{
			if (is_snack_eaten)
			{
				m_snake.attachNewBody(temp_pos, temp_dir);
				setTileStateInMatrix(temp_pos, Tile_state::BODY);

				m_snack_eaten_anim.setAnimationPos(m_snack.getPos());
				m_snack_eaten_anim.startAnimation();

				moveSnack();
			}
			else
				setTileStateInMatrix(temp_pos, Tile_state::EMPTY);
			
			setTileStateInMatrix(m_snack.getPos(), Tile_state::SNACK); // doing this last avoids overwriting snack position to empty

			break;
		}
		
		new_pos = temp_pos;
		new_dir = temp_dir;
	}

	// Check if snake ran into a wall / Check whether Game_state was changed from before (delay needed to make body move one last time)
	if (getGameState() == Game_state::GAME_OVER)
	{
		// Set head pos to error position
		m_snake.setHeadPos(Vector2{ -1.f, -1.f });

		m_game_over_anim.setAnimationPos(Vector2{ 0.f, 0.f });
		m_game_over_anim.startAnimation();

		return;
	}

	// Check if snake ran into itself
	if (next_tile.getState() == Tile_state::BODY)
	{
		m_game_over_anim.setAnimationPos(Vector2{ 0.f, 0.f});
		m_game_over_anim.startAnimation();

		setGameState(Game_state::GAME_OVER);
	}

	std::stringstream ss;
	ss << "Head moved to (" << m_snake.getHead().getPos().x << ", " << m_snake.getHead().getPos().y << ')';

	snakeLog(ss);
}

void Game::moveSnack()
{
	std::vector<std::unique_ptr<Tile>> empty_tile_vec;

	for (const Tile &tile : m_tile_matrix)
		if (tile.getState() == Tile_state::EMPTY)
			empty_tile_vec.push_back(std::make_unique<Tile>(tile));

	static std::random_device r;

	static std::default_random_engine generator{ r() };
	std::uniform_int_distribution<int> distribution(0, static_cast<int>(empty_tile_vec.size()) - 1);

	m_snack.setPos(empty_tile_vec.at(distribution(generator))->getVec2());

	std::stringstream ss;
	ss << "Snack moved to (" << m_snack.getPos().x << ", " << m_snack.getPos().y << ')';

	snakeLog(ss);
}

void Game::drawSnake()
{
	const std::vector<Snake::Body> &local_body_vec = m_snake.getBodyVec();

	// Take direction of the current and next segment to choose sprite
	Direction cur_dir = m_snake.getHead().getDir();
	Direction next_dir = cur_dir; // placeholder initializer

	bool is_last_segment = false;

	// Draw each body segment
	for (uint8_t i = 0; i < local_body_vec.size(); ++i)
	{
		Vector2 cur_pos = local_body_vec.at(i).getPos();

		if (i != local_body_vec.size() - 1)
			next_dir = local_body_vec.at(i + 1).getDir();
		else
			is_last_segment = true;

		// If turning counter-clockwise
		if (cur_dir == next_dir + 3)	// equal to -1
			drawBodySegment(m_snake_body_03, getTileFromMatrix(cur_pos), next_dir);
			
		// If turning clockwise
		else if (cur_dir == next_dir + 1)
			drawBodySegment(m_snake_body_02, getTileFromMatrix(cur_pos), next_dir);
			
		else
			drawBodySegment(is_last_segment ? m_snake_tail_01 : m_snake_body_01, getTileFromMatrix(cur_pos), cur_dir);

		cur_dir = next_dir;
	}

	// Draw Head last so that it's on the top layer (important for game over visuals)
	// Also don't try drawing it if it's out-of-bounds
	if (m_snake.getHead().getPos().x != -1.f)
		drawBodySegment(m_snake_head_01, getTileFromMatrix(m_snake.getHead().getPos()), m_snake.getDir());
}

void Game::drawBodySegment(const Texture2D &arg_texture, const Tile &arg_tile, const Direction &arg_dir) const
{
	float local_pos_x = arg_tile.getPosXTL();
	float local_pos_y = arg_tile.getPosYTL();
	float rotation = 0.f;

	switch (arg_dir)
	{
	case Direction::UP:
	{
		local_pos_y += 60.f;
		rotation = 270.f;
		break;
	}

	case Direction::RIGHT:
		break;

	case Direction::DOWN:
	{
		local_pos_x += 60.f;
		rotation = 90.f;
		break;
	}

	case Direction::LEFT:
	{
		local_pos_x += 60.f;
		local_pos_y += 60.f;
		rotation = 180.f;
		break;
	}

	default:
		throw std::runtime_error("drawBodySegment(): INVALID PARAMETER");
	}

	DrawTextureEx(arg_texture, Vector2{ local_pos_x, local_pos_y }, rotation, 1.f, WHITE);
}

void Game::drawSnack()
{
	const Tile &snack_tile = getTileFromMatrix(m_snack.getPos());
	DrawTextureV(m_snack_01, Vector2{ snack_tile.getPosXTL(), snack_tile.getPosYTL() }, WHITE);
}

void Game::drawScore() const
{
	Vector2 draw_pos
	{ 
		m_score < 10 ? static_cast<float>(GetScreenWidth() / 2 - 10) : static_cast<float>(GetScreenWidth() / 2 - 18),
		25.f 
	};

	DrawTextEx(m_score_font, 
			   std::to_string(m_score).c_str(), 
			   draw_pos,
			   static_cast<float>(m_score_font.baseSize * 4),
			   2.f, 
			   Color{ 159, 138, 191, 255 });
}

void Game::drawAnimationFrame(Animation &arg_anim)
{
	const Tile &local_tile = getTileFromMatrix(arg_anim.getAnimationPos());

	DrawTexturePro(
		arg_anim.getSpriteSheet(), 
		arg_anim.getAnimationFrame(), 
		Rectangle{ local_tile.getPosXTL(), local_tile.getPosYTL(), static_cast<float>(arg_anim.getSpriteWidth()), static_cast<float>(arg_anim.getSpriteHeight()) },
		Vector2{ 0.f, 0.f }, 
		0.f, 
		WHITE);
}

/// Returns 'const Tile &' from 'Vector2' coordinates
const Tile &Game::getTileFromMatrix(const Vector2 &arg_pos) const
{
	const uint8_t index = uint8_t(arg_pos.x + arg_pos.y * 12);

	return m_tile_matrix.at(index);
}

/// Find the tile at 'arg_pos' and change it to 'arg_state'
void Game::setTileStateInMatrix(const Vector2 &arg_pos, const Tile_state &arg_state)
{
	const size_t index = size_t(arg_pos.x + arg_pos.y * 12);

	m_tile_matrix.at(index).setState(arg_state);
}

void Game::handleOtherInput(const int &arg_key)
{
	switch (arg_key)
	{
	// Game speed-
	case KEY_PERIOD:
	{
		m_game_speed += static_cast<std::chrono::milliseconds>(300);
		
		std::stringstream ss;
		ss << "Game speed +300ms: new value: " << m_game_speed;
		snakeLog(ss);
		
		break;
	}

	// Game speed+
	case KEY_COMMA:
	{
		m_game_speed -= static_cast<std::chrono::milliseconds>(300);
		
		std::stringstream ss;
		ss << "Game speed -300ms: new value: " << m_game_speed;
		snakeLog(ss);
		
		break;
	}

	// Pause / Unpause
	case KEY_ESCAPE:
	{
		if (getGameState() != Game_state::PAUSE)
		{
			setGameState(Game_state::PAUSE);
			m_snack_eaten_anim.resetAnimation();

			snakeLog("Game Paused");
		}
		
		else
		{
			m_move_timer.first = std::chrono::high_resolution_clock::now();
			setGameState(Game_state::PLAY);
			
			snakeLog("Game Unpaused");
		}

		break;
	}
	case KEY_R:
	{
		m_snack_eaten_anim.resetAnimation();
		resetGame();
		m_move_timer.first = std::chrono::high_resolution_clock::now();

		snakeLog("Game restarted");

		break;
	}
	}
}


				///////////////////////////
				//  Game_over_exception  //
				///////////////////////////
/*----------------------------------------*\
| Constructor / Destructor                 |
\*----------------------------------------*/
Exit_game_exception::Exit_game_exception(const std::string &msg)
	: message(msg)
{}

const char *Exit_game_exception::what() const noexcept
{
	return message.c_str();
}
