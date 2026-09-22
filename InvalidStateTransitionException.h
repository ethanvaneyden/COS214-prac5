#ifndef INVALID_STATE_TRANSITION_EXCEPTION_H
#define INVALID_STATE_TRANSITION_EXCEPTION_H

#include <stdexcept>
#include <string>

/**
 * @brief This class is an exception when you have an invalid state transition
 *
 */
class InvalidStateTransitionException {
private:
  std::string message;

public:
  /**
   * @brief Construct a new Invalid State Transition Exception object
   *
   * @param message
   */
  InvalidStateTransitionException(const std::string &message);
};

#endif