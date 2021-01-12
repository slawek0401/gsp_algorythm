#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

#include "Candidate.h"

typedef std::vector<long> Sequence;
typedef std::vector<Sequence> Sequences;

Sequence deepCopy(const Sequence &vec) {
	std::vector<long> result;
	result.assign(vec.begin(), vec.end());
	//copy(vec.begin(), vec.end(), result.begin());
	return result;
}

void print(const Sequence& vec) {
	for (long i : vec)
		std::cout << i << " ";
}

void print(const Sequences& vec) {
	for (auto i : vec) {
		print(i);
		std::cout << std::endl;
	}
}

void printCandidates(const std::vector<Candidate> &vec) {
	for (auto i : vec) {
		print(i.value);
		std::cout << "sup:" << i.support << std::endl;
	}
}

Sequences load(std::string filename) {
	std::fstream file(filename, std::ios::in);
	Sequences data;
	Sequence currentSequence;

	while (!file.eof() && file.is_open()) {
		long value;
		file >> value;
		if (value < 0) {
			if (value == -2) {
				data.push_back(deepCopy(currentSequence));
				currentSequence.clear();
			}
		}
		else {
			currentSequence.push_back(value);
		}
	}
	return data;
}

void countSupport(const Sequences &data, std::vector<Candidate> &candidatesVector) {
	for (auto sequence : data)
		for (unsigned candidateIter = 0; candidateIter < candidatesVector.size(); ++candidateIter) {
			auto candidate = candidatesVector[candidateIter];
			unsigned sequenceIterator = 0;
			unsigned itemsFound = 0;
			for (auto item : candidate.value) {
				while (sequenceIterator < sequence.size() && sequence[sequenceIterator] != item)
					++sequenceIterator;
				if (sequenceIterator == sequence.size())
					break;
				if (sequence[sequenceIterator] == item) {
					++itemsFound;
					++sequenceIterator;
				}
			}
			if (itemsFound == candidate.value.size())
				++candidatesVector[candidateIter].support;
		}
}

std::vector<Candidate> findItems(const Sequences& vec ) {
	std::set<long> set;
	std::vector<Candidate> result;
	for (auto i : vec)
		for(auto j : i)
			set.insert(j);
	for (auto i : set)
		result.push_back(Candidate(i));
	return result;
}

void removeLowSupportCandidates(std::vector<Candidate> &candidatesVector, unsigned minimumSupport) {
	for (unsigned i = 0; i < candidatesVector.size(); ++i) {
		if (candidatesVector[i].support < minimumSupport) {
			candidatesVector.erase(candidatesVector.begin() + i);
			--i;
		}
	}
}

void addToResult(std::vector<Candidate> &result, const std::vector<Candidate> &candidatesVector) {
	for (auto candidate : candidatesVector) 
		result.push_back(candidate);
}

std::vector<Candidate> generateCandidatesLengthTwo(std::vector<Candidate> previousLengthSequences) {
	std::vector<Candidate> result;
	for (auto i : previousLengthSequences)
		for (auto j : previousLengthSequences) {
			Sequence candidateValue = { i.value[0], j.value[0] };
			result.push_back(Candidate(candidateValue));
		}
	return result; 
}

bool sameMiddle(const Candidate &a, const Candidate &b) {
	for (unsigned i = 1; i < a.value.size(); ++i)
		if (a.value[i] != b.value[i - 1])
			return false;
	return true;
}

std::vector<Candidate> generateCandidatesHigherLength(std::vector<Candidate> previousLengthSequences) {
	std::vector<Candidate> result;
	for (auto i : previousLengthSequences)
		for (auto j : previousLengthSequences) {
			if (sameMiddle(i, j)) {
				Sequence candidateValue = deepCopy(i.value);
				candidateValue.push_back(j.value[j.value.size() - 1]);
				result.push_back(Candidate(candidateValue));
			}
		}
	return result;
}

std::vector<Candidate> generateCandidates(std::vector<Candidate> previousLengthSequences) {
	if (previousLengthSequences[0].value.size() == 1)
		return generateCandidatesLengthTwo(previousLengthSequences);
	else
		return generateCandidatesHigherLength(previousLengthSequences);
}

bool equals(const Sequence &first, unsigned exclude, const Sequence &second) {
	if (first.size() - 1 != second.size())
		return false;
	unsigned j = 0;
	for (unsigned i = 0; i < second.size(); ++i) {
		if (i == j)
			++j;
		if (first[j] != second[i])
			return false;
		++j;
	}
	return true;
}

bool isPreviousSequence(Sequence subSequence, unsigned j, const std::vector<Candidate> &previousLengthSequences) {
	for (auto seq : previousLengthSequences) 
		if (equals(subSequence, j, seq.value))
			return true;
	return false;
}

void cleanCandidates(std::vector<Candidate> &candidatesVector, const std::vector<Candidate> &previousLengthSequences) {
	for (unsigned i = 0; i < candidatesVector.size(); ++i) {
		for (unsigned j = 0; j < candidatesVector[i].value.size(); ++j) {
			if (!isPreviousSequence(candidatesVector[i].value, j, previousLengthSequences)) {
				candidatesVector.erase(candidatesVector.begin() + i);
				--i;
				break;
			}
		}
	}
}

std::vector<Candidate> gsp(std::string filename, unsigned minimalSupport) {
	auto data = load(filename);
	std::vector<Candidate> result;
	std::vector<Candidate> previousLengthSequences;
	unsigned iterations = 1; //also candidates-sequence length
	std::vector<Candidate> candidatesVector;

	while (true) {
		if (iterations == 1)
			candidatesVector = findItems(data);
		else
			candidatesVector = generateCandidates(previousLengthSequences);
		if (candidatesVector.size() == 0)
			break;
		if (iterations >= 3)
			cleanCandidates(candidatesVector, previousLengthSequences);
		if (candidatesVector.size() == 0)
			break;
		countSupport(data, candidatesVector);
		removeLowSupportCandidates(candidatesVector, minimalSupport);
		if (candidatesVector.size() == 0)
			break;
		addToResult(result, candidatesVector);
		previousLengthSequences = candidatesVector;
		++iterations;
	}
	return result;
}

int main() {
	std::string filename = "data2.txt";
	unsigned minimalSupport = 2;
	auto result = gsp(filename, minimalSupport);
	printCandidates(result);
}