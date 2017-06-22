#ifndef __PDDL_PARSE__AST_OUTPUT_H
#define __PDDL_PARSE__AST_OUTPUT_H

#include <colorlog/ColorStream.h>
#include <colorlog/Formatting.h>

#include <pddlparse/AST.h>
#include <pddlparse/detail/parsing/Requirement.h>

namespace pddl
{
namespace ast
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Output
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrintContext
{
	size_t indentationLevel{0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Identifier
{
	Identifier(const char *content)
	:	content{content}
	{
	}

	Identifier(const std::string &content)
	:	content{content.c_str()}
	{
	}

	const char *content;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Identifier &identifier)
{
	return (stream
		<< colorlog::Format({colorlog::Color::Green, colorlog::FontWeight::Normal})
		<< identifier.content << colorlog::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Type
{
	Type(const char *name)
	:	name{name}
	{
	}

	Type(const std::string &name)
	:	name{name.c_str()}
	{
	}

	const char *name;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Type &type)
{
	return (stream
		<< colorlog::Format({colorlog::Color::Red, colorlog::FontWeight::Normal})
		<< type.name << colorlog::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Constant
{
	Constant(const char *name)
	:	name{name}
	{
	}

	Constant(const std::string &name)
	:	name{name.c_str()}
	{
	}

	const char *name;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Constant &constant)
{
	return (stream
		<< colorlog::Format({colorlog::Color::Yellow, colorlog::FontWeight::Normal})
		<< constant.name << colorlog::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////////////////////////

colorlog::ColorStream &printIndentedNewline(colorlog::ColorStream &stream, detail::PrintContext &printContext)
{
	stream << std::endl;

	for (size_t i = 0; i < printContext.indentationLevel; i++)
		stream << "\t";

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Constant &constant, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const ConstantDeclaration &constantDeclaration, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveType &primitiveType, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveTypeDeclaration &primitiveTypeDeclaration, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variable &variable, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const VariableDeclaration &variableDeclaration, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Predicate &predicate, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PredicateDeclaration &predicateDeclaration, detail::PrintContext &printContext);

template<class Derived, class ArgumentLeft, class ArgumentRight = ArgumentLeft>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Binary<Derived, ArgumentLeft, ArgumentRight> &binary, detail::PrintContext &printContext);
template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const NAry<Derived, Argument> &nAry, detail::PrintContext &printContext);
template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Quantified<Derived, Argument> &quantified, detail::PrintContext &printContext);

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const At<Argument> &at, detail::PrintContext &printContext);
template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Not<Argument> &not_, detail::PrintContext &printContext);

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Action &action, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Domain &domain, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const InitialState &initialState, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Problem &problem, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Requirement &requirement, detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Description &description, detail::PrintContext &printContext);

template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::unique_ptr<ValueType> &variant, detail::PrintContext &printContext);
template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::vector<ValueType> &variant, detail::PrintContext &printContext);
template<class Variant>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variant &variant, detail::PrintContext &printContext);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Constant &constant, detail::PrintContext &)
{
	return stream << detail::Constant(constant.declaration->name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const ConstantDeclaration &constantDeclaration, detail::PrintContext &printContext)
{
	stream << detail::Constant(constantDeclaration.name);

	if (constantDeclaration.type)
	{
		stream << " - ";
		print(stream, constantDeclaration.type.value(), printContext);
	}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveType &primitiveType, detail::PrintContext &)
{
	return stream << detail::Type(primitiveType.declaration->name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PrimitiveTypeDeclaration &primitiveTypeDeclaration, detail::PrintContext &printContext)
{
	if (primitiveTypeDeclaration.parentTypes.empty())
		return (stream << detail::Type(primitiveTypeDeclaration.name));

	if (!primitiveTypeDeclaration.parentTypes.empty())
		for (const auto &parentType : primitiveTypeDeclaration.parentTypes)
		{
			if (&parentType != &primitiveTypeDeclaration.parentTypes.front())
				printIndentedNewline(stream, printContext);

			stream << detail::Type(primitiveTypeDeclaration.name) << " - " << detail::Type(parentType->declaration->name);
		}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Unsupported &unsupported, detail::PrintContext &)
{
	const auto unsupportedName = "<unsupported " + unsupported.type + ">";

	return (stream << colorlog::Reserved(unsupportedName.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variable &variable, detail::PrintContext &)
{
	const auto variableName = "?" + variable.declaration->name;

	return (stream << colorlog::Variable(variableName.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const VariableDeclaration &variableDeclaration, detail::PrintContext &printContext)
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

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Predicate &predicate, detail::PrintContext &printContext)
{
	stream << "(" << detail::Identifier(predicate.declaration->name) << " ";

	for (const auto &argument : predicate.arguments)
	{
		if (&argument != &predicate.arguments.front())
			stream << " ";

		print(stream, argument, printContext);
	}

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const PredicateDeclaration &predicateDeclaration, detail::PrintContext &printContext)
{
	stream << "(" << detail::Identifier(predicateDeclaration.name);

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
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Binary<Derived, ArgumentLeft, ArgumentRight> &binary, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(Derived::Identifier);

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	print(stream, binary.argumentLeft, printContext);
	printIndentedNewline(stream, printContext);
	print(stream, binary.argumentRight, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const NAry<Derived, Argument> &nAry, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(Derived::Identifier);

	printContext.indentationLevel++;

	for (const auto &argument : nAry.arguments)
	{
		printIndentedNewline(stream, printContext);
		print(stream, argument, printContext);
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Quantified<Derived, Argument> &quantified, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(Derived::Identifier);

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	print(stream, quantified.parameters, printContext);
	printIndentedNewline(stream, printContext);
	print(stream, quantified.argument, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const And<Argument> &and_, detail::PrintContext &printContext)
{
	return print(stream, static_cast<const NAry<And<Argument>, Argument> &>(and_), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const At<Argument> &at, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("at");

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	stream << colorlog::Number<decltype(at.timePoint)>(at.timePoint);
	printIndentedNewline(stream, printContext);
	print(stream, at.argument, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Either<Argument> &either, detail::PrintContext &printContext)
{
	return print(stream, static_cast<const NAry<Either<Argument>, Argument> &>(either), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Exists<Argument> &exists, detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Quantified<Exists<Argument>, Argument> &>(exists), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const ForAll<Argument> &forAll, detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Quantified<ForAll<Argument>, Argument> &>(forAll), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Imply<Argument> &imply, detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Binary<Imply<Argument>, Argument> &>(imply), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Not<Argument> &not_, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("not");

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	print(stream, not_.argument, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Or<Argument> &or_, detail::PrintContext &printContext)
{
	return print(stream, static_cast<const NAry<Or<Argument>, Argument> &>(or_), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ArgumentLeft, class ArgumentRight>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const When<ArgumentLeft, ArgumentRight> &when, detail::PrintContext &printContext)
{
	return print(stream, static_cast<const Binary<When<ArgumentLeft, ArgumentRight>, ArgumentLeft, ArgumentRight> &>(when), printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PDDL Structure
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Action &action, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(":action") << " " << detail::Identifier(action.name);

	printContext.indentationLevel++;

	if (!action.parameters.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":parameters");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, action.parameters, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (action.precondition)
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":precondition");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, action.precondition.value(), printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (action.effect)
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":effect");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, action.effect.value(), printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Domain &domain, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("define") << " (" << colorlog::Keyword("domain") << " " << detail::Identifier(domain.name) << ")";

	printContext.indentationLevel++;

	if (!domain.requirements.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":requirements");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, domain.requirements, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.types.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":types");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, domain.types, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.constants.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":constants");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, domain.constants, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.predicates.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":predicates");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, domain.predicates, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.actions.empty())
	{
		printIndentedNewline(stream, printContext);
		print(stream, domain.actions, printContext);
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const InitialState &initialState, detail::PrintContext &printContext)
{
	assert(!initialState.facts.empty());

	stream << "(" << colorlog::Keyword(":init");

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	print(stream, initialState.facts, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Problem &problem, detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("define") << " (" << colorlog::Keyword("problem") << " " << detail::Identifier(problem.name) << ")";

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	stream << "(" << colorlog::Keyword(":domain") << " " << detail::Identifier(problem.domain->name) << ")";

	if (!problem.requirements.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":requirements");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, problem.requirements, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!problem.objects.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":objects");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, problem.objects, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!problem.initialState.facts.empty())
	{
		printIndentedNewline(stream, printContext);
		print(stream, problem.initialState, printContext);
	}

	if (problem.goal)
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":goal");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, problem.goal.value(), printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Requirement &requirement, detail::PrintContext &)
{
	auto requirementName = std::string(":") + pddl::detail::toString(requirement);

	return (stream << detail::Identifier(requirementName.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Description &description, detail::PrintContext &printContext)
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
	detail::PrintContext printContext;

	return print(stream, description, printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Variants
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::unique_ptr<ValueType> &uniquePointer, detail::PrintContext &printContext)
{
	assert(uniquePointer);
	return print(stream, *uniquePointer, printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ValueType>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const std::vector<ValueType> &vector, detail::PrintContext &printContext)
{
	for (const auto &element : vector)
	{
		if (&element != &vector.front())
			printIndentedNewline(stream, printContext);

		print(stream, element, printContext);
	}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Variant>
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Variant &variant, detail::PrintContext &printContext)
{
	variant.match([&](const auto &x){return print(stream, x, printContext);});

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
