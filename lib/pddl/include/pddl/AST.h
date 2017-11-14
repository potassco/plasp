#ifndef __PDDL__AST_H
#define __PDDL__AST_H

#include <limits>
#include <experimental/optional>
#include <set>
#include <type_traits>
#include <vector>

#include <pddl/ASTForward.h>

namespace pddl
{
namespace ast
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AST
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Constant
{
	explicit Constant(ConstantDeclaration *declaration)
	:	declaration{declaration}
	{
	}

	Constant(const Constant &other) = delete;
	Constant &operator=(const Constant &&other) = delete;
	Constant(Constant &&other) = default;
	Constant &operator=(Constant &&other) = default;

	ConstantDeclaration *declaration;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct ConstantDeclaration
{
	explicit ConstantDeclaration(std::string &&name, std::experimental::optional<Type> &&type = std::experimental::nullopt)
	:	name{std::move(name)},
		type{std::move(type)}
	{
	}

	ConstantDeclaration(const ConstantDeclaration &other) = delete;
	ConstantDeclaration &operator=(const ConstantDeclaration &&other) = delete;
	ConstantDeclaration(ConstantDeclaration &&other) = delete;
	ConstantDeclaration &operator=(ConstantDeclaration &&other) = delete;

	std::string name;
	// TODO: check whether “either” types should actually be allowed at all
	std::experimental::optional<Type> type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrimitiveType
{
	explicit PrimitiveType(PrimitiveTypeDeclaration *declaration)
	:	declaration{declaration}
	{
	}

	PrimitiveType(const PrimitiveType &other) = delete;
	PrimitiveType &operator=(const PrimitiveType &&other) = delete;
	PrimitiveType(PrimitiveType &&other) = default;
	PrimitiveType &operator=(PrimitiveType &&other) = default;

	PrimitiveTypeDeclaration *declaration;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrimitiveTypeDeclaration
{
	explicit PrimitiveTypeDeclaration(std::string &&name)
	:	name{std::move(name)}
	{
	}

	PrimitiveTypeDeclaration(const PrimitiveTypeDeclaration &other) = delete;
	PrimitiveTypeDeclaration &operator=(const PrimitiveTypeDeclaration &&other) = delete;
	PrimitiveTypeDeclaration(PrimitiveTypeDeclaration &&other) = delete;
	PrimitiveTypeDeclaration &operator=(PrimitiveTypeDeclaration &&other) = delete;

	std::string name;
	std::vector<PrimitiveTypePointer> parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Variable
{
	explicit Variable(VariableDeclaration *declaration)
	:	declaration{declaration}
	{
	}

	Variable(const Variable &other) = delete;
	Variable &operator=(const Variable &&other) = delete;
	Variable(Variable &&other) = default;
	Variable &operator=(Variable &&other) = default;

	VariableDeclaration *declaration;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct VariableDeclaration
{
	explicit VariableDeclaration(std::string &&name, std::experimental::optional<Type> type = std::experimental::nullopt)
	:	name{std::move(name)},
		type{std::move(type)}
	{
	}

	VariableDeclaration(const VariableDeclaration &other) = delete;
	VariableDeclaration &operator=(const VariableDeclaration &&other) = delete;
	VariableDeclaration(VariableDeclaration &&other) = delete;
	VariableDeclaration &operator=(VariableDeclaration &&other) = delete;

	std::string name;
	std::experimental::optional<Type> type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Compounds
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Predicate
{
	using Arguments = Terms;

	explicit Predicate(Arguments &&arguments, PredicateDeclaration *declaration)
	:	arguments{std::move(arguments)},
		declaration{declaration}
	{
	}

	Predicate(const Predicate &other) = delete;
	Predicate &operator=(const Predicate &&other) = delete;
	Predicate(Predicate &&other) = default;
	Predicate &operator=(Predicate &&other) = default;

	Arguments arguments;
	PredicateDeclaration *declaration;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct PredicateDeclaration
{
	explicit PredicateDeclaration(std::string &&name, VariableDeclarations &&parameters)
	:	name{std::move(name)},
		parameters{std::move(parameters)}
	{
	}

	PredicateDeclaration(const PredicateDeclaration &other) = delete;
	PredicateDeclaration &operator=(const PredicateDeclaration &&other) = delete;
	PredicateDeclaration(PredicateDeclaration &&other) = default;
	PredicateDeclaration &operator=(PredicateDeclaration &&other) = default;

	std::string name;
	VariableDeclarations parameters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions: Base Classes
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class ArgumentLeft, class ArgumentRight = ArgumentLeft>
struct Binary
{
	explicit Binary(ArgumentLeft &&argumentLeft, ArgumentRight &&argumentRight)
	:	argumentLeft{std::move(argumentLeft)},
		argumentRight{std::move(argumentRight)}
	{
	}

	Binary(const Binary &other) = delete;
	Binary &operator=(const Binary &&other) = delete;
	Binary(Binary &&other) = default;
	Binary &operator=(Binary &&other) = default;

	ArgumentLeft argumentLeft;
	ArgumentRight argumentRight;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class Argument>
struct NAry
{
	using Arguments = std::vector<Argument>;

	explicit NAry(Arguments &&arguments) noexcept
	:	arguments{std::move(arguments)}
	{
	}

	NAry(const NAry &other) = delete;
	NAry &operator=(const NAry &&other) = delete;
	NAry(NAry &&other) = default;
	NAry &operator=(NAry &&other) = default;

	Arguments arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class Argument>
struct Quantified
{
	using Parameters = VariableDeclarations;

	explicit Quantified(Parameters &&parameters, Argument &&argument)
	:	parameters{std::move(parameters)},
		argument{std::move(argument)}
	{
	}

	Quantified(const Quantified &other) = delete;
	Quantified &operator=(const Quantified &&other) = delete;
	Quantified(Quantified &&other) = default;
	Quantified &operator=(Quantified &&other) = default;

	Parameters parameters;
	Argument argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct And: public NAry<And<Argument>, Argument>
{
	using typename NAry<And<Argument>, Argument>::Arguments;
	static constexpr const auto Identifier = "and";

	explicit And(Arguments &&arguments) noexcept
	:	NAry<And<Argument>, Argument>(std::move(arguments))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: make binary expression
template<class Argument>
struct At
{
	static constexpr const auto TimePointStart = std::numeric_limits<std::size_t>::max();
	static constexpr const auto TimePointEnd = std::numeric_limits<std::size_t>::max() - 1;

	At(std::size_t timePoint, Argument &&argument)
	:	timePoint{timePoint},
		argument{std::move(argument)}
	{
	}

	At(const At &other) = delete;
	At &operator=(const At &&other) = delete;
	At(At &&other) = default;
	At &operator=(At &&other) = default;

	std::size_t timePoint;
	Argument argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct Either: public NAry<Either<Argument>, Argument>
{
	using typename NAry<Either<Argument>, Argument>::Arguments;
	static constexpr const auto Identifier = "either";

	explicit Either(Arguments &&arguments) noexcept
	:	NAry<Either<Argument>, Argument>(std::move(arguments))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ArgumentLeft, class ArgumentRight>
struct Equals: public Binary<Equals<ArgumentLeft, ArgumentRight>, ArgumentLeft, ArgumentRight>
{
	static constexpr const auto Identifier = "=";

	explicit Equals(ArgumentLeft &&argumentLeft, ArgumentRight &&argumentRight)
	:	Binary<Equals<ArgumentLeft, ArgumentRight>, ArgumentLeft, ArgumentRight>(std::move(argumentLeft), std::move(argumentRight))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct Exists: public Quantified<Exists<Argument>, Argument>
{
	static constexpr const auto Identifier = "exists";

	explicit Exists(VariableDeclarations &&parameters, Argument &&argument)
	:	Quantified<Exists<Argument>, Argument>(std::move(parameters), std::move(argument))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct ForAll: public Quantified<ForAll<Argument>, Argument>
{
	static constexpr const auto Identifier = "forall";

	explicit ForAll(VariableDeclarations &&parameters, Argument &&argument)
	:	Quantified<ForAll<Argument>, Argument>(std::move(parameters), std::move(argument))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct Imply: public Binary<Imply<Argument>, Argument>
{
	static constexpr const auto Identifier = "imply";

	// TODO: make noexcept consistent
	explicit Imply(Argument &&argumentLeft, Argument &&argumentRight)
	:	Binary<Imply<Argument>, Argument>(std::move(argumentLeft), std::move(argumentRight))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct Not
{
	explicit Not(Argument &&argument)
	:	argument{std::move(argument)}
	{
	}

	Not(const Not &other) = delete;
	Not &operator=(const Not &&other) = delete;
	Not(Not &&other) = default;
	Not &operator=(Not &&other) = default;

	Argument argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct Or: public NAry<Or<Argument>, Argument>
{
	using typename NAry<Or<Argument>, Argument>::Arguments;
	static constexpr const auto Identifier = "or";

	explicit Or(Arguments &&arguments) noexcept
	:	NAry<Or<Argument>, Argument>(std::move(arguments))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ArgumentLeft, class ArgumentRight>
struct When: public Binary<When<ArgumentLeft, ArgumentRight>, ArgumentLeft, ArgumentRight>
{
	static constexpr const auto Identifier = "when";

	explicit When(ArgumentLeft &&argumentLeft, ArgumentRight &&argumentRight)
	:	Binary<When<ArgumentLeft, ArgumentRight>, ArgumentLeft, ArgumentRight>(std::move(argumentLeft), std::move(argumentRight))
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// PDDL Structure
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Action
{
	Action() = default;

	Action(const Action &other) = delete;
	Action &operator=(const Action &&other) = delete;
	Action(Action &&other) = default;
	Action &operator=(Action &&other) = default;

	std::string name;

	VariableDeclarations parameters;
	std::experimental::optional<Precondition> precondition;
	std::experimental::optional<Effect> effect;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Domain
{
	Domain() = default;

	Domain(const Domain &other) = delete;
	Domain &operator=(const Domain &&other) = delete;
	Domain(Domain &&other) = delete;
	Domain &operator=(Domain &&other) = delete;

	std::string name;
	Requirements requirements;
	PrimitiveTypeDeclarations types;
	ConstantDeclarations constants;
	PredicateDeclarations predicates;
	Actions actions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct InitialState
{
	InitialState() = default;

	InitialState(const InitialState &other) = delete;
	InitialState &operator=(const InitialState &&other) = delete;
	InitialState(InitialState &&other) = default;
	InitialState &operator=(InitialState &&other) = default;

	Facts facts;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Problem
{
	Problem() = default;

	Problem(Domain *domain)
	:	domain{domain}
	{
	}

	Problem(const Problem &other) = delete;
	Problem &operator=(const Problem &&other) = delete;
	Problem(Problem &&other) = default;
	Problem &operator=(Problem &&other) = default;

	Domain *domain;
	std::string name;
	Requirements requirements;
	ConstantDeclarations objects;
	InitialState initialState;
	std::experimental::optional<Goal> goal;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: think about ignoring requirement statements entirely and computing them instead
enum class Requirement
{
	STRIPS,
	Typing,
	NegativePreconditions,
	DisjunctivePreconditions,
	Equality,
	ExistentialPreconditions,
	UniversalPreconditions,
	QuantifiedPreconditions,
	ConditionalEffects,
	Fluents,
	NumericFluents,
	ObjectFluents,
	ADL,
	DurativeActions,
	DurationInequalities,
	ContinuousEffects,
	DerivedPredicates,
	TimedInitialLiterals,
	Preferences,
	Constraints,
	ActionCosts,
	GoalUtilities
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Description
{
	Description() = default;

	Description(const Description &other) = delete;
	Description &operator=(const Description &&other) = delete;
	Description(Description &&other) = default;
	Description &operator=(Description &&other) = default;

	DomainPointer domain;
	std::experimental::optional<ProblemPointer> problem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
