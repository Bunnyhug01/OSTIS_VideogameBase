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


#include "ChanceCalculatorAgent.hpp"
#include "keynodes/keynodes.hpp"

#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;
using namespace utils;

namespace chanceCalculatorModule
{

string weaponIdtf_str;
vector<float> to_hit;
vector<int> hit_chance;
vector<float> avg_damage;

float iterate(ScIterator5Ptr it, unique_ptr<ScMemoryContext> & ms_context, string log_str){

  float weapon_value = 0;
  string weaponNrelIdtf_str;

  while (it->Next())
  {
    
    ScAddr weaponIdtf = it->Get(0);
    weaponIdtf_str = ms_context->HelperGetSystemIdtf(weaponIdtf);

    ScAddr weaponNrelIdtf = it->Get(4);
    weaponNrelIdtf_str = ms_context->HelperGetSystemIdtf(weaponNrelIdtf);
      
    ScAddr gamer_param = it->Get(2);
    string str1 = CommonUtils::getIdtfValue(ms_context.get(), gamer_param, Keynodes::nrel_main_idtf);

    if (str1.find('%') != string::npos){
      replace(str1.begin(), str1.end(), '%', '\0');
    }

    replace(str1.begin(), str1.end(), '.', ',');
    weapon_value = stof(str1);

    if (weaponNrelIdtf_str == "nrel_to_hit"){
      to_hit.push_back(weapon_value);
    }

    SC_LOG_INFO(log_str);
    SC_LOG_INFO(str1);
    
  }

  return weapon_value;

}

void in_file(string file_name){

  ofstream fout;

  fout.open(file_name, ios::app);

  fout << weaponIdtf_str << "\n";

  for (int i = 0; i < hit_chance.size(); i++){

    string chance_str = to_string(hit_chance[i]);
    string avg_damage_str = to_string(avg_damage[i]);

    replace(chance_str.begin(), chance_str.end(), ',', '.');
    replace(avg_damage_str.begin(), avg_damage_str.end(), ',', '.');

    fout << "=> nrel_hit_chance:\n\t" << chance_str << "(*=>nrel_main_idtf: [" << chance_str << "%];;*);\n";
    fout << "=> nrel_avg_damage:\n\t" << avg_damage_str << "(*=>nrel_main_idtf: [" << avg_damage_str << "];;*);";

  }

  fout << ";\n";
  
  fout.close();
}


SC_AGENT_IMPLEMENTATION(ChanceCalculatorAgent)
{

 if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;

  SC_LOG_INFO("----------ChanceCalculator begin----------");
  ScAddr actionNode = ms_context->GetEdgeTarget(edgeAddr);
  
  ScAddr weapon = IteratorUtils::getFirstFromSet(ms_context.get(), actionNode);


  if (!weapon.IsValid())
    return SC_RESULT_ERROR;

  ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, weapon);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_shots_count);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_shot_damage);
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_to_hit);

  ScIterator5Ptr it = IteratorUtils::getIterator5(ms_context.get(), weapon, Keynodes::nrel_shots_count, true);
  ScIterator5Ptr it1 = IteratorUtils::getIterator5(ms_context.get(), weapon, Keynodes::nrel_shot_damage, true);
  ScIterator5Ptr it2 = IteratorUtils::getIterator5(ms_context.get(), weapon, Keynodes::nrel_to_hit, true);

  float shots_count_value = iterate(it, ms_context, "Shots Count"); 
  float shot_damage_value = iterate(it1, ms_context, "Shot Damage");

  iterate(it2, ms_context, "To Hit");

  for (int i = 0; i < to_hit.size(); i++){
    float chance = 1 - ( (to_hit[i] - 1) / 6);
    float avg_damage_value = shots_count_value * shot_damage_value * chance;

    chance *= 100;

    hit_chance.push_back((int)chance);
    avg_damage.push_back(avg_damage_value);

    SC_LOG_INFO("To Hit, Hit Chance, Average Damage");
    SC_LOG_INFO(to_string(to_hit[i]) + ' ' + to_string(chance) + ' ' + to_string(avg_damage_value));
    
  }

  in_file("/home/user/ostis/kb/videogame_base/other/tests/testAnswer.scs");  

  SC_LOG_INFO("----------ChanceCalculator end----------");
  AgentUtils::finishAgentWork(ms_context.get(), actionNode, answer);
  
  return SC_RESULT_OK;
  
}

}

