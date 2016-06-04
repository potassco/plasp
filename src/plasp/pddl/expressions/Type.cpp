#include <plasp/pddl/expressions/Type.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/expressions/Reference.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExistingPrimitiveType(utils::Parser &parser, Context &context, const Variables &parameters)
{
	auto reference = std::make_unique<Reference<PrimitiveType>>(PrimitiveType::parseExisting(parser, context));

	return reference;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/*ExpressionPointer parseExistingType(utils::Parser &parser, Context &context, const Variables &parameters)
{
	parser.skipWhiteSpace();

	// Parse either type (always begins with opening parenthesis)
	if (parser.currentCharacter() == '(')
		return Either::parse(parser, context, parameters, parseExistingPrimitiveType);

	// Parse primitive type
	auto type = std::make_unique<Reference<PrimitiveType>>(PrimitiveType::parseExisting(parser, context));

	return type;
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
