/********************/
// Snack.hpp		//
// 15/05/2026		//
/********************/

#ifndef SNACK_HPP
#define SNACK_HPP

#include <raylib.h>

#include <iostream>


/*----------------------------------------*\
| Class                                    |
\*----------------------------------------*/
class Snack
{
public:

	Snack(const Vector2 &arg_pos);
	~Snack();

	const Vector2 &getPos();
	void setPos(const Vector2 &arg_pos);


private:

	Vector2 m_pos;
};



#endif // !SNACK_HPP
