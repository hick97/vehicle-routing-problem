#include <vector>
#include "Scenary.h"

using namespace std;

struct Solution
{
  Scenary *s;
  unsigned int fitness;

  Solution(Scenary *scenary);
  unsigned int construction();
};