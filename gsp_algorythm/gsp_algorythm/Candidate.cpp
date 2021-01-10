#include "Candidate.h"

Candidate::Candidate() {
}

Candidate::Candidate(long value) {
	this->value.push_back(value);
}

Candidate::Candidate(unsigned support) {
	this->support = support;
}

Candidate::Candidate(long value, unsigned support) {
	this->value.push_back(value);
	this->support = support;
}

Candidate::Candidate(std::vector<long> value) {
	this->value = value;
}

Candidate::Candidate(std::vector<long> value, unsigned support) {
	this->value = value;
	this->support = support;
}