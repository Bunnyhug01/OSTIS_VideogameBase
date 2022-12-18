/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "chanceCalculatorModule.hpp"
#include "keynodes/keynodes.hpp"
#include "agents/ChanceCalculatorAgent.hpp"

using namespace chanceCalculatorModule;

SC_IMPLEMENT_MODULE(ChanceCalculatorModule)

sc_result ChanceCalculatorModule::InitializeImpl()
{
  if (!chanceCalculatorModule::Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(ChanceCalculatorAgent)

  return SC_RESULT_OK;
}

sc_result ChanceCalculatorModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ChanceCalculatorAgent)

  return SC_RESULT_OK;
}
