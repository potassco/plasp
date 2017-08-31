#include <catch.hpp>

#include <experimental/filesystem>

#include <pddl/AST.h>
#include <pddl/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[signature matching] Predicate signatures are matched correctly", "[signature matching]")
{
	std::stringstream input;
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);
	const auto domainFile = fs::path("data") / "test-cases" / "skeleton.pddl";
	context.tokenizer.read(domainFile);

	SECTION("directly matching primitive types")
	{
		input
			<< "(:action test :parameters (?x - a1 ?y - a2 ?z - a3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("derived primitive types (direct children)")
	{
		input
			<< "(:action test :parameters (?x - b1 ?y - b2 ?z - b3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("derived primitive types (indirect children)")
	{
		input
			<< "(:action test :parameters (?x - c1 ?y - c2 ?z - c3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("incompatible primitive types")
	{
		input
			<< "(:action test :parameters (?x - a3 ?y - a2 ?z - a3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	SECTION("unrelated primitive types")
	{
		input
			<< "(:action test :parameters (?x - b1 ?y - b2 ?z - b1) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	SECTION("incompatible parent types (1)")
	{
		input
			<< "(:action test :parameters (?x - object ?y - b2 ?z - c3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	SECTION("incompatible parent types (2)")
	{
		input
			<< "(:action test :parameters (?x - a1 ?y - a2 ?z - a3) :precondition (p2 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	SECTION("matching types with multiple inheritance (1)")
	{
		input
			<< "(:action test :parameters (?x - b1 ?y - b2 ?z - cx) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("matching types with multiple inheritance (2)")
	{
		input
			<< "(:action test :parameters (?x - bx ?y - b2 ?z - c3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("“either” type not matching primitive type")
	{
		input
			<< "(:action test :parameters (?x - either (a1 a2) ?y - b2 ?z - c3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	SECTION("1-ary “either” type matching primitive type")
	{
		input
			<< "(:action test :parameters (?x - (either a1) ?y - a2 ?z - a3) :precondition (p1 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("“either” type directly matching another “either” type")
	{
		input
			<< "(:action test :parameters (?x - (either a1 a2) ?y ?z - (either b1 b3)) :precondition (p3 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("“either” type matching another “either” type via inheritance")
	{
		input
			<< "(:action test :parameters (?x - (either b1 a2) ?y ?z - (either c1 b3)) :precondition (p3 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("“either” type incompatible with another “either” type")
	{
		input
			<< "(:action test :parameters (?x - (either b1 a2) ?y ?z - (either c2 b3)) :precondition (p3 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	SECTION("“either” type compatible with another “either” type via multiple inheritance")
	{
		input
			<< "(:action test :parameters (?x - (either cx c2) ?y ?z - (either cx c3)) :precondition (p3 ?x ?y ?z)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("constants compatible with “either” type via multiple inheritance")
	{
		input
			<< "(:action test :precondition (p3 cbx cb1 cb3)))";
		context.tokenizer.read("input", input);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}
}
