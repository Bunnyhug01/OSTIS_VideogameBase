/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"

#include "keynodes.generated.hpp"

namespace gameCalculatorModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:

  SC_PROPERTY(Keynode("question_game_calculator"), ForceCreate)
  static ScAddr question_game_calculator;
	
  SC_PROPERTY(Keynode("nrel_health"), ForceCreate)
  static ScAddr nrel_health;
  
  SC_PROPERTY(Keynode("nrel_armor"), ForceCreate)
  static ScAddr nrel_armor;
  
  SC_PROPERTY(Keynode("nrel_weapon_damage"), ForceCreate)
  static ScAddr nrel_weapon_damage;
  
  SC_PROPERTY(Keynode("nrel_weapon_skill"), ForceCreate)
  static ScAddr nrel_weapon_skill;
  
  SC_PROPERTY(Keynode("nrel_health_skill"), ForceCreate)
  static ScAddr nrel_health_skill;
  
  SC_PROPERTY(Keynode("nrel_crit"), ForceCreate)
  static ScAddr nrel_crit;
  
  SC_PROPERTY(Keynode("nrel_dodge"), ForceCreate)
  static ScAddr nrel_dodge;
  
  SC_PROPERTY(Keynode("nrel_main_idtf"), ForceCreate)
  static ScAddr nrel_main_idtf;
  
};

} // namespace gameCalculatorModule
