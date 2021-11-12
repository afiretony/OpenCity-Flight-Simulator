#include "StringPlus.h"
#include <iostream>
#include <sstream>

std::vector<std::string> StringPlus::split(std::string str, std::string delim)
{
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> result;
	current = strtok(cstr, delim.c_str());
	while (current != NULL) {
		result.push_back(current);
		current = strtok(NULL, delim.c_str());
	}
	return result;
}

string StringPlus::wrapLongLine(const string& outString, int lineLength)
{
	int stringLength = outString.length();
	int startPosition = 0, currPosition = lineLength;
	string wrapped = "";

	while (currPosition < stringLength) {
		// work backward from lineLength position to find whitespace
		while (outString[currPosition] != ' ' && outString[currPosition] != '\n')
			currPosition--;

		// printout the substring
		wrapped += outString.substr(startPosition, currPosition - startPosition) + '\n';

		// reset for next line
		startPosition = currPosition + 1;
		currPosition = startPosition + lineLength;
	}

	// printout last part 
	wrapped += outString.substr(startPosition, currPosition - startPosition);

	return wrapped;

}

string StringPlus::replaceAll(const string& original, const string& from, const string& to)
{
	string newString = original;  // make a copy
	size_t startPos = 0;

	while ((startPos = newString.find(from, startPos)) != string::npos) // substring was found
		newString.replace(startPos, from.length(), to);

	return newString;
}

string StringPlus::sigFig(double theValue, int figCount)
{
	std::stringstream theStream;
	theStream.precision(figCount);
	theStream << theValue;
	return theStream.str();
}

double StringPlus::getDouble(std::istream& inputStream, const std::string& prompt)
{
	string userInput;
	double output;
	bool allGood = false;
	while (!allGood) {
		try {
			std::cout << prompt;
			getline(inputStream, userInput);
			output = stod(userInput);
			allGood = true;
		}
		catch (exception e) {
			cout << "ERROR: Unable to convert ->" << userInput << "<- into number" << endl;
			cout << "       " << e.what() << endl << endl;
		}
	}
	return output;
}
