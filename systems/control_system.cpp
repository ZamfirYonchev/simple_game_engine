/*
 * control_system.cpp
 *
 *  Created on: Nov 11, 2019
 *      Author: zamfi
 */

#include "control_system.h"
#include "../components/control.h"
#include "../components/health.h"
#include "../globals.h"
#include "../commands/call_procedure_command.h"
#include "../commands/modify_position_command.h"
#include "../commands/modify_control_command.h"
#include <algorithm>

void ControlSystem::update(const Time time_diff)
{
	std::for_each(cbegin(entities), cend(entities),
	[time_diff](const EntityID id)
	{
    	if(entity_system().entity(id))
    	{
			Entity& entity = *(entity_system().entity(id));
			Control* control = entity.control();
			const Health* health = entity.health();


			if(health->alive() && health->stunned() == false)
				control->update_decisions(time_diff);
			else
				control->clear_decisions();

			if(control->decision_attack() && control->attack_proc_id() >= 0)
			{
				const Position* position = entity.position();
				command_queue().push(std::make_unique<CallProcedureCommand>(control->attack_proc_id()));
				if(control->look_dir() == Control::LEFT)
				{
					command_queue().push(std::make_unique<ModifyPositionCommand>(position->x(), position->y(), 0, 0));
					command_queue().push(std::make_unique<ModifyControlCommand>(0, 0, 0, -0.0, -0.0));
					command_queue().push(std::make_unique<ModifyControlCommand>(0, 0, 0, -1, control->look_dir()));
				}
				else // Look dir is right
				{
					//command_queue().push(new ModifyPositionCommand(-0.0, 0, 0, 0));
					//command_queue().push(new ModifyPositionCommand(position->x()+position->w(), position->y(), 0, 0));
					command_queue().push(std::make_unique<ModifyPositionCommand>(position->x(), position->y(), 0, 0));
					command_queue().push(std::make_unique<ModifyControlCommand>(0, 0, 0, -0.0, -0.0));
					command_queue().push(std::make_unique<ModifyControlCommand>(0, 0, 0, 1, control->look_dir()));
				}
			}
    	}
    	else
    	{
    		//error *it
    	}
	});
}
