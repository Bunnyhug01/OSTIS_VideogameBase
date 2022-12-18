set(SC_GAME_CALCULATOR_MODULE_SRC "${PROBLEM_SOLVER_ROOT}/gameCalculatorModule")

set(SOURCES
		"gameCalculatorModule.cpp"
		"keynodes/keynodes.cpp"
		"agents/GameCalculatorAgent.cpp"
		)

set(HEADERS
		"gameCalculatorModule.hpp"
		"keynodes/keynodes.hpp"
		"agents/GameCalculatorAgent.hpp"
		)

include_directories(${SC_GAME_CALCULATOR_MODULE_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})

add_library(gameCalculatorModule SHARED ${SOURCES} ${HEADERS})
add_dependencies(gameCalculatorModule sc-memory-cpp sc-kpm-common sc-agents-common)
target_link_libraries(gameCalculatorModule sc-memory-cpp sc-kpm-common sc-agents-common)

sc_codegen(gameCalculatorModule ${SC_GAME_CALCULATOR_MODULE_SRC})

