#include <llvm_experiments/core/sample.hpp>
#include "clang/Tooling/Tooling.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"


class MyVisitor : public clang::RecursiveASTVisitor<MyVisitor>
{
public:
    bool VisitTranslationUnitDecl(clang::TranslationUnitDecl *D)
     {
     D->dump();
     return true;
     }
    bool VisitDeclaratorDecl(clang::DeclaratorDecl *D)
    {
        llvm::outs() << D->getNameAsString();
        //llvm::outs() << "------------------------------------------\n";
        
        return true;
    }
};

class MyConsumer : public clang::ASTConsumer
{
public:
    virtual void HandleTranslationUnit(clang::ASTContext &Context) override
    {
        _visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
private:
    MyVisitor _visitor;
};

class MyAction : public clang::ASTFrontendAction
{
protected:
    typedef std::unique_ptr<clang::ASTConsumer> ASTConsumerPtr;
    virtual ASTConsumerPtr CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) override
    {
        return ASTConsumerPtr(new MyConsumer);
    }
};

int main()
{
    
    
     const bool ret = clang::tooling::runToolOnCode(new MyAction,
     "struct MyStruct {};"
     "class MyClass {};"
     "namespace Nspace {"
     "class MyDerived : public MyStruct {};"
     "class MyDerived2 : public MyDerived, public virtual MyClass {};"
     "}");
    
    /*const bool ret = clang::tooling::runToolOnCode(new MyAction,
                                                   "enum enNotifyIds"
                                                   "{"
                                                   "eNotify_None = 0,"
                                                   "eNotify_Any_InternalError = 101,"
                                                   "eNotify_Product_LicenceAgreementViolated = 201,"
                                                   "eNotify_Product_LicenceAlmostExpired = 203,"
                                                   "eNotify_Product_LicenceExpiresSoon = 204,"
                                                   "};");*/
    return ret ? 0 : -1;
    //return 0;
}

