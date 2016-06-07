#include <plasp/pddl/expressions/Type.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
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

ExpressionPointer parseExistingPrimitiveType(Context &context, ExpressionContext &expressionContext)
{
	auto *primitiveType = PrimitiveType::parseAndFindOrCreate(context, expressionContext.domain);

	return std::make_unique<Reference<PrimitiveType>>(primitiveType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
