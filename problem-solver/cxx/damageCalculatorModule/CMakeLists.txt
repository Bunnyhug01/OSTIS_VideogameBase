set(SC_DAMAGE_CALCULATOR_MODULE_SRC "${PROBLEM_SOLVER_ROOT}/damageCalculatorModule")

set(SOURCES
		"damageCalculatorModule.cpp"
		"keynodes/keynodes.cpp"
		"agents/DamageCalculatorAgent.cpp"
		)

set(HEADERS
		"damageCalculatorModule.hpp"
		"keynodes/keynodes.hpp"
		"agents/DamageCalculatorAgent.hpp"
		)

include_directories(${SC_DAMAGE_CALCULATOR_MODULE_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})

add_library(damageCalculatorModule SHARED ${SOURCES} ${HEADERS})
add_dependencies(damageCalculatorModule sc-memory-cpp sc-kpm-common sc-agents-common)
target_link_libraries(damageCalculatorModule sc-memory-cpp sc-kpm-common sc-agents-common)

sc_codegen(damageCalculatorModule ${SC_DAMAGE_CALCULATOR_MODULE_SRC})

