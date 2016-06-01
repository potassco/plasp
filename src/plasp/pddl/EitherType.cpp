#include <plasp/pddl/EitherType.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EitherType
//
////////////////////////////////////////////////////////////////////////////////////////////////////

EitherType &EitherType::parse(utils::Parser &parser, Context &context)
{
	parser.skipWhiteSpace();

	auto eitherType = std::make_unique<EitherType>(EitherType());

	parser.expect<std::string>("(either");
	parser.skipWhiteSpace();

	while (parser.currentCharacter() != ')')
	{
		eitherType->m_allowedTypes.push_back(&PrimitiveType::parse(parser, context));

		parser.skipWhiteSpace();
	}

	context.eitherTypes.emplace_back(std::move(eitherType));

	parser.expect<std::string>(")");

	return *context.eitherTypes.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<const PrimitiveType *> &EitherType::allowedTypes() const
{
	return m_allowedTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
