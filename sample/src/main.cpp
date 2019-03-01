#include <llvm_experiments/core/sample.hpp>
#include "llvm/Support/InitLLVM.h"

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
    bool VisitEnumDecl(clang::EnumDecl *D)
    {
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

static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional, llvm::cl::desc("<input files>"), llvm::cl::ZeroOrMore);

int main(int argc, char **argv)
{
    llvm::InitLLVM X(argc, argv);
    llvm::cl::ParseCommandLineOptions(argc, argv, "llvm object size dumper\n");
    
    for (const auto &file : InputFilenames)
    {
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> Buffer = llvm::MemoryBuffer::getFileOrSTDIN(file);
        if (std::error_code EC = Buffer.getError())
            llvm::errs() << file << ": " << EC.message() << '\n';
        else
            clang::tooling::runToolOnCode(new MyAction, Buffer.get()->getMemBufferRef().getBuffer());

    }
  return EXIT_SUCCESS;
}

