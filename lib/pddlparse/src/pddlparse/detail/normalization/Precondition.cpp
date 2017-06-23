#include <pddlparse/detail/normalization/Precondition.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/AtomicFormula.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer addDerivedPredicate(normalizedAST::DerivedPredicateDeclarations &derivedPredicates, const std::string &type)
{
	auto name = "derived-" + type + "-" + std::to_string(derivedPredicates.size() + 1);

	derivedPredicates.emplace_back(std::make_unique<normalizedAST::DerivedPredicateDeclaration>(std::move(name), normalizedAST::VariableDeclarations()));

	return std::make_unique<normalizedAST::DerivedPredicate>(normalizedAST::DerivedPredicate::Arguments(), derivedPredicates.back().get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer normalizeNested(ast::AndPointer<ast::Precondition> &, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	// TODO: handle arguments recursively
	return addDerivedPredicate(derivedPredicates, "and");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer normalizeNested(ast::AtomicFormula &, normalizedAST::DerivedPredicateDeclarations &)
{
	// TODO: add explanation
	throw std::logic_error("atomic formulas should have been handled earlier");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer normalizeNested(ast::ExistsPointer<ast::Precondition> &, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	// TODO: handle arguments recursively
	return addDerivedPredicate(derivedPredicates, "exists");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer normalizeNested(ast::ForAllPointer<ast::Precondition> &, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	// TODO: handle arguments recursively
	return addDerivedPredicate(derivedPredicates, "forall");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer normalizeNested(ast::ImplyPointer<ast::Precondition> &, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	// TODO: handle arguments recursively
	return addDerivedPredicate(derivedPredicates, "imply");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer normalizeNested(ast::NotPointer<ast::Precondition> &, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	// TODO: handle arguments recursively
	return addDerivedPredicate(derivedPredicates, "not");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer normalizeNested(ast::OrPointer<ast::Precondition> &, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	// TODO: handle arguments recursively
	return addDerivedPredicate(derivedPredicates, "or");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::PredicatePointer normalize(ast::UnsupportedPointer &&unsupported, normalizedAST::DerivedPredicateDeclarations &)
{
	throw NormalizationException("“" + unsupported->type + "” expressions in preconditions can’t be normalized currently");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Normalize top-level negations
normalizedAST::NotPointer<normalizedAST::AtomicFormula> normalize(ast::NotPointer<ast::Precondition> &&not_, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	// “not” expressions may be nested one time to form simple literals
	if (not_->argument.is<ast::AtomicFormula>())
		return std::make_unique<normalizedAST::Not<normalizedAST::AtomicFormula>>(normalize(std::move(not_->argument.get<ast::AtomicFormula>())));

	const auto handleNested =
		[&](auto &nested) -> normalizedAST::AtomicFormula
		{
			return normalizeNested(nested, derivedPredicates);
		};

	const auto handleUnsupported =
		[&](ast::UnsupportedPointer &unsupported) -> normalizedAST::AtomicFormula
		{
			throw NormalizationException("“" + unsupported->type + "” expressions in preconditions can’t be normalized currently");
		};

	auto normalizedArgument = not_->argument.match(handleNested, handleUnsupported);

	return std::make_unique<normalizedAST::Not<normalizedAST::AtomicFormula>>(std::move(normalizedArgument));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Normalize top-level conjunctions
normalizedAST::AndPointer<normalizedAST::Literal> normalize(ast::AndPointer<ast::Precondition> &&and_, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	normalizedAST::And<normalizedAST::Literal>::Arguments arguments;

	arguments.reserve(and_->arguments.size());

	const auto handleAtomicFormula =
		[&](ast::AtomicFormula &atomicFormula) -> normalizedAST::Literal
		{
			return normalize(std::move(atomicFormula));
		};

	const auto handleNot =
		[&](ast::NotPointer<ast::Precondition> &not_) -> normalizedAST::Literal
		{
			return normalize(std::move(not_), derivedPredicates);
		};

	const auto handleNested =
		[&](auto &nested) -> normalizedAST::Literal
		{
			return normalizeNested(nested, derivedPredicates);
		};

	const auto handleUnsupported =
		[&](ast::UnsupportedPointer &unsupported) -> normalizedAST::Literal
		{
			throw NormalizationException("“" + unsupported->type + "” expressions in preconditions can’t be normalized currently");
		};

	for (auto &&argument : and_->arguments)
	{
		auto normalizedArgument = argument.match(handleAtomicFormula, handleNot, handleNested, handleUnsupported);

		arguments.emplace_back(std::move(normalizedArgument));
	}

	return std::make_unique<normalizedAST::And<normalizedAST::Literal>>(std::move(arguments));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Precondition normalize(ast::Precondition &&precondition, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	/*
	reduce

	AtomicFormula,
	AndPointer<Precondition>,
	NotPointer<Precondition>,
	UnsupportedPointer>;

	ExistsPointer<Precondition>,
	ForAllPointer<Precondition>,
	ImplyPointer<Precondition>,
	OrPointer<Precondition>,

	to

	Literal,
	AndPointer<Literal>>;
	*/
	const auto handleAtomicFormula =
		[&](ast::AtomicFormula &atomicFormula) -> normalizedAST::Precondition
		{
			return normalize(std::move(atomicFormula));
		};

	const auto handleNot =
		[&](ast::NotPointer<ast::Precondition> &not_) -> normalizedAST::Precondition
		{
			return normalize(std::move(not_), derivedPredicates);
		};

	const auto handleAnd =
		[&](ast::AndPointer<ast::Precondition> &and_) -> normalizedAST::Precondition
		{
			return normalize(std::move(and_), derivedPredicates);
		};

	const auto handleNested =
		[&](auto &nested) -> normalizedAST::Precondition
		{
			return normalizeNested(nested, derivedPredicates);
		};

	const auto handleUnsupported =
		[&](ast::UnsupportedPointer &unsupported) -> normalizedAST::Precondition
		{
			throw NormalizationException("“" + unsupported->type + "” expressions in preconditions can’t be normalized currently");
		};

	return precondition.match(handleAtomicFormula, handleNot, handleAnd, handleUnsupported, handleNested);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
