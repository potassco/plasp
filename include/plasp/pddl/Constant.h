#ifndef __PLASP__PDDL__CONSTANT_H
#define __PLASP__PDDL__CONSTANT_H

#include <string>
#include <vector>

#include <plasp/pddl/Type.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Constant
{
	public:
		static Constant &parse(utils::Parser &parser, Context &context);
		static Constant &parseDeclaration(utils::Parser &parser, Context &context);

	public:
		const std::string &name() const;
		const PrimitiveType *type() const;

		bool isDeclared() const;

	private:
		Constant(std::string name);

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setDeclared();

		void setType(const PrimitiveType *parentType);

		bool m_isDirty;
		bool m_isDeclared;

		std::string m_name;

		const PrimitiveType *m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
