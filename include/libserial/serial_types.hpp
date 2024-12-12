#ifndef SERIAL_TYPES_HPP
#define SERIAL_TYPES_HPP


namespace serial {
  /**
   * @brief Enum class
   */
  enum class NumBits {
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
  };

  /**
   * @brief Enum class
   */
  enum class Parity {
    ENABLE,
    DISABLE,
  };

  enum class StopBits {
    ENABLE,
    DISABLE,
  };

  enum class FlowControl {
    Hardware,
    Software,
  };

  enum class CanonicalMode {
    ENABLE,
    DISABLE,
  };

  enum class Terminator {
    EOT = 4,
    CR  = 13,
  };

} // namespace serial




#endif // SERIAL_TYPES_HPP