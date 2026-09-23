#include "InvalidStateTransitionException.h"

using namespace std;

InvalidStateTransitionException::InvalidStateTransitionException(
    const string &message)
    : std::runtime_error(message) {}