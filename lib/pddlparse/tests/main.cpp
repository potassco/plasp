/*#define CATCH_CONFIG_MAIN
#include <catch.hpp>
*/

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: reimplement unit tests
int main()
{
	const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

	pddl::Tokenizer tokenizer;
	tokenizer.read("data/blocksworld-domain.pddl");
	tokenizer.read("data/blocksworld-problem.pddl");
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto description = pddl::parseDescription(context);

	std::cout << description.domain->requirements.size() << std::endl;
	std::cout << description.domain->predicates.size() << std::endl;
	std::cout << description.domain->actions.size() << std::endl;
	std::cout << description.problem.value()->name << std::endl;
	std::cout << description.problem.value()->objects.size() << std::endl;
}
