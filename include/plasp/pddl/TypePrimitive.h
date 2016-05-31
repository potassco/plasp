#ifndef __PLASP__PDDL__TYPE_PRIMITIVE_H
#define __PLASP__PDDL__TYPE_PRIMITIVE_H

#include <string>
#include <vector>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TypePrimitive
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context;

////////////////////////////////////////////////////////////////////////////////////////////////////

class TypePrimitive
{
	public:
		static TypePrimitive &parse(utils::Parser &parser, Context &context);
		static TypePrimitive &parseDeclaration(utils::Parser &parser, Context &context);

	public:
		const std::string &name() const;
		const std::vector<const TypePrimitive *> &parentTypes() const;

		bool isDeclared() const;

	private:
		TypePrimitive(std::string name);

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setDeclared();

		void addParentType(const TypePrimitive &parentType);

		bool m_isDirty;
		bool m_isDeclared;

		std::string m_name;

		std::vector<const TypePrimitive *> m_parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
