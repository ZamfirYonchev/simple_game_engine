/*
 * movement_system.cpp
 *
 *  Created on: Nov 11, 2019
 *      Author: zamfi
 */

#include "movement_system.h"
#include "../components/movement.h"
#include "../components/control.h"
#include "../components/collision.h"
#include "../globals.h"
#include "../math_ext.h"

void MovementSystem::update(const Time time_delta)
{
    for(auto it = entities.begin(); it != entities.end(); ++it)
    {
    	if(entity_system().entity(*it))
    	{
			Control* control = entity_system().entity(*it)->control();
			Movement* movement = entity_system().entity(*it)->movement();
			Collision* collision = entity_system().entity(*it)->collision();
			Position* position  = entity_system().entity(*it)->position();

			if(movement->gravity_affected())
			{
				movement->mod_force_y(GRAVITY_ACCEL*movement->mass());

				if(control->decision_jump() && collision->standing_on() == Collision::GROUND)
					movement->mod_velocity_y(movement->jump_force()/movement->mass());
			}
			else
			{
				if(control->decision_jump())
					movement->mod_force_y(movement->move_force());
				if(control->decision_duck())
					movement->mod_force_y(-movement->move_force());
			}


			movement->mod_force_x(movement->move_force()*control->decision_walk());

			double vx_avg = movement->vx() + time_delta*movement->fx()/movement->mass()/2.0;
			double vy_avg = movement->vy() + time_delta*movement->fy()/movement->mass()/2.0;

			movement->mod_force_x(-vx_avg*movement->friction()); //account for air friction
			movement->mod_force_y(-vy_avg*movement->friction()); //account for air friction

			double sx = (movement->vx() + time_delta*movement->fx()/movement->mass()/2.0)*time_delta;
			double sy = (movement->vy() + time_delta*movement->fy()/movement->mass()/2.0)*time_delta;

			position->mod_x(sx);
			position->mod_y(sy);

			movement->mod_velocity_x(time_delta*movement->fx()/movement->mass());
			movement->mod_velocity_y(time_delta*movement->fy()/movement->mass());

			movement->set_force_x(0);
			movement->set_force_y(0);
    	}
    	else
    	{
    		//error *it
    	}
    }
}


