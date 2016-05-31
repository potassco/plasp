#ifndef __PLASP__PDDL__VARIABLE_H
#define __PLASP__PDDL__VARIABLE_H

#include <vector>

#include <plasp/pddl/Type.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context;

class Variable;
using Variables = std::vector<Variable>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Variable
{
	public:
		static Variable parse(utils::Parser &parser, Context &context);

	public:
		const std::string &name() const;
		const Type &type() const;

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setType(const Type &type);

	private:
		Variable(std::string name);

		bool m_isDirty;

		std::string m_name;

		const Type *m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
