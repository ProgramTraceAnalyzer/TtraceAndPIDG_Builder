#include "pch.h"
#include "CppUnitTest.h"
#include "../ConsoleApplication6/Memory.h"
#include "../ConsoleApplication6/ExprNode.h"
#include "../ConsoleApplication6/Action.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(TestMemory)
	{
	public:

		
		
		TEST_METHOD(TestScalarSubmemory)
		{
			ScalarMemory scalarMemory = { { {"x",5}, {"y",2} } };
			Memory memory = { scalarMemory };

			Memory expected_submemory = { { { {"x",5} } } };

			Memory actual_submemory = memory.get_submemorty({ "x" });

			Assert::IsTrue(expected_submemory == actual_submemory);
		}

		TEST_METHOD(TestArraySubmemory)
		{
			Memory memory = { { {{"x",5}}}, {{{"a",{10,{{0,5},{1,2},{2,3}}}}, {"b",{10,{{0,5},{1,2},{2,3}}}}  }} };
			Memory expected_submemory = { {}, {{{"b",{10,{{0,5},{1,2},{2,3}}}}}} };
			Memory actual_submemory = memory.get_submemorty({ "b" });
			Assert::IsTrue(expected_submemory == actual_submemory);
		}
	};

	TEST_CLASS(TestExprNode)
	{
		TEST_METHOD(TestTernarTrue)
		{
			ExprNode* cond = eq(create_var_node("a"), create_int_node(5));
			ExprNode* trueBranch = create_int_node(7);
			ExprNode* falseBranch = create_int_node(2);
			ExprNode* ternar = create_ternar(cond, trueBranch, falseBranch);
			Memory memory = { { {{"a",5},{"c",7}}}, {{{"b",{10,{{0,0},{1,2},{2,3},{3,7}}}}}}};
			int val = calcExprNode(ternar, memory);
			int expect = 7;
			Assert::AreEqual(expect, val);
		}

		TEST_METHOD(TestTernarFalse)
		{
			ExprNode* cond = eq(create_var_node("a"), create_int_node(5));
			ExprNode* trueBranch = create_int_node(7);
			ExprNode* falseBranch = create_int_node(2);
			ExprNode* ternar = create_ternar(cond, trueBranch, falseBranch);
			Memory memory = { { {{"a",3},{"c",7}}}, {{{"b",{10,{{0,0},{1,2},{2,3},{3,7}}}}}} };
			int val = calcExprNode(ternar, memory);
			int expect = 2;
			Assert::AreEqual(expect, val);
		}

		TEST_METHOD(TestUnaryMinus)
		{
			ExprNode* arr_ind = get_element_by_index("b", create_int_node(0));
			ExprNode* e = create_unary_minus(arr_ind);
			Memory memory = { {}, {{{"b",{10,{{0,-5},{1,2},{2,3},{3,7}}}}}} };
			int val = calcExprNode(e, memory);
			Assert::AreEqual(5,val);
		}

		TEST_METHOD(TestCalcByMemory)
		{
			ExprNode* index = create_int_node(3);
			ExprNode* arr_ref = get_element_by_index("b", index);
			Memory memory = { {}, {{{"b",{10,{{0,5},{1,2},{2,3},{3,7}}}}}} };
			int val = calcExprNode(arr_ref, memory);
			Assert::AreEqual(val, 7);
		}

		TEST_METHOD(TestNotExprNodeArray)
		{
			Memory memory = { { {{"i",0}}}, {{{"b",{10,{{0,0},{1,2},{2,3},{3,7}}}}}}};
			memory.scalarMemory.var_values["max"] = 0;

			ExprNode* expr = parse_expr_from_RPN("max b i [] > !");
			int val = calcExprNode(expr, memory);
			Assert::AreEqual(val, 1);

			
		}

		TEST_METHOD(TestNotExprNodeArray2)
		{
			Memory memory = { { {{"i",0}}}, {{{"b",{10,{{0,0},{1,2},{2,3},{3,7}}}}}} };
			memory.scalarMemory.var_values["max"] = 0;

			ExprNode* expr = parse_expr_from_RPN("max b i [] >");
			int val = calcExprNode(expr, memory);
			Assert::AreEqual(val, 0);

			ExprNode* not_expr = create_not_node(expr);
			int not_val = calcExprNode(not_expr, memory);
			Assert::AreEqual(not_val, 1);
		}
	};
	
	TEST_CLASS(TestAction)
	{
		TEST_METHOD(TestExecuteScalarAction)
		{
			Action act;
			act.assigned_variable = { SCALAR_VAR, "a" };
			act.action_expr = add(create_var_node("b"), create_var_node("c"));
			act.type = ASSIGN;

			Memory memory = { {{{"b",5},{"c",3}}}, {}};
			Memory expected_memory = { {{{"a",8}, {"b",5},{"c",3}}}, {}};
			act.executeAction(memory);
			Assert::IsTrue(memory == expected_memory);
		}

		TEST_METHOD(TestExecuteScalarActionWithFingerPrint)
		{
			Action act, fing, fing_ref;
			act.assigned_variable = { SCALAR_VAR, "a" };
			act.action_expr = add(create_var_node("b"), create_var_node("c"));
			act.type = ASSIGN;



			Memory memory = { {{{"b",5},{"c",3}}}, {} };
			Memory expected_memory = { {{{"a",8}, {"b",5},{"c",3}}}, {} };
			act.executeAction(memory, &fing);
			Assert::IsTrue(memory == expected_memory);

			Assert::IsTrue(fing.type == ASSIGN);
			Assert::IsTrue(fing.assigned_variable.type == SCALAR_VAR);
			Assert::IsTrue(fing.assigned_variable.name=="a");
			Assert::IsTrue(fing.action_expr->equalsTo(add(create_var_node("b"), create_var_node("c"))));
		};
			

		
	};

	TEST_CLASS(TestVariable)
	{
		TEST_METHOD(TestNotEquelasArrayelements)
		{
			Variable arr1 = create_concrete_array_element_variable("a", 1);
			Variable arr2 = create_concrete_array_element_variable("a", 2);
			Assert::IsTrue(arr1<arr2);
		}

		TEST_METHOD(TestEquelasArrayelements)
		{
			Variable arr1 = create_concrete_array_element_variable("a", 2);
			Variable arr2 = create_concrete_array_element_variable("a", 2);
			Assert::IsFalse(arr1 < arr2);
			Assert::IsTrue(arr1 == arr2);
		}

		TEST_METHOD(TestArrElementAndScalar)
		{
			Variable arr1 = create_concrete_array_element_variable("a", 2);
			Variable x = create_scalar_variable("x");
			Assert::IsTrue(x < arr1);
		}
	};
}
