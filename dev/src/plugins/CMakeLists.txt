# Add a line for your plugin here

## Models
add_subdirectory(analytic)
add_subdirectory(rk4)

## Percentile engines
add_subdirectory(taylor)
add_subdirectory(monte_carlo)

## Post engines
add_subdirectory(bayesian)

## Reverse engines
add_subdirectory(newton)
add_subdirectory(csp)
