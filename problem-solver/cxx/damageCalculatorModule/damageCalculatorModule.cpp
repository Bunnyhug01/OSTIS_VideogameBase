/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "damageCalculatorModule.hpp"
#include "keynodes/keynodes.hpp"
#include "agents/DamageCalculatorAgent.hpp"

using namespace damageCalculatorModule;

SC_IMPLEMENT_MODULE(DamageCalculatorModule)

sc_result DamageCalculatorModule::InitializeImpl()
{
  if (!damageCalculatorModule::Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(DamageCalculatorAgent)

  return SC_RESULT_OK;
}

sc_result DamageCalculatorModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(DamageCalculatorAgent)

  return SC_RESULT_OK;
}
