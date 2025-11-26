#include <stdio.h>

int calculateFuel(int fuel, int consumption, int recharge, int solarBonus,
                  int planet, int totalPlanets) {
  if (fuel <= 0) {
    printf("Planet %d: Fuel Remaining = 0 (Fuel exhausted!)\n", planet);
    return 0;
  }

  if (planet > totalPlanets) {
    return 1;
  }

  fuel += recharge - consumption;

  if (planet % 4 == 0) {
    fuel += solarBonus;
    printf("Solar Recharge Activated at Planet %d! +%d Fuel\n", planet,
           solarBonus);
  }

  if (fuel < 0)
    fuel = 0;

  printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);

  return calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1,
                       totalPlanets);
}

int main() {
  int startingFuel = 500;
  int consumption = 70;
  int recharge = 20;
  int solarBonus = 100;
  int totalPlanets = 10;

  printf("Spacecraft Fuel Simulation Started\n");
  printf("----------------------------------\n");

  int success = calculateFuel(startingFuel, consumption, recharge, solarBonus,
                              1, totalPlanets);

  printf("\n----------------------------------\n");

  if (success)
    printf("MISSION STATUS: Success! Spacecraft completed its journey.\n");
  else
    printf("MISSION STATUS: Failure! Spacecraft ran out of fuel.\n");

  return 0;
}
