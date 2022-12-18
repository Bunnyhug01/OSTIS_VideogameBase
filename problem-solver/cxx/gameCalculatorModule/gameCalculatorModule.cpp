/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "gameCalculatorModule.hpp"
#include "keynodes/keynodes.hpp"
#include "agents/GameCalculatorAgent.hpp"

using namespace gameCalculatorModule;

SC_IMPLEMENT_MODULE(GameCalculatorModule)

sc_result GameCalculatorModule::InitializeImpl()
{
  if (!gameCalculatorModule::Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(GameCalculatorAgent)

  return SC_RESULT_OK;
}

sc_result GameCalculatorModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(GameCalculatorAgent)

  return SC_RESULT_OK;
}
