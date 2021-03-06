/*
 * null_sounds.h
 *
 *  Created on: Nov 25, 2020
 *      Author: zamfi
 */

#ifndef COMPONENTS_NULL_SOUNDS_H_
#define COMPONENTS_NULL_SOUNDS_H_

#include "sounds.h"

class NullSounds : public Sounds
{
public:
	using Base = Sounds;
    void print(std::ostream& to) const
    {
    	to << "UseNullSounds ";
    }

    void update(const Time time_diff) {}
    bool changed() const { return false; }
    SoundID id() const { return -1; }
    double volume() const { return 0; }
};

#endif /* COMPONENTS_NULL_SOUNDS_H_ */
