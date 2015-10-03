/*
 * Copyright 2015 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

 #pragma once

#include "Actuator.h"
#include "SetPoint.h"
#include "defaultDevices.h"
 
/*
 * A linear actuator that sets a setpoint to reference setpoint + actuator value
 */
class ActuatorSetPoint : public ActuatorRange
{
public:
    ActuatorSetPoint(SetPoint * targ = &defaultSetPoint, // setpoint to manipulate
                     const SetPoint * ref = &defaultSetPoint, //setpoint to offset from
                     temp_t mini = temp_t::min(), // minimum actuator value (targ - ref)
                     temp_t maxi = temp_t::max()){ // maximum actuator value
        target = targ;
        reference = ref;
        minimum = mini;
        maximum = maxi;
    }

    void setActive(bool active) {
        // do nothing, doesn't mean anything for a SetPoint Actuator
    }
    bool isActive() const{
        temp_t referenceTemp = reference->read();
        if(referenceTemp.isDisabledOrInvalid()){
            return false;
        }
        return target->read() != reference->read();
    }

    void setValue(temp_t const& val) {
        temp_t offset = val;
        if(offset < minimum){
            offset = minimum;
        }
        else if(offset > maximum){
            offset = maximum;
        }
        target->write(reference->read() + offset);
    }

    temp_t readValue() const{
        return target->read() - reference->read();
    }

    temp_t min() const{
        return minimum;
    }

    temp_t max() const{
        return maximum;
    }

private:
    SetPoint * target;
    const SetPoint * reference;
    temp_t minimum;
    temp_t maximum;
};