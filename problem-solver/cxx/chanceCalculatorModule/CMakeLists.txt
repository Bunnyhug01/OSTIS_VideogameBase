set(SC_CHANCE_CALCULATOR_MODULE_SRC "${PROBLEM_SOLVER_ROOT}/chanceCalculatorModule")

set(SOURCES
		"chanceCalculatorModule.cpp"
		"keynodes/keynodes.cpp"
		"agents/ChanceCalculatorAgent.cpp"
		)

set(HEADERS
		"chanceCalculatorModule.hpp"
		"keynodes/keynodes.hpp"
		"agents/ChanceCalculatorAgent.hpp"
		)

include_directories(${SC_CHANCE_CALCULATOR_MODULE_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})

add_library(chanceCalculatorModule SHARED ${SOURCES} ${HEADERS})
add_dependencies(chanceCalculatorModule sc-memory-cpp sc-kpm-common sc-agents-common)
target_link_libraries(chanceCalculatorModule sc-memory-cpp sc-kpm-common sc-agents-common)

sc_codegen(chanceCalculatorModule ${SC_CHANCE_CALCULATOR_MODULE_SRC})

