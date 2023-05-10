#pragma once

template <typename T, typename E> class Result {
private:
  T value;
  E error;
  bool errSet;

public:
  bool operator==(Result<T, E> const &rhs) const {
    return errSet ? error == rhs.error : value == rhs.value;
  }

  bool operator!=(Result<T, E> const &rhs) const { return !(*this == rhs); }

  void setError(E error) {
    this->error = error;
    errSet = true;
  }

  void setValue(T value) {
    this->value = value;
    errSet = false;
  }

  bool hasError() { return errSet; }

  T getValue() { return value; }

  E getError() { return error; }
};
