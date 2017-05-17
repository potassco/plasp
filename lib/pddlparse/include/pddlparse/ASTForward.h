#ifndef __PDDL_PARSE__AST_FORWARD_H
#define __PDDL_PARSE__AST_FORWARD_H

#include <iosfwd>
#include <memory>
#include <experimental/optional>
#include <set>
#include <vector>

#include <pddlparse/Variant.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AST Forward Declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ast
{

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Constant;
struct ConstantDeclaration;
using ConstantDeclarations = std::vector<ConstantDeclaration>;
struct Dummy;
struct PrimitiveType;
using PrimitiveTypes = std::vector<PrimitiveType>;
struct PrimitiveTypeDeclaration;
using PrimitiveTypeDeclarations = std::vector<PrimitiveTypeDeclaration>;
struct Unsupported;
struct Variable;
using Variables = std::vector<Variable>;
struct VariableDeclaration;
using VariableDeclarations = std::vector<VariableDeclaration>;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Compounds
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Predicate;
using Predicates = std::vector<Predicate>;
struct PredicateDeclaration;
using PredicateDeclarations = std::vector<PredicateDeclaration>;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct And;
template<class Argument>
struct At;
template<class Argument>
struct Either;
template<class Argument>
struct Exists;
template<class Argument>
struct ForAll;
template<class Argument>
struct Imply;
template<class Argument>
struct Not;
template<class Argument>
struct Or;
template<class ArgumentLeft, class ArgumentRight>
struct When;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PDDL Structure
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Description;
struct Domain;
struct Problem;
enum class Requirement;
using Requirements = std::vector<Requirement>;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Variants
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using TermT = Variant<
	Constant,
	Variable>;
}

class Term : public detail::TermT
{
	using detail::TermT::TermT;
};

using Terms = std::vector<Term>;

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using AtomicFormulaT = Variant<
	Predicate,
	Unsupported>;
}

class AtomicFormula : public detail::AtomicFormulaT
{
	using detail::AtomicFormulaT::AtomicFormulaT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class Precondition;

namespace detail
{
using PreconditionT = Variant<
	AtomicFormula,
	And<Precondition>,
	Exists<Precondition>,
	ForAll<Precondition>,
	Imply<Precondition>,
	Not<Precondition>,
	Or<Precondition>,
	Unsupported>;
}

class Precondition : public detail::PreconditionT
{
	using detail::PreconditionT::PreconditionT;
};

using Preconditions = std::vector<Precondition>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Effect;

namespace detail
{
using EffectT = Variant<
	AtomicFormula,
	And<Effect>,
	ForAll<Effect>,
	Not<Effect>,
	When<Precondition, Effect>,
	Unsupported>;
}

class Effect : public detail::EffectT
{
	using detail::EffectT::EffectT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using TypeT = Variant<
	Either<PrimitiveType>,
	PrimitiveType>;
}

class Type : public detail::TypeT
{
	using detail::TypeT::TypeT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using LiteralT = Variant<
	AtomicFormula,
	Not<AtomicFormula>>;
}

class Literal : public detail::LiteralT
{
	using detail::LiteralT::LiteralT;
};

using Literals = std::vector<Literal>;

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
using FactT = Variant<
	AtomicFormula,
	At<Literal>>;
}

class Fact : public detail::FactT
{
	using detail::FactT::FactT;
};

using Facts = std::vector<Fact>;

////////////////////////////////////////////////////////////////////////////////////////////////////

using Goal = Precondition;

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
