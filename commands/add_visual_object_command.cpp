/*
 * add_visual_object_command.cpp
 *
 *  Created on: Nov 25, 2019
 *      Author: zamfi
 */

#include "add_visual_object_command.h"
#include "../globals.h"
#include "../entity.h"
#include "../components/absolute_position.h"
#include "../components/static_visuals.h"

void AddVisualObjectCommand::execute() const
{
    globals.entity_system.add_new_entity();
	Entity& entity = *(globals.entity_system.entity(globals.entity_system.last_id()));
	entity.set_position(new AbsolutePosition(m_x, m_y, m_w, m_h));
	entity.set_visuals(new StaticVisuals(m_spr_id, m_sprite));
    globals.access_entity_id = globals.entity_system.last_id();
}
