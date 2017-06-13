/*#define CATCH_CONFIG_MAIN
#include <catch.hpp>
*/

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

	pddl::Tokenizer tokenizer;
	tokenizer.read("data/blocksworld-domain.pddl");
	tokenizer.read("data/blocksworld-problem.pddl");
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto description = pddl::parseDescription(context);

	std::cout << description.domain->requirements.size() << std::endl;
}
