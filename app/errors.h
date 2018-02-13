#include <exception>

class ConstraintViolation : public std::exception {};
class CannotPlaceItem : public std::exception {};
