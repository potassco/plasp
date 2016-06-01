#ifndef __PLASP__PDDL__PRIMITIVE_TYPE_H
#define __PLASP__PDDL__PRIMITIVE_TYPE_H

#include <string>
#include <vector>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrimitiveType
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context;

////////////////////////////////////////////////////////////////////////////////////////////////////

class PrimitiveType
{
	public:
		static PrimitiveType &parse(utils::Parser &parser, Context &context);
		static PrimitiveType &parseDeclaration(utils::Parser &parser, Context &context);

	public:
		const std::string &name() const;
		const std::vector<const PrimitiveType *> &parentTypes() const;

		bool isDeclared() const;

	private:
		PrimitiveType(std::string name);

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setDeclared();

		void addParentType(const PrimitiveType &parentType);

		bool m_isDirty;
		bool m_isDeclared;

		std::string m_name;

		std::vector<const PrimitiveType *> m_parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
