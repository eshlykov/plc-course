#include "Exception.h"

#include <memory>

std::unique_ptr<CThrowHandler> topThrowHandler{ new CThrowHandler{} };
