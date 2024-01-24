#ifndef VARIABLETABLE_H
#define VARIABLETABLE_H

#include <map>
#include <string>

class VariableTable {
public:
	VariableTable() : variable_map{ } {}
	~VariableTable() = default;

	VariableTable(const VariableTable& other) = default;
	VariableTable& operator=(const VariableTable& other) = default;

	// The method takes care of storing the variable if it hasn't been defined 
	// otherwise it updates its value.
	void push(std::string name, int64_t value) {
		auto itr = variable_map.find(name);
		// If the variable is not present, it gets created
		if (itr == variable_map.end())
			variable_map.insert({ name, value });

		// otherwise, the value is updated
		else
			itr->second = value;
	}
	int64_t getVariableValue(std::string name) const {
		auto itr = variable_map.find(name);
		return itr->second;
	}
	bool FindVariable(std::string name) const {
		auto itr = variable_map.find(name);
		// If the variable is not present, it returns false
		if (itr == variable_map.end())
			return false;

		// otherwise, it returns true
		else
			return true;
	}
private:
	std::map<std::string, int64_t> variable_map;
};

#endif
