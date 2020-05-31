/*
 * use_attached_health_command.cpp
 *
 *  Created on: Nov 25, 2019
 *      Author: zamfi
 */

#include "use_attached_health_command.h"
#include "../globals.h"
#include "../utilities.h"
#include "../components/attached_health.h"

void UseAttachedHealthCommand::execute() const
{
	if(entity_system().entity(globals().access_entity_id))
		entity_system().entity(globals().access_entity_id)->set_health(new AttachedHealth(resolved_entity(m_attached_id), m_offset_hp, m_offset_max_hp));
	else
	{
		//error globals().access_entity_id
	}
}
