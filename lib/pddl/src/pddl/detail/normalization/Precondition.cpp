#include <pddl/detail/normalization/Precondition.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/AtomicFormula.h>
#include <pddl/detail/normalization/CollectFreeVariables.h>
#include <pddl/detail/normalization/Reduction.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward Declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::AndPointer<ast::Precondition> &and_, detail::NormalizationContext &normalizationContext);
normalizedAST::Literal normalizeNested(ast::AtomicFormula &, detail::NormalizationContext &);
normalizedAST::Literal normalizeNested(ast::ExistsPointer<ast::Precondition> &exists, detail::NormalizationContext &normalizationContext);
normalizedAST::Literal normalizeNested(ast::ForAllPointer<ast::Precondition> &forAll, detail::NormalizationContext &);
normalizedAST::Literal normalizeNested(ast::ImplyPointer<ast::Precondition> &, detail::NormalizationContext &);
normalizedAST::Literal normalizeNested(ast::NotPointer<ast::Precondition> &not_, detail::NormalizationContext &normalizationContext);
normalizedAST::Literal normalizeNested(ast::OrPointer<ast::Precondition> &or_, detail::NormalizationContext &normalizationContext);
normalizedAST::AndPointer<normalizedAST::Literal> normalizeTopLevel(ast::AndPointer<ast::Precondition> &and_, detail::NormalizationContext &normalizationContext);
normalizedAST::AtomicFormula normalizeTopLevel(ast::AtomicFormula &, detail::NormalizationContext &);
normalizedAST::Literal normalizeTopLevel(ast::ExistsPointer<ast::Precondition> &exists, detail::NormalizationContext &normalizationContext);
normalizedAST::Literal normalizeTopLevel(ast::ForAllPointer<ast::Precondition> &forAll, detail::NormalizationContext &);
normalizedAST::Literal normalizeTopLevel(ast::ImplyPointer<ast::Precondition> &, detail::NormalizationContext &);
normalizedAST::NotPointer<normalizedAST::AtomicFormula> normalizeTopLevel(ast::NotPointer<ast::Precondition> &not_, detail::NormalizationContext &normalizationContext);
normalizedAST::OrPointer<normalizedAST::Literal> normalizeTopLevel(ast::OrPointer<ast::Precondition> &or_, detail::NormalizationContext &normalizationContext);

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DerivedPredicatePointer addDerivedPredicate(const std::vector<normalizedAST::VariableDeclaration *> &parameters, detail::NormalizationContext &normalizationContext)
{
	auto &derivedPredicates = normalizationContext.derivedPredicates;
	const auto derivedPredicateID = normalizationContext.derivedPredicateIDStart + derivedPredicates.size();
	auto name = "derived-predicate-" + std::to_string(derivedPredicateID);

	normalizedAST::DerivedPredicate::Arguments arguments;
	arguments.reserve(parameters.size());

	for (const auto &parameter : parameters)
		arguments.emplace_back(std::make_unique<normalizedAST::Variable>(parameter));

	derivedPredicates.emplace_back(std::make_unique<normalizedAST::DerivedPredicateDeclaration>());

	auto *derivedPredicate = derivedPredicates.back().get();
	derivedPredicate->name = std::move(name);
	derivedPredicate->parameters = std::move(parameters);

	return std::make_unique<normalizedAST::DerivedPredicate>(std::move(arguments), derivedPredicate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::AndPointer<ast::Precondition> &and_, detail::NormalizationContext &normalizationContext)
{
	std::vector<normalizedAST::VariableDeclaration *> parameters;
	VariableStack variableStack;

	collectFreeVariables(and_, parameters, variableStack);

	auto derivedPredicate = addDerivedPredicate(parameters, normalizationContext);

	normalizedAST::And<normalizedAST::Literal>::Arguments normalizedArguments;
	normalizedArguments.reserve(and_->arguments.size());

	for (auto &argument : and_->arguments)
		normalizedArguments.emplace_back(argument.match(
			[&](auto &x) -> normalizedAST::Literal
			{
				return normalizeNested(x, normalizationContext);
			}));

	derivedPredicate->declaration->precondition = std::make_unique<normalizedAST::And<normalizedAST::Literal>>(std::move(normalizedArguments));

	// TODO: investigate, could be a compiler bug
	return std::move(derivedPredicate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::AtomicFormula &atomicFormula, detail::NormalizationContext &)
{
	return normalize(std::move(atomicFormula));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::ExistsPointer<ast::Precondition> &exists, detail::NormalizationContext &normalizationContext)
{
	std::vector<normalizedAST::VariableDeclaration *> parameters;
	VariableStack variableStack;

	collectFreeVariables(exists, parameters, variableStack);

	auto derivedPredicate = addDerivedPredicate(parameters, normalizationContext);

	derivedPredicate->declaration->existentialParameters = std::move(exists->parameters);
	derivedPredicate->declaration->precondition = exists->argument.match(
		[&](auto &x) -> normalizedAST::DerivedPredicatePrecondition
		{
			return normalizeTopLevel(x, normalizationContext);
		});

	// TODO: investigate, could be a compiler bug
	return std::move(derivedPredicate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::ForAllPointer<ast::Precondition> &, detail::NormalizationContext &)
{
	// “forall” expressions should be reduced to negated “exists” statements at this point
	throw std::logic_error("precondition not in normal form (forall), please report to the bug tracker");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::ImplyPointer<ast::Precondition> &, detail::NormalizationContext &)
{
	// “imply” expressions should be reduced to disjunctions at this point
	throw std::logic_error("precondition not in normal form (imply), please report to the bug tracker");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::NotPointer<ast::Precondition> &not_, detail::NormalizationContext &normalizationContext)
{
	auto normalizedArgumentLiteral = not_->argument.match(
		[&](auto &x) -> normalizedAST::Literal
		{
			return normalizeNested(x, normalizationContext);
		});

	// Multiple negations should be eliminated at this point
	if (normalizedArgumentLiteral.is<normalizedAST::NotPointer<normalizedAST::AtomicFormula>>())
		throw std::logic_error("precondition not in normal form (multiple negation), please report to the bug tracker");

	auto &normalizedArgument = normalizedArgumentLiteral.get<normalizedAST::AtomicFormula>();

	return std::make_unique<normalizedAST::Not<normalizedAST::AtomicFormula>>(std::move(normalizedArgument));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeNested(ast::OrPointer<ast::Precondition> &or_, detail::NormalizationContext &normalizationContext)
{
	std::vector<normalizedAST::VariableDeclaration *> parameters;
	VariableStack variableStack;

	collectFreeVariables(or_, parameters, variableStack);

	auto derivedPredicate = addDerivedPredicate(parameters, normalizationContext);

	normalizedAST::Or<normalizedAST::Literal>::Arguments normalizedArguments;
	normalizedArguments.reserve(or_->arguments.size());

	for (auto &argument : or_->arguments)
		normalizedArguments.emplace_back(argument.match(
			[&](auto &x) -> normalizedAST::Literal
			{
				return normalizeNested(x, normalizationContext);
			}));

	derivedPredicate->declaration->precondition = std::make_unique<normalizedAST::Or<normalizedAST::Literal>>(std::move(normalizedArguments));

	// TODO: investigate, could be a compiler bug
	return std::move(derivedPredicate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::AtomicFormula normalizeTopLevel(ast::AtomicFormula &atomicFormula, detail::NormalizationContext &)
{
	return normalize(std::move(atomicFormula));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Normalize top-level conjunctions
normalizedAST::AndPointer<normalizedAST::Literal> normalizeTopLevel(ast::AndPointer<ast::Precondition> &and_, detail::NormalizationContext &normalizationContext)
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
			return normalizeTopLevel(not_, normalizationContext);
		};

	const auto handleNested =
		[&](auto &nested) -> normalizedAST::Literal
		{
			return normalizeNested(nested, normalizationContext);
		};

	for (auto &argument : and_->arguments)
	{
		auto normalizedArgument = argument.match(handleAtomicFormula, handleNot, handleNested);

		arguments.emplace_back(std::move(normalizedArgument));
	}

	return std::make_unique<normalizedAST::And<normalizedAST::Literal>>(std::move(arguments));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeTopLevel(ast::ExistsPointer<ast::Precondition> &exists, detail::NormalizationContext &normalizationContext)
{
	return normalizeNested(exists, normalizationContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeTopLevel(ast::ForAllPointer<ast::Precondition> &, detail::NormalizationContext &)
{
	throw std::logic_error("precondition not in normal form (forall), please report to the bug tracker");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalizeTopLevel(ast::ImplyPointer<ast::Precondition> &, detail::NormalizationContext &)
{
	throw std::logic_error("precondition not in normal form (imply), please report to the bug tracker");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Normalize top-level negations
normalizedAST::NotPointer<normalizedAST::AtomicFormula> normalizeTopLevel(ast::NotPointer<ast::Precondition> &not_, detail::NormalizationContext &normalizationContext)
{
	// “not” expressions may be nested one time to form simple literals
	if (not_->argument.is<ast::AtomicFormula>())
		return std::make_unique<normalizedAST::Not<normalizedAST::AtomicFormula>>(normalize(std::move(not_->argument.get<ast::AtomicFormula>())));

	auto normalizedArgument = not_->argument.match(
		[&](auto &nested) -> normalizedAST::AtomicFormula
		{
			auto normalizedLiteral = normalizeNested(nested, normalizationContext);

			// Multiple negations should be eliminated at this point
			if (normalizedLiteral.template is<normalizedAST::NotPointer<normalizedAST::AtomicFormula>>())
				throw std::logic_error("precondition not in normal form (multiple negation), please report to the bug tracker");

			return std::move(normalizedLiteral.template get<normalizedAST::AtomicFormula>());
		});

	return std::make_unique<normalizedAST::Not<normalizedAST::AtomicFormula>>(std::move(normalizedArgument));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: refactor to avoid code duplication
normalizedAST::OrPointer<normalizedAST::Literal> normalizeTopLevel(ast::OrPointer<ast::Precondition> &or_, detail::NormalizationContext &normalizationContext)
{
	normalizedAST::Or<normalizedAST::Literal>::Arguments arguments;

	arguments.reserve(or_->arguments.size());

	const auto handleAtomicFormula =
		[&](ast::AtomicFormula &atomicFormula) -> normalizedAST::Literal
		{
			return normalize(std::move(atomicFormula));
		};

	const auto handleNot =
		[&](ast::NotPointer<ast::Precondition> &not_) -> normalizedAST::Literal
		{
			return normalizeTopLevel(not_, normalizationContext);
		};

	const auto handleNested =
		[&](auto &nested) -> normalizedAST::Literal
		{
			return normalizeNested(nested, normalizationContext);
		};

	for (auto &argument : or_->arguments)
	{
		auto normalizedArgument = argument.match(handleAtomicFormula, handleNot, handleNested);

		arguments.emplace_back(std::move(normalizedArgument));
	}

	return std::make_unique<normalizedAST::Or<normalizedAST::Literal>>(std::move(arguments));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Precondition normalize(ast::Precondition &&precondition, detail::NormalizationContext &normalizationContext)
{
	// Bring precondition to normal form
	reduce(precondition);

	const auto handleAtomicFormula =
		[&](ast::AtomicFormula &atomicFormula) -> normalizedAST::Precondition
		{
			return normalizeTopLevel(atomicFormula, normalizationContext);
		};

	const auto handleAnd =
		[&](ast::AndPointer<ast::Precondition> &and_) -> normalizedAST::Precondition
		{
			return normalizeTopLevel(and_, normalizationContext);
		};

	const auto handleExists =
		[&](ast::ExistsPointer<ast::Precondition> &exists) -> normalizedAST::Precondition
		{
			return normalizeTopLevel(exists, normalizationContext);
		};

	const auto handleForAll =
		[&](ast::ForAllPointer<ast::Precondition> &forAll) -> normalizedAST::Precondition
		{
			return normalizeTopLevel(forAll, normalizationContext);
		};

	const auto handleImply =
		[&](ast::ImplyPointer<ast::Precondition> &imply) -> normalizedAST::Precondition
		{
			return normalizeTopLevel(imply, normalizationContext);
		};

	const auto handleNot =
		[&](ast::NotPointer<ast::Precondition> &not_) -> normalizedAST::Precondition
		{
			return normalizeTopLevel(not_, normalizationContext);
		};

	const auto handleOr =
		[&](ast::OrPointer<ast::Precondition> &or_) -> normalizedAST::Precondition
		{
			return normalizeNested(or_, normalizationContext);
		};

	return precondition.match(handleAtomicFormula, handleAnd, handleExists, handleForAll, handleImply, handleNot, handleOr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
