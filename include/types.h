// typedefs

#ifndef TYPES__H
#define TYPES__H

#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/IRBuilder.h"

typedef llvm::IRBuilder<> Builder;

typedef std::function<llvm::Value*()> generateFn;

class Generator;

#endif
