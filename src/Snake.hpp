/********************/
// Snake.hpp		//
// 14/05/2026		//
/********************/

#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <raylib.h>

#include <vector>
#include <iostream>
#include <sstream>
#include <random>


/*----------------------------------------*\
| Enum                                     |
\*----------------------------------------*/
enum class Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};


/*----------------------------------------*\
| Classes                                  |
\*----------------------------------------*/
class Snake
{
public:

	explicit Snake();
	
	class Body
	{
	public:
		explicit Body(const Vector2 &arg_pos, const Direction &arg_dir);
		~Body();

		const Vector2 &getPos() const;
		const Direction &getDir() const;

		void setPos(const Vector2 &arg_pos);
		void setDir(const Direction &arg_dir);

	private:
		Vector2 m_pos;
		Direction m_dir;
	};


private:

	// Direction buffer to store movement input until the snake should move
	Direction m_dir_buffer;
	Direction m_dir;

	// m_head's m_dir must ALWAYS be equal to Snake's m_dir (after buffer is applied)
	Body m_head;
	std::vector<Body> m_body_vec;


public:

	const Snake::Body &getHead() const;
	const std::vector<Body> &getBodyVec() const;
	const Direction &getDir() const;

	void setHeadPos(const Vector2 &arg_pos);
	void setBodySegmentPos(const Vector2 &arg_pos, const size_t &arg_index);
	void setBodySegmentDir(const Direction &arg_dir, const size_t &arg_index);

	void handleDirectionInput(const int &arg_key);

	void applyDirectionBuffer();

	void attachNewBody(const Vector2 &arg_pos, const Direction &arg_dir);
};


/*----------------------------------------*\
| Operator Overloading                     |
\*----------------------------------------*/
Direction operator+(const Direction &dir, const int &num);

Direction operator++(const Direction &dir);


/*----------------------------------------*\
| Helper Functions                         |
\*----------------------------------------*/
void snakeLog(const std::string &arg_str);
void snakeLog(const std::stringstream &arg_ss);

Vector2 getRandomPos(const int &max_x, const int &max_y, const int &min_x = 0, const int &min_y = 0);

Vector2 directionToPos(const Vector2 &arg_pos, const Direction &arg_dir);



#endif // !SNAKE_HPP
