/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "keynodes.hpp"

#include <sc-memory/cpp/sc_memory.hpp>

namespace damageCalculatorModule
{

ScAddr Keynodes::question_damage_calculator;
ScAddr Keynodes::nrel_health;
ScAddr Keynodes::nrel_weapon_damage;
ScAddr Keynodes::nrel_damage_threshold;
ScAddr Keynodes::nrel_damage_resistance;
ScAddr Keynodes::nrel_main_idtf;
}
