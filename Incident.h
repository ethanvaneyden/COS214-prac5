#ifndef INCIDENT_H
#define INCIDENT_H

#include "IncidentState.h"
#include <memory>
#include <string>

/**
 * @brief Represents a reported incident
 *
 */
class Incident {
private:
  std::string id;
  std::string location;
  std::string description;
  std::string type;
  std::unique_ptr<IncidentState> state;

public:
  /**
   * @brief Construct a new Incident object
   *
   * @param id
   * @param location
   * @param description
   * @param type
   */
  Incident(std::string id, std::string location, std::string description,
           std::string type);
  /**
   * @brief Set the state
   *
   * @param newState
   */
  void setState(std::unique_ptr<IncidentState> newState);
  /**
   * @brief Delegates the escalatation to the current state
   *
   */
  void escalate();

  /**
   * @brief Get the ID
   *
   * @return std::string
   */
  std::string getId() const;
  /**
   * @brief Get the Location
   *
   * @return std::string
   */
  std::string getLocation() const;
  /**
   * @brief Get the Description
   *
   * @return std::string
   */
  std::string getDescription() const;
  /**
   * @brief Get the Type
   *
   * @return std::string
   */
  std::string getType() const;
  /**
   * @brief Get the State Name
   *
   * @return std::string
   */
  std::string getStateName() const;
};

#endif