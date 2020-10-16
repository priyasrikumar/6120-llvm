#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

namespace
{
  struct SkeletonPass : public FunctionPass
  {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F)
    {
      for (auto &B : F)
      {
        for (auto &I : B)
        {
          if (auto *op = dyn_cast<BinaryOperator>(&I))
          {
            // Insert at the point where the instruction `op` appears.
            IRBuilder<> builder(op);

            // Make a multiply with the same operands as `op`.
            Value *lhs = op->getOperand(0);
            Value *rhs = op->getOperand(1);
            Value *res = nullptr;
            switch (op->getOpcode())
            {
            case 13:
              res = builder.CreateSub(lhs, rhs);
              break;
            case 14:
              res = builder.CreateFSub(lhs, rhs);
              break;
            case 15:
              res = builder.CreateAdd(lhs, rhs);
              break;
            case 16:
              res = builder.CreateFAdd(lhs, rhs);
              break;
            case 17:
              res = builder.CreateUDiv(lhs, rhs);
              break;
            case 18:
              res = builder.CreateFDiv(lhs, rhs);
              break;
            case 19:
              res = builder.CreateMul(lhs, rhs);
              break;
            case 20:
              res = builder.CreateMul(lhs, rhs);
              break;
            case 21:
              res = builder.CreateFMul(lhs, rhs);
              break;
            case 28:
              res = builder.CreateOr(lhs, rhs);
              break;
            case 29:
              res = builder.CreateAnd(lhs, rhs);
              break;
            default:
              res = op;
            }
            errs() << "Old Instruction: " << op << "\n";
            for (auto &U : op->uses())
            {
              User *user = U.getUser();
              user->setOperand(U.getOperandNo(), res);
            }
            errs() << "Changed Instruction: " << op << "\n";
            return true;
          }
        }
      }

      return false;
    }
  };
} // namespace

char SkeletonPass::ID = 0;

static void registerSkeletonPass(const PassManagerBuilder &,
                                 legacy::PassManagerBase &PM)
{
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerSkeletonPass);