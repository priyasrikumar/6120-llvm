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
      bool modified = false;
      for (auto &B : F)
      {
        for (auto &I : B)
        {
          if (auto *op = dyn_cast<BinaryOperator>(&I))
          {
            errs() << "Old Instruction: " << I << "\n";
            IRBuilder<> builder(op);

            Value *lhs = op->getOperand(0);
            Value *rhs = op->getOperand(1);
            Instruction *res = nullptr;
            switch (op->getOpcode())
            {
            case 13:
              res = BinaryOperator::CreateSub(lhs, rhs);
              break;
            case 14:
              res = BinaryOperator::CreateFSub(lhs, rhs);
              break;
            case 15:
              res = BinaryOperator::CreateAdd(lhs, rhs);
              break;
            case 16:
              res = BinaryOperator::CreateFAdd(lhs, rhs);
              break;
            case 17:
              res = BinaryOperator::CreateUDiv(lhs, rhs);
              break;
            case 18:
              res = BinaryOperator::CreateFDiv(lhs, rhs);
              break;
            case 19:
              res = BinaryOperator::CreateMul(lhs, rhs);
              break;
            case 20:
              res = BinaryOperator::CreateMul(lhs, rhs);
              break;
            case 21:
              res = BinaryOperator::CreateFMul(lhs, rhs);
              break;
            case 28:
              res = BinaryOperator::CreateOr(lhs, rhs);
              break;
            case 29:
              res = BinaryOperator::CreateAnd(lhs, rhs);
              break;
            default:
              res = &I;
            }
            res->setHasNoSignedWrap(I.hasNoSignedWrap());
            B.getInstList().insert(op->getIterator(), res);
            errs() << "New Instruction: " << *res << "\n";
            modified = true;
          }
        }
      }

      return modified;
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