#ifndef INCIDENT_STATE_H
#define INCIDENT_STATE_H

#include <string>

class Incident;

/**
 * @brief The interface for states
 *
 */
class IncidentState {
public:
  /**
   * @brief Destroy the Incident State object
   *
   */
  virtual ~IncidentState() = default;
  /**P
   * @brief Decides how to handle the escalation of the incident
   *
   * @param context
   */
  virtual void handleEscalation(Incident *context) = 0;
  /**
   * @brief Get the name of the state
   *
   * @return std::string
   */
  virtual std::string getStateName();
};

#endif