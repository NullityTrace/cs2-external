#pragma once
#include <memory>
#include "HandleInspector.h"
#include "pProcess.h"

class Reader {
public:
    std::shared_ptr<pProcess> process;
    ProcessModule base_client;
    ProcessModule base_engine;

    void init();
};

inline Reader Reader;
