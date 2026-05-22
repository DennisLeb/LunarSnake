/********************/
// Tile.cpp			//
// 14/05/2026		//
/********************/

#include "Tile.hpp"

				////////////
				//  Tile  //
				////////////
/*----------------------------------------*\
| Constructor                              |
\*----------------------------------------*/
Tile::Tile(const Vector2 &pos)
	: m_pos(pos), 
	m_state(Tile_state::EMPTY)
{}


/*----------------------------------------*\
| Member Functions                         |
\*----------------------------------------*/
/// Return top-left coordinates (in pixels)
float Tile::getPosXTL() const 
{ 
	return m_pos.x * 60.f;
}

float Tile::getPosYTL() const 
{ 
	return m_pos.y * 60.f; 
}

/// Return center coordinates (in pixels)
float Tile::getPosXCenter() const 
{ 
	return m_pos.x * 60.f + 30.f; 
}

float Tile::getPosYCenter() const 
{ 
	return m_pos.y * 60.f + 30.f; 
}

/// Return tile coordinates
const Vector2 &Tile::getVec2() const 
{ 
	return m_pos; 
}

/// Return tile state
const Tile_state &Tile::getState() const 
{
	return m_state;
}

/// Set tile state
void Tile::setState(const Tile_state &arg_state) 
{ 
	m_state = arg_state; 
}
