/********************/
// Snack.cpp		//
// 15/05/2026		//
/********************/

#include "Snack.hpp"

				/////////////
				//  Snack  //
				/////////////
/*----------------------------------------*\
| Constructor                              |
\*----------------------------------------*/
Snack::Snack(const Vector2 &arg_pos)
	: m_pos(arg_pos)
{}

Snack::~Snack()
{}


/*----------------------------------------*\
| Member Functions                         |
\*----------------------------------------*/
const Vector2 &Snack::getPos()
{
	return m_pos;
}

void Snack::setPos(const Vector2 &arg_pos)
{
	m_pos = arg_pos;
}
