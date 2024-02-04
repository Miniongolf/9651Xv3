#include <cmath>
#include <array>
#include <vector>

namespace nicklib {
/**
 * @brief Curves the input.
 * @param input the number to curve.
 * @param gain gain value of the curve.
 * @param scale the max value of the input/output.
 * @return the curved input.
 */
float curveInput(float input, float gain = 2.0, float scale = 1.0);

/**
 * @brief Normalizes the velocities.
 * @param vel1 the first velocity.
 * @param vel2 the second velocity.
 * @param scale the max value of the input/output.
 * @return array of the normalized velocities.
 */
std::array<float, 2> normalizeVels(float vel1, float vel2, float scale = 1.0);

/**
 * @brief Normalizes the velocities.
 * @param vels vector of velocities.
 * @param scale the max value of the input/output.
 * @return vector of the normalized velocities.
 */
std::vector<float> normalizeVels(std::vector<float> vels, float scale = 1.0);

/**
 * @brief Applies a cross deadzone to the input.
 * @param input the input to apply the deadzone to.
 * @param deadzone the minimum input to consider.
 * @return the input with the deadzone applied.
 */
float deadzone(float input, float deadzone = 0.1);

} // namespace nicklib