#ifndef __PLASP__PDDL__CONTEXT_H
#define __PLASP__PDDL__CONTEXT_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <plasp/pddl/Action.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/PrimitiveType.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Context
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context
{
	public:
		Context(utils::Parser &parser)
		:	parser(parser)
		{
		}

		utils::Parser &parser;

		expressions::PrimitiveTypes primitiveTypes;
		//std::unordered_map<std::string, expressions::PrimitiveType *> primitiveTypesHashMap;

		expressions::Constants constants;
		//std::unordered_map<std::string, expressions::Constant *> constantsHashMap;

		expressions::PredicateDeclarations predicateDeclarations;
		//std::unordered_map<expressions::PredicateHashMapKey, expressions::Predicate *> predicatesHashMap;

		std::vector<std::unique_ptr<Action>> actions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
