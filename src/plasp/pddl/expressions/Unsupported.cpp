#include <plasp/pddl/expressions/Unsupported.h>

#include <plasp/pddl/IO.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Unsupported
//
////////////////////////////////////////////////////////////////////////////////////////////////////

UnsupportedPointer Unsupported::parse(Context &context)
{
	auto &tokenizer = context.tokenizer;

	auto expression = UnsupportedPointer(new Unsupported);

	tokenizer.expect<std::string>("(");

	expression->m_type = tokenizer.getIdentifier();

	context.logger.log(output::Priority::Warning, tokenizer, "expression type “" + expression->m_type + "” currently unsupported in this context");

	skipSection(tokenizer);

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Unsupported::type() const
{
	return m_type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Unsupported::print(std::ostream &ostream) const
{
	// TODO: implement correctly
	ostream << "(<unsupported expression “" << m_type << "”>)";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
