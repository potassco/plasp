#ifndef __PLASP__PDDL__TYPE_H
#define __PLASP__PDDL__TYPE_H

#include <functional>
#include <string>
#include <vector>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Type
{
	public:
		Type(std::string name);

	public:
		void setDirty(bool isDirty = true);
		bool isDirty() const;

		const std::string &name() const;

		void addParentType(const Type &parentType);
		const std::vector<const Type *> &parentTypes() const;

	private:
		bool m_isDirty;

		std::string m_name;

		std::vector<const Type *> m_parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
