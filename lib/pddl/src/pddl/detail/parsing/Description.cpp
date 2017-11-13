#include <pddl/detail/parsing/Description.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/detail/parsing/Domain.h>
#include <pddl/detail/parsing/Problem.h>
#include <pddl/detail/parsing/Utils.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

DescriptionParser::DescriptionParser(Context &context)
:	m_context{context},
	m_domainPosition{tokenize::InvalidStreamPosition},
	m_problemPosition{tokenize::InvalidStreamPosition}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::Description DescriptionParser::parse()
{
	auto &tokenizer = m_context.tokenizer;
	tokenizer.removeComments(";", "\n", false);

	findSections();

	if (m_domainPosition == tokenize::InvalidStreamPosition)
		throw ParserException("no PDDL domain specified");

	tokenizer.seek(m_domainPosition);

	auto domain = DomainParser(m_context).parse();

	// If no problem is given, return just the domain
	if (m_problemPosition == tokenize::InvalidStreamPosition)
		return {std::move(domain), std::experimental::nullopt};

	tokenizer.seek(m_problemPosition);

	auto problem = ProblemParser(m_context, *domain).parse();

	// TODO: check consistency
	// * check typing requirement
	// * check that typing is used consistently
	// * check that constants/objects, variables, and predicates aren't declared twice
	// * check section order
	// * check that preconditions and effects are well-formed
	return {std::move(domain), std::move(problem)};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DescriptionParser::findSections()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.skipWhiteSpace();

	while (!tokenizer.atEnd())
	{
		const auto position = tokenizer.position();

		tokenizer.expect<std::string>("(");

		if (m_context.mode == Mode::Compatibility && tokenizer.testAndReturn<std::string>("in-package"))
		{
			m_context.warningCallback(tokenizer.location(), "“in-package” section is not part of the PDDL 3.1 specification, ignoring section");

			skipSection(tokenizer);
			tokenizer.skipWhiteSpace();

			continue;
		}

		tokenizer.expect<std::string>("define");
		tokenizer.expect<std::string>("(");

		if (tokenizer.testAndSkip<std::string>("domain"))
		{
			if (m_domainPosition != tokenize::InvalidStreamPosition)
				throw ParserException(tokenizer.location(), "PDDL description may not contain two domains");

			m_domainPosition = position;
			skipSection(tokenizer);
			skipSection(tokenizer);
		}
		else if (m_context.tokenizer.testAndSkip<std::string>("problem"))
		{
			if (m_problemPosition != tokenize::InvalidStreamPosition)
				throw ParserException("PDDL description may not contain two problems currently");

			m_problemPosition = position;
			skipSection(tokenizer);
			skipSection(tokenizer);
		}
		else
		{
			const auto sectionIdentifier = tokenizer.get<std::string>();
			throw ParserException(tokenizer.location(), "unknown PDDL section “" + sectionIdentifier + "”");
		}

		tokenizer.skipWhiteSpace();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
