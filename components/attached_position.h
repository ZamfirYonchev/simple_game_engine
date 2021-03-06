/*
 * attached_position.h
 *
 *  Created on: Nov 10, 2019
 *      Author: zamfi
 */

#ifndef COMPONENTS_ATTACHED_POSITION_H_
#define COMPONENTS_ATTACHED_POSITION_H_

#include "position.h"
#include "../types.h"
#include "../utilities.h"

template<typename EntitySystemT>
class AttachedPosition : public Position
{
public:
	using Base = Position;
    AttachedPosition
		(const EntityID attached_id
	   , const double offset_x
	   , const double offset_y
	   , const double offset_w
	   , const double offset_h
	   , const EntitySystemT& entity_system)
    : m_attached_id(attached_id)
    , m_offset_x(offset_x)
    , m_offset_y(offset_y)
    , m_offset_w(offset_w)
    , m_offset_h(offset_h)
	, m_entity_system(entity_system)
    {}

    void print(std::ostream& to) const
    {
    	to << "UseAttachedPosition "
    	   << m_offset_x << " "
		   << m_offset_y << " "
		   << m_offset_w << " "
		   << m_offset_h << " ";
    }

    double x() const
    {
    	return m_entity_system.template entity_component<Position>(m_attached_id).x()
    		+ absolute_or_scaled(m_offset_x, m_entity_system.template entity_component<Position>(m_attached_id).w());
    }

    double y() const
    {
    	return m_entity_system.template entity_component<Position>(m_attached_id).y()
    		+ absolute_or_scaled(m_offset_y, m_entity_system.template entity_component<Position>(m_attached_id).h());
    }

    double w() const
    {
    	return m_entity_system.template entity_component<Position>(m_attached_id).w()
    		 + absolute_or_scaled(m_offset_w, m_entity_system.template entity_component<Position>(m_attached_id).w());
    }

    double h() const
    {
    	return m_entity_system.template entity_component<Position>(m_attached_id).h()
    		 + absolute_or_scaled(m_offset_h, m_entity_system.template entity_component<Position>(m_attached_id).h());
    }

    void set_x(double val) {}
    void set_y(double val) {}
    void set_w(double val) {}
    void set_h(double val) {}

    void mod_x(double val) { m_offset_x += val; }
    void mod_y(double val) { m_offset_y += val; }
    void mod_w(double val) { m_offset_w += val; }
    void mod_h(double val) { m_offset_h += val; }

    EntityID m_attached_id;

private:
    double m_offset_x, m_offset_y, m_offset_w, m_offset_h;
    const EntitySystemT& m_entity_system;
};


#endif /* COMPONENTS_ATTACHED_POSITION_H_ */
