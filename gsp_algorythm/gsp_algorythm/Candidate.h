#pragma once

#include <vector>

class Candidate
{
public:
	std::vector<long> value;
	unsigned support = 0;

	Candidate();
	Candidate(long value);
	Candidate(std::vector<long> value);
	Candidate(unsigned support);
	Candidate(long value, unsigned support);
	Candidate(std::vector<long> value, unsigned support);
};

