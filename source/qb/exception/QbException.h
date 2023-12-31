#ifndef QB_EXCEPTION_H
#define QB_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class QbException : public std::exception {
private:
    std::string errorMessage;
public:
    explicit QbException(std::string  message) : errorMessage(std::move(message)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

#endif
