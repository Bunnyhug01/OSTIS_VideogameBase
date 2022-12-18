/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"

#include "keynodes.generated.hpp"

namespace chanceCalculatorModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:

  SC_PROPERTY(Keynode("question_chance_calculator"), ForceCreate)
  static ScAddr question_chance_calculator;
	
  SC_PROPERTY(Keynode("nrel_shots_count"), ForceCreate)
  static ScAddr nrel_shots_count;
  
  SC_PROPERTY(Keynode("nrel_shot_damage"), ForceCreate)
  static ScAddr nrel_shot_damage;
  
  SC_PROPERTY(Keynode("nrel_to_hit"), ForceCreate)
  static ScAddr nrel_to_hit;
  
  SC_PROPERTY(Keynode("nrel_main_idtf"), ForceCreate)
  static ScAddr nrel_main_idtf;
  
};

} // namespace chanceCalculatorModule
