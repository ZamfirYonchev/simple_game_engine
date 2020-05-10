/*
 * modify_health_command.cpp
 *
 *  Created on: Nov 25, 2019
 *      Author: zamfi
 */

#include "modify_health_command.h"
#include "../globals.h"
#include <cmath>

void ModifyHealthCommand::execute() const
{
	if(globals.entity_system.entity(globals.access_entity_id))
	{
		Health* health = globals.entity_system.entity(globals.access_entity_id)->health();

		if(m_max_hp == 0 && std::signbit(m_max_hp))
			health->set_max_hp(0);
		else
			health->set_max_hp(health->max_hp() + m_max_hp);

		if(m_hp == 0 && std::signbit(m_hp))
			health->set_hp(0);
		else
			health->set_hp(health->hp() + m_hp);

		if(m_hp_change == 0 && std::signbit(m_hp_change))
			health->set_hp_change(0);
		else
			health->mod_hp_change(m_hp_change);
	}
	else
	{
		//error globals.access_entity_id
	}
}

