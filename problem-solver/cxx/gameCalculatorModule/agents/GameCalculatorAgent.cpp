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


#include "GameCalculatorAgent.hpp"
#include "keynodes/keynodes.hpp"

#include <string>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace utils;

namespace gameCalculatorModule
{

string gamerIdtf_str;
string health_skill_operator;
string weapon_skill_operator;

string check_operation(string str, string nrel_str) {

  if (str.find('%') != string::npos){
    replace(str.begin(), str.end(), '%', '\0');
    if (str.find('*') != string::npos){
      if ("nrel_health_skill" == nrel_str){
        health_skill_operator = "%*";
      } 
      else{
        weapon_skill_operator = "%*";
      }

    }
    else {
      if ("nrel_health_skill" == nrel_str){
        health_skill_operator = "%";
      } 
      else{
        weapon_skill_operator = "%";
      }
    }
  }
  else if (str.find('*') != string::npos) {
    replace(str.begin(), str.end(), '*', '\0');
    if ("nrel_health_skill" == nrel_str){
      health_skill_operator = '*';
    }
    else{
      weapon_skill_operator = '*';
    }
    
  }

  return str;

}

float iterate(ScIterator5Ptr it, unique_ptr<ScMemoryContext> & ms_context, string log_str, string nrel_str){

  float gamer_value = 0.0;

  while (it->Next())
  {
    
    ScAddr gamerIdtf = it->Get(0);
    gamerIdtf_str = ms_context->HelperGetSystemIdtf(gamerIdtf);
      
    ScAddr gamer_param = it->Get(2);

    string str1 = CommonUtils::getIdtfValue(ms_context.get(), gamer_param, Keynodes::nrel_main_idtf);

    str1 = check_operation(str1, nrel_str);

    replace(str1.begin(), str1.end(), '.', ',');
    gamer_value = stof(str1);
  
    SC_LOG_INFO(log_str);
    SC_LOG_INFO(str1);
    
  }

  return gamer_value;

}

void in_file(string file_name, string gamerIdtf_str,  string new_health, string new_weapon_damage_value, string effectiveness_value){

  ofstream fout;

  fout.open(file_name, ios::app);

  fout << gamerIdtf_str << "\n";
  fout << "=> nrel_new_health:\n\t" << new_health << "(*=>nrel_main_idtf: [" << new_health << "];;*);\n";
  fout << "=> nrel_new_weapon_damage:\n\t" << new_weapon_damage_value << "(*=>nrel_main_idtf: [" << new_weapon_damage_value << "];;*);\n";
  fout << "=> nrel_effectiveness:\n\t" << effectiveness_value << "(*=>nrel_main_idtf: [" << effectiveness_value << "];;*);;\n\n";

  fout.close();
}


SC_AGENT_IMPLEMENTATION(GameCalculatorAgent)
{

 if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;

  SC_LOG_INFO("----------GameCalculator begin----------");
  ScAddr actionNode = ms_context->GetEdgeTarget(edgeAddr);
  
  ScAddr gamer = IteratorUtils::getFirstFromSet(ms_context.get(), actionNode);


  if (!gamer.IsValid())
    return SC_RESULT_ERROR;

  ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, gamer);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_health);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_armor);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_weapon_damage);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_weapon_skill);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_health_skill);
  

  ScIterator5Ptr it = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_health, true);
  ScIterator5Ptr it1 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_armor, true);
  ScIterator5Ptr it2 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_weapon_damage, true);
  ScIterator5Ptr it3 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_weapon_skill, true);
  ScIterator5Ptr it4 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_health_skill, true);
  ScIterator5Ptr it5 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_crit, true);
  ScIterator5Ptr it6 = IteratorUtils::getIterator5(ms_context.get(), gamer, Keynodes::nrel_dodge, true);

  float health_value = iterate(it, ms_context, "Health", "nrel_health"); 
  float armor_value = iterate(it1, ms_context, "Armor", "nrel_armor");
  float weapon_damage_value = iterate(it2, ms_context, "Weapon Damage", "nrel_weapon_damage");
  float weapon_skill_value = iterate(it3, ms_context, "Weapon Skill", "nrel_weapon_skill");
  float health_skill_value = iterate(it4, ms_context, "Health Skill", "nrel_health_skill");
  float crit_value = iterate(it5, ms_context, "Crit", "nrel_crit");
  float dodge_value = iterate(it6, ms_context, "Dodge", "nrel_dodge");
  
  float new_health;
  float new_weapon_damage_value;

  if (health_skill_operator.find('%') != string::npos){
    health_skill_value = (health_skill_value * health_value) / 100;

    if (health_skill_operator.find('*') != string::npos){
      new_health = (health_skill_value * health_value) / (1 - dodge_value / 100);
    }
    else{
      new_health = (health_skill_value + health_value) / (1 - dodge_value / 100);
    }
  }
  else if (health_skill_operator.find('*') != string::npos) {
    new_health = (health_skill_value * health_value) / (1 - dodge_value / 100);
  }
  else {
    new_health = (health_skill_value + health_value) / (1 - dodge_value / 100);
  }


  if (weapon_skill_operator.find('%') != string::npos){
    weapon_skill_value = (weapon_skill_value * weapon_damage_value) / 100;

    if (weapon_skill_operator.find('*') != string::npos){
      new_weapon_damage_value = (weapon_skill_value * weapon_damage_value) * (1 + crit_value / 100);
    }
    else{
      new_weapon_damage_value = (weapon_skill_value + weapon_damage_value) * (1 + crit_value / 100);
    }
  }
  else if (weapon_skill_operator.find('*') != string::npos){
    new_weapon_damage_value = (weapon_skill_value * weapon_damage_value) * (1 + crit_value / 100);
  }
  else {
    new_weapon_damage_value = (weapon_skill_value + weapon_damage_value) * (1 + crit_value / 100);
  }

  float effectiveness_value = (new_health + armor_value) * (new_weapon_damage_value); 

  string health_str = to_string(new_health);
  string weapon_damage_str = to_string(new_weapon_damage_value);
  string effectiveness_str = to_string(effectiveness_value);

  SC_LOG_INFO("New Health");
  SC_LOG_INFO(health_str);

  SC_LOG_INFO("New Weapon Damage");
  SC_LOG_INFO(weapon_damage_str);

  SC_LOG_INFO("Effectiveness");
  SC_LOG_INFO(effectiveness_str);

  replace(health_str.begin(), health_str.end(), ',', '.');
  replace(weapon_damage_str.begin(), weapon_damage_str.end(), ',', '.');
  replace(effectiveness_str.begin(), effectiveness_str.end(), ',', '.');

  in_file("/home/user/ostis/kb/videogame_base/other/tests/testAnswer.scs", gamerIdtf_str, health_str, weapon_damage_str, effectiveness_str);  

  SC_LOG_INFO("----------GameCalculator end----------");
  AgentUtils::finishAgentWork(ms_context.get(), actionNode, answer);
  
  return SC_RESULT_OK;
  
}

}

