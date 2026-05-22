/********************/
// Snake.cpp		//
// 14/05/2026		//
/********************/

#include "Snake.hpp"

				/////////////////
				//  Direction  //
				/////////////////
/*----------------------------------------*\
| Operator Overloading                     |
\*----------------------------------------*/
/// Wrap around if new number exceeds enum definition range.
Direction operator+(const Direction &dir, const int &num)
{
	return static_cast<Direction>((static_cast<int>(dir) + num) % 4);
}

Direction operator++(const Direction &dir)
{
	return dir + 1;
}


				/////////////
				//  Snake  //
				/////////////
/*----------------------------------------*\
| Constructor / Destructor                 |
\*----------------------------------------*/
Snake::Snake()
	: m_dir(Direction::RIGHT),
	m_dir_buffer(Direction::RIGHT),
	m_head(getRandomPos(5, 8, 1), m_dir)
{
	m_body_vec.push_back(Body{ directionToPos(m_head.getPos(), m_dir + 2 ), m_dir }); // add 2 to get opposite direction

	snakeLog("'Snake' class initialized");
}


/*----------------------------------------*\
| Member Functions                         |
\*----------------------------------------*/
const Snake::Body &Snake::getHead() const
{
	return m_head;
}

const std::vector<Snake::Body> &Snake::getBodyVec() const
{
	return m_body_vec;
}

const Direction &Snake::getDir() const
{
	return m_dir;
}

void Snake::setHeadPos(const Vector2 &arg_pos)
{
	m_head.setPos(arg_pos);
}

void Snake::setBodySegmentPos(const Vector2 &arg_pos, const size_t &arg_index)
{
	m_body_vec.at(arg_index).setPos(arg_pos);
}

void Snake::setBodySegmentDir(const Direction &arg_dir, const size_t &arg_index)
{
	m_body_vec.at(arg_index).setDir(arg_dir);
}

void Snake::handleDirectionInput(const int &arg_key)
{
	switch (arg_key)
	{
	case KEY_W:
	case KEY_UP:
	{
		if (m_dir == Direction::DOWN)
		{
			snakeLog("Can't change direction from 'DOWN' to 'UP'");
			return;
		}
		if (m_dir == Direction::UP)
		{
			snakeLog("Direction is already 'UP'");
			return;
		}

		this->m_dir_buffer = Direction::UP;
		snakeLog("Direction buffer changed to 'UP'");
		break;
	}

	case KEY_A:
	case KEY_LEFT:
	{
		if (m_dir == Direction::RIGHT)
		{
			snakeLog("Can't change direction from 'RIGHT' to 'LEFT'");
			return;
		}
		if (m_dir == Direction::LEFT)
		{
			snakeLog("Direction is already 'LEFT'");
			return;
		}

		this->m_dir_buffer = Direction::LEFT;
		snakeLog("Direction buffer changed to 'LEFT'");
		break;
	}

	case KEY_S:
	case KEY_DOWN:
	{
		if (m_dir == Direction::UP)
		{
			snakeLog("Can't change direction from 'UP' to 'DOWN'");
			return;
		}
		if (m_dir == Direction::DOWN)
		{
			snakeLog("Direction is already 'DOWN'");
			return;
		}

		this->m_dir_buffer = Direction::DOWN;
		snakeLog("Direction buffer changed to 'DOWN'");
		break;
	}

	case KEY_D:
	case KEY_RIGHT:
	{
		if (m_dir == Direction::LEFT)
		{
			snakeLog("Can't change direction from 'LEFT' to 'RIGHT'");
			return;
		}
		if (m_dir == Direction::RIGHT)
		{
			snakeLog("Direction is already 'RIGHT'");
			return;
		}

		this->m_dir_buffer = Direction::RIGHT;
		snakeLog("Direction buffer changed to 'RIGHT'");
		break;
	}
	}
}

void Snake::applyDirectionBuffer()
{
	m_head.setDir(m_dir_buffer);
	m_dir = m_dir_buffer;
}

void Snake::attachNewBody(const Vector2 &arg_pos, const Direction &arg_dir)
{
	m_body_vec.push_back(Body{ arg_pos, arg_dir });
}



				////////////
				//  Body  //
				////////////
/*----------------------------------------*\
| Constructor / Destructor                 |
\*----------------------------------------*/
Snake::Body::Body(const Vector2 &arg_pos, const Direction &arg_dir)
	: m_pos(arg_pos),
	m_dir(arg_dir)
{
	std::stringstream ss;
	ss << "Body constructed at (" << m_pos.x << ", " << m_pos.y << ')';

	snakeLog(ss);
}

Snake::Body::~Body()
{}


/*----------------------------------------*\
| Member Functions                         |
\*----------------------------------------*/
const Vector2 &Snake::Body::getPos() const
{
	return m_pos;
}

const Direction &Snake::Body::getDir() const
{
	return m_dir;
}

void Snake::Body::setPos(const Vector2 &arg_pos)
{
	m_pos = arg_pos;
}

void Snake::Body::setDir(const Direction &arg_dir)
{
	m_dir = arg_dir;
}


/*----------------------------------------*\
| Helper Functions                         |
\*----------------------------------------*/
void snakeLog(const std::string &arg_str)
{
	std::cout << "SNAKE: " << arg_str << std::endl;
}

void snakeLog(const std::stringstream &arg_ss)
{
	std::cout << "SNAKE: " << arg_ss.str() << std::endl;
}

/// Returns a random tile position in a specified range
Vector2 getRandomPos(const int &max_x, const int &max_y, const int &min_x, const int &min_y)
{
	if (max_x < min_x || max_y < min_y || min_x < 0 || min_y < 0 || max_x > 11 || max_y > 8)
		throw std::runtime_error("getRandomPos(): INVALID PARAMETERS");

	// seed to change output for each game
	static std::random_device r;

	static std::default_random_engine generator{ r() };
	std::uniform_int_distribution<int> distribution_x(min_x, max_x);
	std::uniform_int_distribution<int> distribution_y(min_y, max_y);

	return Vector2
	{
		static_cast<float>(distribution_x(generator)), 
		static_cast<float>(distribution_y(generator)) 
	};
}

/// Returns an adjacent tile position to arg_pos in arg_dir
Vector2 directionToPos(const Vector2 &arg_pos, const Direction &arg_dir)
{
	switch (arg_dir)
	{
	case Direction::UP:
	{
		// Return error value if game border is hit
		if (arg_pos.y == 0.f)
			return Vector2{ -1.f, -1.f };
	
		else
			return Vector2{ arg_pos.x, arg_pos.y - 1.f };
	}

	case Direction::RIGHT:
	{
		if (arg_pos.x == 11.f)
			return Vector2{ -1.f, -1.f };

		else
			return Vector2{ arg_pos.x + 1.f, arg_pos.y };
	}

	case Direction::DOWN:
	{
		if (arg_pos.y == 8.f)
			return Vector2{ -1.f, -1.f };

		else
			return Vector2{ arg_pos.x, arg_pos.y + 1.f };
	}

	case Direction::LEFT:
	{
		if (arg_pos.x == 0.f)
			return Vector2{ -1.f, -1.f };
		
		else
			return Vector2{ arg_pos.x - 1.f, arg_pos.y };
	}

	default:
		throw std::runtime_error("getAdjacentPos(): INVALID PARAMETER");
	}
}
