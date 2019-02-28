#pragma once

#include <llvm_experiments/core/export.hpp>
#include <stdexcept>

namespace llvm_experiments {
namespace core {
namespace exceptions_base {

class LLVM_EXPERIMENTS_CORE_EXPORT templateproj_error_base
    :public std::runtime_error
{
public:
    templateproj_error_base(const std::string &aMessage, const int &aErrorCode)
        :runtime_error(aMessage), m_ErrorCode(aErrorCode)
    {}

    int GetErrorCode() const
    {
        return m_ErrorCode;
    }

private:
    int m_ErrorCode;
};

}
}
}
