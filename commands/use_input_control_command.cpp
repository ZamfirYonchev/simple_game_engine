/*
 * use_input_control_command.cpp
 *
 *  Created on: Nov 25, 2019
 *      Author: zamfi
 */

#include "use_input_control_command.h"
#include "../globals.h"
#include "../components/input_control.h"

void UseInputControlCommand::execute() const
{
	if(globals.entity_system.entity(globals.access_entity_id))
		globals.entity_system.entity(globals.access_entity_id)->set_control(new InputControl(&globals.input_handler, m_shoot_id, m_shoot_cooldown));
	else
	{
		//error globals.access_entity_id
	}
}
