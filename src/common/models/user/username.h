//
// Created by Anton Romanova on 17/02/2022.
//

#ifndef QUORIDOR_SRC_SERVER_SIDE_MODELS_USER_USERNAME_H_
#define QUORIDOR_SRC_SERVER_SIDE_MODELS_USER_USERNAME_H_

#include "abstract_validator.h"
#include <exception>

struct InvalidUsernameError : public std::exception {
  [[nodiscard]] const char *what() const noexcept override;
  ///
  /// \param validation_error_description
  InvalidUsernameError(std::string validation_error_description);
 private:
  std::string error_message_;
};


struct UsernameValidator : public AbstractValidator {
 public:
  ValidatorResult Validate(const std::string &value) override;
};

class Username {
 public:
   Username();

   static bool IsValid(const std::string &username);

    explicit Username(std::string value);

    [[nodiscard]] const std::string &GetValue() const;

 private:
    std::string value_;
};

#endif //QUORIDOR_SRC_SERVER_SIDE_MODELS_USER_USERNAME_H_
