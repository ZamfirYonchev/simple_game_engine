/*
 * add_character_command.h
 *
 *  Created on: Nov 25, 2019
 *      Author: zamfi
 */

#ifndef COMMANDS_ADD_CHARACTER_COMMAND_H_
#define COMMANDS_ADD_CHARACTER_COMMAND_H_

#include "command.h"
#include "../types.h"

class AddCharacterCommand : public Command
{
public:
    AddCharacterCommand(double x, double y, double w, double h, double hp, SpritesheetID spr_id)
	: m_x(x)
	, m_y(y)
	, m_w(w)
	, m_h(h)
	, m_hp(hp)
	, m_spr_id(spr_id)
	{}

    void execute() const;
    Command* clone() { return new AddCharacterCommand(m_x, m_y, m_w, m_h, m_hp, m_spr_id); }

private:
    double m_x, m_y, m_w, m_h, m_hp;
    SpritesheetID m_spr_id;
};



#endif /* COMMANDS_ADD_CHARACTER_COMMAND_H_ */
