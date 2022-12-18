/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <sc-memory/cpp/sc_stream.hpp>
#include <sc-kpm/sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/AgentUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/CommonUtils.hpp>
#include <sc-kpm/sc-agents-common/keynodes/coreKeynodes.hpp>


#include "DamageCalculatorAgent.hpp"
#include "keynodes/keynodes.hpp"

#include <string>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace utils;

namespace damageCalculatorModule
{

string gamerIdtf_str;

int iterate(ScIterator5Ptr it, unique_ptr<ScMemoryContext> & ms_context, string log_str){

  int gamer_value = 0;

  while (it->Next())
  {
    
    ScAddr gamerIdtf = it->Get(0);
    gamerIdtf_str = ms_context->HelperGetSystemIdtf(gamerIdtf);
      
    ScAddr gamer_param = it->Get(2);
    string str1 = CommonUtils::getIdtfValue(ms_context.get(), gamer_param, Keynodes::nrel_main_idtf);

    if (str1.find('%') != string::npos){
      replace(str1.begin(), str1.end(), '%', '\0');
    }

    replace(str1.begin(), str1.end(), '.', ',');
    gamer_value = stof(str1);
  
    SC_LOG_INFO(log_str);
    SC_LOG_INFO(str1);
    
  }

  return gamer_value;

}

void in_file(string file_name, string gamerIdtf_str,  string new_health, string new_weapon_damage_value){

  ofstream fout;

  fout.open(file_name, ios::app);

  fout << gamerIdtf_str << "\n";
  fout << "=> nrel_new_health:\n\t" << new_health << "(*=>nrel_main_idtf: [" << new_health << "];;*);\n";
  fout << "=> nrel_new_weapon_damage:\n\t" << new_weapon_damage_value << "(*=>nrel_main_idtf: [" << new_weapon_damage_value << "];;*);;\n\n";

  fout.close();
}


SC_AGENT_IMPLEMENTATION(DamageCalculatorAgent)
{

 if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;

  SC_LOG_INFO("----------DamageCalculator begin----------");
  ScAddr actionNode = ms_context->GetEdgeTarget(edgeAddr);
  
  ScAddr gamer = IteratorUtils::getFirstFromSet(ms_context.get(), actionNode);


  if (!gamer.IsValid())
    return SC_RESULT_ERROR;

  ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, gamer);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_health);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_weapon_damage);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_damage_resistance);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_damage_threshold);
  

  ScIterator5Ptr it = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_health, true);
  ScIterator5Ptr it1 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_weapon_damage, true);
  ScIterator5Ptr it2 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_damage_resistance, true);
  ScIterator5Ptr it3 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_damage_threshold, true);

  int health_value = iterate(it, ms_context, "Health"); 
  int weapon_damage_value = iterate(it1, ms_context, "Weapon Damage");
  int damage_resistance_value = (iterate(it2, ms_context, "Damage Resistance") * weapon_damage_value) / 100;
  int damage_threshold_value = iterate(it3, ms_context, "Damage Threshold");
  
  int new_weapon_damage_value = weapon_damage_value - damage_resistance_value - damage_threshold_value;
  int new_health = health_value - new_weapon_damage_value;

  string health_str = to_string(new_health);
  string weapon_damage_str = to_string(new_weapon_damage_value);

  SC_LOG_INFO("New Health");
  SC_LOG_INFO(health_str);

  SC_LOG_INFO("New Weapon Damage");
  SC_LOG_INFO(weapon_damage_str);

  replace(health_str.begin(), health_str.end(), ',', '.');
  replace(weapon_damage_str.begin(), weapon_damage_str.end(), ',', '.');

  in_file("/home/user/ostis/kb/videogame_base/other/tests/testAnswer.scs", gamerIdtf_str, health_str, weapon_damage_str);  

  SC_LOG_INFO("----------DamageCalculator end----------");
  AgentUtils::finishAgentWork(ms_context.get(), actionNode, answer);
  
  return SC_RESULT_OK;
  
}

}

