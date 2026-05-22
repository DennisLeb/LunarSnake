/********************/
// Tile.hpp			//
// 14/05/2026		//
/********************/

#ifndef TILE_HPP
#define TILE_HPP

#include <raylib.h>


/*----------------------------------------*\
| Enum                                     |
\*----------------------------------------*/
enum class Tile_state
{
	EMPTY,
	BODY,
	HEAD,
	SNACK
};


/*----------------------------------------*\
| Class                                    |
\*----------------------------------------*/
class Tile
{
public:

	explicit Tile(const Vector2 &pos);

	float getPosXTL() const;
	float getPosYTL() const;

	float getPosXCenter() const;
	float getPosYCenter() const;

	const Vector2 &getVec2() const;

	const Tile_state &getState() const;

	void setState(const Tile_state &arg_state);


private:

	Vector2 m_pos;
	Tile_state m_state;
};



#endif // !TILE_HPP
