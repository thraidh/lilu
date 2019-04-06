
#include "AstBuilder.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include <iostream>

using namespace std;
using namespace llvm;
using namespace llvm::orc;

class BuildVisistor : public AstVisitor<Value *>
{
  public:
    LLVMContext Context;
    Module *M;
    IRBuilder<> builder;
    map<string, Value *> lookup;
    map<string, Function *> funcs;
    std::unique_ptr<Module> Owner;

    BuildVisistor() : Context(), M(nullptr), builder(Context)
    {
        // Create some module to put our function into it.
        Owner = llvm::make_unique<Module>("test", Context);
        M = Owner.get();
    }
    Value *visit_AstNode(AstNode const *n, void *ctx) override
    {
        return nullptr;
    };
    Value *visit_ArgNode(ArgNode const *n, void *ctx) override
    {
        return nullptr;
    };
    Value *visit_FunctionNode(FunctionNode const *n, void *ctx) override
    {
        Type *retType = typeFrom(n->rtype);

        SmallVector<Type *, 8> ArgTys;
        for (auto arg : n->args)
        {
            ArgTys.push_back(typeFrom(arg->type));
        }

        auto functype = FunctionType::get(retType, ArgTys, false);

        Function *func =
            cast<Function>(M->getOrInsertFunction(n->name, functype));
        funcs[n->name] = func;

        auto it = func->arg_begin();
        for (auto arg : n->args)
        {
            it->setName(arg->name);
            lookup[arg->name] = it;
            ++it;
        }

        BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", func);
        builder.SetInsertPoint(BB);
        auto ret = visit(n->body);
        builder.CreateRet(ret);
        return nullptr;
    };
    Value *visit_BlockNode(BlockNode const *n, void *ctx) override
    {
        Value *ret = nullptr;
        for (auto e : n->elements)
            ret = visit(e);
        return ret;
    };
    Value *visit_ExprNode(ExprNode const *n, void *ctx) override
    {
        return nullptr;
    };
    Value *visit_IdExprNode(IdExprNode const *n, void *ctx) override
    {
        auto l = lookup[n->id];
        if (l)
            return l;
        return builder.getInt32(12345);
    };
    Value *visit_NumberExprNode(NumberExprNode const *n, void *ctx) override
    {
        return builder.getInt32(n->num);
    };
    Value *visit_FuncallExprNode(FuncallExprNode const *n, void *ctx) override
    {
        auto op = ((IdExprNode *)n->exprs[0])->id;
        auto it = n->exprs.begin();
        ++it;
        vector<Value *> args;
        while (it != n->exprs.end())
        {
            args.push_back(visit(*it));
            ++it;
        }
        if (op == "+")
        {
            return builder.CreateAdd(args[0], args[1]);
        }
        if (op == "-")
        {
            return builder.CreateSub(args[0], args[1]);
        }
        if (op == "*")
        {
            return builder.CreateMul(args[0], args[1]);
        }
        if (op == "/")
        {
            return builder.CreateSDiv(args[0], args[1]);
        }
        return nullptr;
    };

    void print()
    {
        outs() << "We just constructed this LLVM module:\n\n"
               << *M;
        outs().flush();
    }

    Type *typeFrom(string const &s)
    {
        if (s == "int")
            return Type::getInt32Ty(Context);
        return Type::getVoidTy(Context);
    }
};

/*
int main(int argc, char **argv)
{
    LLVMContext Context;

    // Create some module to put our function into it.
    std::unique_ptr<Module> Owner = llvm::make_unique<Module>("test", Context);
    Module *M = Owner.get();

    // Create the add1 function entry and insert this entry into module M.  The
    // function will have a return type of "int" and take an argument of "int".
    Function *Add1F =
        cast<Function>(M->getOrInsertFunction("add1", Type::getInt32Ty(Context),
                                              Type::getInt32Ty(Context)));

    BasicBlock *BB = nullptr;
    //IRBuilder<> builder(BB);

    // Add a basic block to the function. As before, it automatically inserts
    // because of the last argument.
    BB = BasicBlock::Create(Context, "EntryBlock", Add1F);

    // Get pointers to the constant `1'.
    Value *One = builder.getInt32(1);

    // Get pointers to the integer argument of the add1 function...
    assert(Add1F->arg_begin() != Add1F->arg_end()); // Make sure there's an arg
    Argument *ArgX = &*Add1F->arg_begin();          // Get the arg
    ArgX->setName("AnArg");                         // Give it a nice symbolic name for fun.

    // Create the add instruction, inserting it into the end of BB.
    Value *Add = builder.CreateAdd(One, ArgX);

    // Create the return instruction and add it to the basic block
    builder.CreateRet(Add);

    // Now, function add1 is ready.

    // Now we're going to create function `foo', which returns an int and takes no
    // arguments.
    Function *FooF =
        cast<Function>(M->getOrInsertFunction("foo", Type::getInt32Ty(Context)));

    // Add a basic block to the FooF function.
    BB = BasicBlock::Create(Context, "EntryBlock", FooF);
    IRBuilder<> builder(BB);

    // Tell the basic block builder to attach itself to the new basic block
    builder.SetInsertPoint(BB);

    // Get pointer to the constant `10'.
    Value *Ten = builder.getInt32(10);

    // Pass Ten to the call to Add1F
    CallInst *Add1CallRes = builder.CreateCall(Add1F, Ten);
    Add1CallRes->setTailCall(true);

    // Create the return instruction and add it to the basic block.
    builder.CreateRet(Add1CallRes);

    // Now we create the JIT.
    ExecutionEngine *EE = EngineBuilder(std::move(Owner)).create();
    EE->addGlobalMapping(Add1F, (void *)add10);

    outs() << "We just constructed this LLVM module:\n\n"
           << *M;
    outs() << "\n\nRunning foo: ";
    outs().flush();

    // Call the `foo' function with no arguments:
    std::vector<GenericValue> noargs;
    GenericValue gv = EE->runFunction(FooF, noargs);

    std::vector<GenericValue> Args(1);
    Args[0].IntVal = APInt(32, n);
    GenericValue GV = EE->runFunction(FibF, Args);

    // Import result of execution:
    outs() << "Result: " << gv.IntVal << "\n";
    delete EE;
    llvm_shutdown();

    return 0;
}
*/

std::unique_ptr<LLJIT> makeJit()
{
    auto ES = llvm::make_unique<ExecutionSession>();
    auto JTMB = cantFail(JITTargetMachineBuilder::detectHost());
    auto TM = cantFail(JTMB.createTargetMachine());
    auto DL = TM->createDataLayout();
    return cantFail(LLJIT::Create(std::move(ES), std::move(TM), std::move(DL)));
}

void buildIRFrom(AstNode const *n)
{
    BuildVisistor v;
    v.visit(n);
    v.print();

    auto jit = makeJit();

    cantFail(jit->addIRModule(std::move(v.Owner)));

    //    int (*fun)(int, int) = (int (*)(int, int))EE->getPointerToFunction(v.funcs["cde"]);
    // int (*fun)(int) = (int (*)(int))EE->getPointerToFunction(v.funcs["cde"]);
    // int res = fun(2);
    // outs() << "Result: " << res << "\n";

    //std::vector<GenericValue> args;
    //args[0].IntVal = APInt(32, 2);
    //GenericValue gv = EE->runFunction(v.funcs["xyz"], args);
    //outs() << "Result: " << gv.IntVal << "\n";

    auto fn = cantFail(jit->lookup("abc"));
    int (*fun2)(int, int) = (int (*)(int, int))fn.getAddress();
    int res = fun2(2, 6);
    outs() << "Result: " << res << "\n";
}