#ifndef __PDDL__AST_OUTPUT_H
#define __PDDL__AST_OUTPUT_H

#include <colorlog/ColorStream.h>
#include <colorlog/Formatting.h>

#include <pddl/AST.h>
#include <pddl/detail/OutputUtils.h>
#include <pddl/detail/parsing/Requirement.h>

namespace pddl
{
namespace ast
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Output
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Constant &constant, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const ConstantDeclaration &constantDeclaration, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveType &primitiveType, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveTypeDeclaration &primitiveTypeDeclaration, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variable &variable, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const VariableDeclaration &variableDeclaration, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Predicate &predicate, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PredicateDeclaration &predicateDeclaration, pddl::detail::PrintContext &printContext);

template<class Derived, class ArgumentLeft, class ArgumentRight = ArgumentLeft>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Binary<Derived, ArgumentLeft, ArgumentRight> &binary, pddl::detail::PrintContext &printContext);
template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const NAry<Derived, Argument> &nAry, pddl::detail::PrintContext &printContext);
template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Quantified<Derived, Argument> &quantified, pddl::detail::PrintContext &printContext);

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const At<Argument> &at, pddl::detail::PrintContext &printContext);
template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Not<Argument> &not_, pddl::detail::PrintContext &printContext);

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Action &action, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Domain &domain, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const InitialState &initialState, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Problem &problem, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Requirement &requirement, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Description &description, pddl::detail::PrintContext &printContext);

