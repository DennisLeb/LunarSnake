/********************/
// Animation.hpp	//
// 17/05/2026		//
/********************/

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <raylib.h>

#include <iostream>


/*----------------------------------------*\
| Enum                                     |
\*----------------------------------------*/
enum class Animation_type
{
	REPEATING,
	PLAY_ONCE
};
// The REPEATING type won't be used for this project (raylib_Snake).


/*----------------------------------------*\
| Class                                    |
\*----------------------------------------*/
class Animation
{
public:

	Animation();

	void init(const Texture2D &arg_sprite_sheet, 
			  const uint8_t &arg_num_of_sprites,
			  const float &arg_speed = 0.1f,
			  const Animation_type &arg_anim_type = Animation_type::PLAY_ONCE);

	void updateAnimationFrame();
	void startAnimation();
	void resetAnimation();

	void setAnimationPos(const Vector2 &arg_pos);

	Rectangle getAnimationFrame() const;
	const Texture2D &getSpriteSheet() const;
	const Vector2 &getAnimationPos() const;
	const int &getSpriteWidth() const;
	const int &getSpriteHeight() const;

	// Only relevant for the PLAY_ONCE type
	const bool &isAnimationComplete() const;


private:

	// Imagine this as a 0-indexed array holding the animation frames
	std::unique_ptr<Texture2D> m_sprite_sheet;

	uint8_t m_first_frame;
	uint8_t m_last_frame;	// m_last_frame will always be m_sprite_sheet.size() - 1, i.e. the last element
	uint8_t m_cur_frame;

	int m_sprite_width;
	int m_sprite_height;

	float m_speed;
	float m_duration_left;

	// Only relevant for the PLAY_ONCE type
	bool m_anim_complete;

	Vector2 m_anim_pos;

	Animation_type m_anim_type;
};



#endif // !ANIMATION_HPP
