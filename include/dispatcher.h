// Dispatcher

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>
#include <string>

#include "llvm/Module.h"

#include "types.h"

class Dispatcher
{
    std::map<std::string, std::vector< std::pair<llvm::Function*, llvm::Function*> > > methods;
    Generator *generator;
  public:
    Dispatcher(Generator *_generator) : generator(_generator) {}

    void registerMethod(std::string name, llvm::Function *fn, llvm::Function *pred);
    llvm::Function *dispatch(std::string name, std::vector<llvm::Value*> arguments);
};

#endif