template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::unique_ptr<ValueType> &variant, pddl::detail::PrintContext &printContext);
template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::vector<ValueType> &variant, pddl::detail::PrintContext &printContext);
template<class Variant>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variant &variant, pddl::detail::PrintContext &printContext);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Constant &constant, pddl::detail::PrintContext &)
{
	return stream << pddl::detail::Constant(constant.declaration->name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const ConstantDeclaration &constantDeclaration, pddl::detail::PrintContext &printContext)
{
	stream << pddl::detail::Constant(constantDeclaration.name);

	if (constantDeclaration.type)
	{
		stream << " - ";
		print(stream, constantDeclaration.type.value(), printContext);
	}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveType &primitiveType, pddl::detail::PrintContext &)
{
	return stream << pddl::detail::Type(primitiveType.declaration->name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveTypeDeclaration &primitiveTypeDeclaration, pddl::detail::PrintContext &printContext)
{
	if (primitiveTypeDeclaration.parentTypes.empty())
		return (stream << pddl::detail::Type(primitiveTypeDeclaration.name));

	if (!primitiveTypeDeclaration.parentTypes.empty())
		for (const auto &parentType : primitiveTypeDeclaration.parentTypes)
		{
			if (&parentType != &primitiveTypeDeclaration.parentTypes.front())
				pddl::detail::printIndentedNewline(stream, printContext);

			stream << pddl::detail::Type(primitiveTypeDeclaration.name) << " - " << pddl::detail::Type(parentType->declaration->name);
		}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variable &variable, pddl::detail::PrintContext &)
{
	const auto variableName = "?" + variable.declaration->name;

	return (stream << colorlog::Variable(variableName.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const VariableDeclaration &variableDeclaration, pddl::detail::PrintContext &printContext)
{
	const auto variableName = "?" + variableDeclaration.name;

	stream << colorlog::Variable(variableName.c_str());

	if (variableDeclaration.type)
	{
		stream << " - ";
		print(stream, *variableDeclaration.type, printContext);
	}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Compounds
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Predicate &predicate, pddl::detail::PrintContext &printContext)
{
	stream << "(" << pddl::detail::Identifier(predicate.declaration->name);

	for (const auto &argument : predicate.arguments)
	{
		stream << " ";
		print(stream, argument, printContext);
	}

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PredicateDeclaration &predicateDeclaration, pddl::detail::PrintContext &printContext)
{
	stream << "(" << pddl::detail::Identifier(predicateDeclaration.name);

	for (const auto &parameter : predicateDeclaration.parameters)
	{
		stream << " ";
		print(stream, *parameter, printContext);
	}

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions: Base Classes
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class ArgumentLeft, class ArgumentRight>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Binary<Derived, ArgumentLeft, ArgumentRight> &binary, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(Derived::Identifier);

	printContext.indentationLevel++;

	pddl::detail::printIndentedNewline(stream, printContext);
	print(stream, binary.argumentLeft, printContext);
	pddl::detail::printIndentedNewline(stream, printContext);
	print(stream, binary.argumentRight, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const NAry<Derived, Argument> &nAry, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(Derived::Identifier);

	printContext.indentationLevel++;

	for (const auto &argument : nAry.arguments)
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, argument, printContext);
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Quantified<Derived, Argument> &quantified, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(Derived::Identifier);

	printContext.indentationLevel++;

	pddl::detail::printIndentedNewline(stream, printContext);
	stream << "(";
	print(stream, quantified.parameters, printContext);
	stream << ")";
	pddl::detail::printIndentedNewline(stream, printContext);
	print(stream, quantified.argument, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const And<Argument> &and_, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const NAry<And<Argument>, Argument> &>(and_), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const At<Argument> &at, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("at");

	printContext.indentationLevel++;

	pddl::detail::printIndentedNewline(stream, printContext);
	stream << colorlog::Number<decltype(at.timePoint)>(at.timePoint);
	pddl::detail::printIndentedNewline(stream, printContext);
	print(stream, at.argument, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Either<Argument> &either, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const NAry<Either<Argument>, Argument> &>(either), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ArgumentLeft, class ArgumentRight>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Equals<ArgumentLeft, ArgumentRight> &equals, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Binary<Equals<ArgumentLeft, ArgumentRight>, ArgumentLeft, ArgumentRight> &>(equals), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Exists<Argument> &exists, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Quantified<Exists<Argument>, Argument> &>(exists), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const ForAll<Argument> &forAll, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Quantified<ForAll<Argument>, Argument> &>(forAll), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Imply<Argument> &imply, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Binary<Imply<Argument>, Argument> &>(imply), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Not<Argument> &not_, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("not") << " ";
	print(stream, not_.argument, printContext);
	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Or<Argument> &or_, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const NAry<Or<Argument>, Argument> &>(or_), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ArgumentLeft, class ArgumentRight>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const When<ArgumentLeft, ArgumentRight> &when, pddl::detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Binary<When<ArgumentLeft, ArgumentRight>, ArgumentLeft, ArgumentRight> &>(when), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PDDL Structure
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Action &action, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(":action") << " " << pddl::detail::Identifier(action.name);

	printContext.indentationLevel++;

	if (!action.parameters.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":parameters");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(";
		print(stream, action.parameters, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (action.precondition)
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":precondition");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, action.precondition.value(), printContext);

		printContext.indentationLevel--;
	}

	if (action.effect)
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":effect");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, action.effect.value(), printContext);

		printContext.indentationLevel--;
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Domain &domain, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("define") << " (" << colorlog::Keyword("domain") << " " << pddl::detail::Identifier(domain.name) << ")";

	printContext.indentationLevel++;

	if (!domain.requirements.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":requirements");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, domain.requirements, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.types.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":types");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, domain.types, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.constants.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":constants");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, domain.constants, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.predicates.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":predicates");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, domain.predicates, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.actions.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, domain.actions, printContext);
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const InitialState &initialState, pddl::detail::PrintContext &printContext)
{
	assert(!initialState.facts.empty());

	stream << "(" << colorlog::Keyword(":init");

	printContext.indentationLevel++;

	pddl::detail::printIndentedNewline(stream, printContext);
	print(stream, initialState.facts, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Problem &problem, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("define") << " (" << colorlog::Keyword("problem") << " " << pddl::detail::Identifier(problem.name) << ")";

	printContext.indentationLevel++;

	pddl::detail::printIndentedNewline(stream, printContext);
	stream << "(" << colorlog::Keyword(":domain") << " " << pddl::detail::Identifier(problem.domain->name) << ")";

	if (!problem.requirements.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":requirements");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, problem.requirements, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!problem.objects.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":objects");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, problem.objects, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!problem.initialState.facts.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, problem.initialState, printContext);
	}

	if (problem.goal)
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":goal");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, problem.goal.value(), printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Requirement &requirement, pddl::detail::PrintContext &)
{
	auto requirementName = std::string(":") + pddl::detail::toString(requirement);

	return (stream << pddl::detail::Identifier(requirementName.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Description &description, pddl::detail::PrintContext &printContext)
{
	print(stream, *description.domain, printContext);
	stream << std::endl;

	if (description.problem)
	{
		stream << std::endl;
		print(stream, *description.problem.value(), printContext);
		stream << std::endl;
	}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Description &description)
{
	pddl::detail::PrintContext printContext;

	return print(stream, description, printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Variants
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::unique_ptr<ValueType> &uniquePointer, pddl::detail::PrintContext &printContext)
{
	assert(uniquePointer);
	return print(stream, *uniquePointer, printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::vector<ValueType> &vector, pddl::detail::PrintContext &printContext)
{
	for (const auto &element : vector)
	{
		if (&element != &vector.front())
			pddl::detail::printIndentedNewline(stream, printContext);

		print(stream, element, printContext);
	}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Variant>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variant &variant, pddl::detail::PrintContext &printContext)
{
	variant.match([&](const auto &x){return print(stream, x, printContext);});

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
