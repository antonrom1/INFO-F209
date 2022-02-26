//
// Created by Anton Romanova on 17/02/2022.
//

#include "password.h"
#include "../../constants.h"

/// check input password conformity
/// \param value
/// \return
ValidatorResult PasswordValidator::Validate(const std::string &value) {
  if (value.size() < MIN_PASSWORD_LENGTH) {
    return ValidatorResult{"Password too short"};
  } else if (value.size() > MAX_PASSWORD_LENGTH) {
    return ValidatorResult{"Password too long"};
  }
  return {};
}