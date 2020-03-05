#include <llvm_experiments/core/sample.hpp>
#include "llvm/Support/InitLLVM.h"

#include "clang/Tooling/Tooling.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

std::string GetName(const clang::QualType& type)
{
    if(type->isRecordType())
        return type->getAsCXXRecordDecl()->getName();
    return type.getAsString();
}

std::string GetSignature(clang::CXXMethodDecl *method)
{
    std::string signature = GetName(method->getReturnType());
    signature += " " + method->getNameAsString() + "(";
    bool isFirst = true;
    for(auto param: method->parameters())
    {
        if(!isFirst)
            signature += ", ";
        signature += GetName(param->getType());
        isFirst = false;
    }
    signature += ")";
    return signature;
}


class MyVisitor : public clang::RecursiveASTVisitor<MyVisitor>
{
public:
    bool VisitCXXRecordDecl(clang::CXXRecordDecl *record)
    {
        llvm::outs() << "class " << record->getName() << "{" << '\n';
        
        for(auto method: record->methods())
            llvm::outs() << "   " << GetSignature(method) << '\n';
        for(auto field: record->fields())
            llvm::outs() << "   " << GetName(field->getType())<< " " << field->getName() << '\n';
        
        llvm::outs() << "}" << '\n';

        for(auto base: record->bases())
            llvm::outs() << base.getType()->getAsCXXRecordDecl()->getName() << " <|-- " << record->getName() <<'\n';
        
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
        Compiler.getDiagnostics().setClient(new clang::IgnoringDiagConsumer);
        return ASTConsumerPtr(new MyConsumer);
    }
};

static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional, llvm::cl::desc("<input files>"), llvm::cl::ZeroOrMore);

int main(int argc, char **argv)
{
    llvm::InitLLVM X(argc, argv);
    llvm::cl::ParseCommandLineOptions(argc, argv, "converter to plantuml format\n");
    
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

