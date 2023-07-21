#ifndef QB_EXCEPTION_H
#define QB_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class qb_exception : public std::exception {
private:
    std::string errorMessage;
public:
    explicit qb_exception(std::string  message) : errorMessage(std::move(message)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

#endif
