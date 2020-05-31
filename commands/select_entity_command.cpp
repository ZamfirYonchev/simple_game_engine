/*
 * select_entity_command.cpp
 *
 *  Created on: Nov 25, 2019
 *      Author: zamfi
 */

#include "select_entity_command.h"
#include "../globals.h"
#include "../utilities.h"

void SelectEntityCommand::execute() const
{
    globals().access_entity_id = resolved_entity(m_id);
    entity_system().add_accessed_entity(resolved_entity(m_id));
}
