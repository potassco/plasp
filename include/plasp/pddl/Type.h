#ifndef __PLASP__PDDL__TYPE_H
#define __PLASP__PDDL__TYPE_H

#include <functional>
#include <string>
#include <unordered_map>
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

class Context;

class Type;
using TypeHashMap = std::unordered_map<std::string, Type>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Type
{
	public:
		static void parsePDDL(utils::Parser &parser, Context &context);

	public:
		const std::string &name() const;
		const std::vector<const Type *> &parentTypes() const;

	private:
		Type(std::string name);

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void addParentType(const Type &parentType);

		bool m_isDirty;

		std::string m_name;

		std::vector<const Type *> m_parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
