/*
 * clear_all_spritesheets_command.h
 *
 *  Created on: Nov 25, 2019
 *      Author: zamfi
 */

#ifndef COMMANDS_CLEAR_ALL_SPRITESHEETS_COMMAND_H_
#define COMMANDS_CLEAR_ALL_SPRITESHEETS_COMMAND_H_

#include "command_return_value.h"
#include "../globals.h"
#include "../systems/resource_system.h"

class ResourceSystem;
class InputSystem;
class RenderingSystem;

class ClearAllSpritesheetsCommand
{
public:
	ClearAllSpritesheetsCommand() {}

    template<typename EntitySystemT, typename CommandSystemT, typename AllSystemsT>
    CommandReturnValue operator()(CommandSystemT& command_system, EntitySystemT& entity_system, ResourceSystem& resource_system, InputSystem& input_system, RenderingSystem& rendering_system, AllSystemsT& all_systems, Globals& globals) const
	{
		resource_system.clear_spritesheets();
		return CommandReturnValue{0l};
	}
};

#endif /* COMMANDS_CLEAR_ALL_SPRITESHEETS_COMMAND_H_ */
