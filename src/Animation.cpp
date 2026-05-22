/********************/
// Animation.cpp	//
// 17/05/2026		//
/********************/

#include "Animation.hpp"

				/////////////////
				//  Animation  //
				/////////////////
/*----------------------------------------*\
| Constructor                              |
\*----------------------------------------*/
/// Public
Animation::Animation()
	: m_sprite_sheet(nullptr),
	m_first_frame(0),
	m_last_frame(NULL),
	m_cur_frame(0),
	m_sprite_width(NULL),
	m_sprite_height(NULL),
	m_speed(0.1f),
	m_duration_left(m_speed),
	m_anim_complete(true),
	m_anim_pos(Vector2{ 0.f, 0.f }),
	m_anim_type(Animation_type::PLAY_ONCE)
{}


/*----------------------------------------*\
| Member Functions                         |
\*----------------------------------------*/
void Animation::init(const Texture2D &arg_sprite_sheet,
					 const uint8_t &arg_num_of_sprites,
					 const float &arg_speed, 
					 const Animation_type &arg_anim_type)
{
	m_sprite_sheet = std::make_unique<Texture2D>(arg_sprite_sheet);
	m_last_frame = arg_num_of_sprites - 1;
	m_speed = arg_speed;
	m_duration_left = arg_speed;
	m_anim_type =  arg_anim_type;

	m_sprite_width = m_sprite_sheet->width / arg_num_of_sprites;
	m_sprite_height = m_sprite_sheet->height;
}

void Animation::updateAnimationFrame()
{
	float delta_t = GetFrameTime();

	m_duration_left -= delta_t;
	if (m_duration_left > 0.f)
		return;


	m_duration_left = m_speed;
	m_cur_frame++;

	if (m_cur_frame < m_last_frame)
	{
		return;
	}
	
	m_anim_complete = true;

	switch (m_anim_type)
	{
	case Animation_type::REPEATING:
	{
		m_cur_frame = m_first_frame;
		break;
	}

	case Animation_type::PLAY_ONCE:
	{
		m_cur_frame = m_last_frame;
		break;
	}
	}
}

void Animation::startAnimation()
{
	m_cur_frame = m_first_frame;
	m_anim_complete = false;
}

void Animation::resetAnimation()
{
	m_cur_frame = m_first_frame;
	m_anim_complete = true;
}

void Animation::setAnimationPos(const Vector2 &arg_pos)
{
	m_anim_pos = arg_pos;
}

/// Returns a Rectangle inside of m_sprite_sheet, which contains the current animation frame
Rectangle Animation::getAnimationFrame() const
{
	uint16_t pos_x = (m_cur_frame % m_last_frame + 1) * m_sprite_width;
	uint16_t pos_y = (m_cur_frame / m_last_frame + 1) * m_sprite_height;

	return Rectangle{ static_cast<float>(pos_x), static_cast<float>(pos_y), static_cast<float>(m_sprite_width), static_cast<float>(m_sprite_height) };
}

const Texture2D &Animation::getSpriteSheet() const
{
	return *m_sprite_sheet;
}

const Vector2 &Animation::getAnimationPos() const
{
	return m_anim_pos;
}

const int &Animation::getSpriteWidth() const
{
	return m_sprite_width;
}

const int &Animation::getSpriteHeight() const
{
	return m_sprite_height;
}

const bool &Animation::isAnimationComplete() const
{
	return m_anim_complete;
}
