#include <iostream>
#include "include/Utils.hpp"

int main()
{
	 uint32_t channelOps = 0b0010;
	 uint32_t channel = 1;
	 uint32_t mask = (1 << channel) - 2; // -1 to get all 1 and - 2 to set the least fignificant bit to 0
	 uint32_t preserved = channelOps & mask;
	 uint32_t max = -1;
	 uint32_t shifted = (channelOps & (max & ~mask)) >> 1;

	 uint32_t ans = shifted | preserved;
	 std::cout << ans << '\n';
	return 0;
}