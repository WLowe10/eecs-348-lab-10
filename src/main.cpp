/*
EECS 348 Lab 10
Author: Wesley Lowe
Date: 2025-04-17
Description: Adds a string double to a string double from a file.
Sources: Claude
Extra notes: All code blocks annotated with an author comment are written by the respective author(s) for the entire block unless specified otherwise.
*/

#include <iostream>
#include <fstream>
#include <stdexcept>

#define TEST_FILE_PATH "./test-file.txt"

// bring std::string into the global namespace
using std::string;

// --- utils ---

// checks if a char is a digit
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

class StringDouble {
	public:
		string value;
		string integerPart;
		string decimalPart;
		bool isNegative;

		// static method to create a StringDouble from a string
		// throws error is string is not valid
		static StringDouble fromStr(string str) {
			// check if string is empty
			if (str.empty()) {
				throw std::invalid_argument("String is empty");
			}

			// create new StringDouble object
			StringDouble newDbl;

			// position to begin parsing digits
			size_t startPos = 0;

			// assign the value
			newDbl.value = str;
		
			// Handle sign
			if (str[0] == '+') {
				newDbl.isNegative = false;
				// skip the sign char
				startPos = 1;
			} else if (str[0] == '-') {
				newDbl.isNegative = true;
				// skip the sign char
				startPos = 1;
			}
			
			// find decimal point position
			size_t decimalPos = str.find('.', startPos);

			// if decimal point exists
			if (decimalPos != std::string::npos) {
				// get chars from start to decimal (the integer part of the double)
				newDbl.integerPart = str.substr(startPos, decimalPos - startPos);
			} else {
				// otherwise, all chars are the integer part
				newDbl.integerPart = str.substr(startPos);
			}
					
			// validate that the integer part isn't empty
			if (newDbl.integerPart.empty()) {
				throw std::invalid_argument("Integer part is empty");
			}

			// validate that all characters in integer part are digits
			for (int i = 0; i < newDbl.integerPart.length(); i++) {
				if (!isDigit(newDbl.integerPart[i])) {
					throw std::invalid_argument("Invalid character in integer part");
				}
			}
			
			// if decimal point exists, get the decimal part
			if (decimalPos != std::string::npos) {
				// get the characters after the decimal point
				newDbl.decimalPart = str.substr(decimalPos + 1);
				
				// validate decimal part isn't empty
				if (newDbl.decimalPart.empty()) {
					throw std::invalid_argument("Decimal part is empty");
				}
				
				// check each char to make sure they are digits
				for (int i = 0; i < newDbl.decimalPart.length(); i++) {
					if (!isDigit(newDbl.decimalPart[i])) {
						throw std::invalid_argument("Invalid character in decimal part");
					}
				}
			}

			return newDbl;
		}

		// default constructor
		StringDouble() {
			this->value = "";
			this->isNegative = false;
			this->integerPart = "";
			this->decimalPart = "";
		}

		// compares	absolute values of two StringDouble objects
		// source: Claude
		int compareAbsoluteValues(StringDouble& other) {
            // Compare integer part lengths first
            if (this->integerPart.length() > other.integerPart.length()) return 1;
            if (this->integerPart.length() < other.integerPart.length()) return -1;
            
            // Compare integer parts digit by digit
            for (size_t i = 0; i < this->integerPart.length(); i++) {
                if (this->integerPart[i] > other.integerPart[i]) return 1;
                if (this->integerPart[i] < other.integerPart[i]) return -1;
            }
            
            // Pad decimal parts to equal length for comparison
            string thisDecimal = this->decimalPart;
            string otherDecimal = other.decimalPart;
            size_t maxLength = std::max(thisDecimal.length(), otherDecimal.length());
            
            while (thisDecimal.length() < maxLength) thisDecimal += '0';
            while (otherDecimal.length() < maxLength) otherDecimal += '0';
            
            // Compare decimal parts digit by digit
            for (size_t i = 0; i < maxLength; i++) {
                if (thisDecimal[i] > otherDecimal[i]) return 1;
                if (thisDecimal[i] < otherDecimal[i]) return -1;
            }
            
            // If we get here, the numbers are equal
            return 0;
        }

        // Subtraction of absolute values (assumes |this| >= |other|)
		// source: Claude
        StringDouble subtractAbsoluteValues(StringDouble& other) {
            StringDouble resultDbl;
            
            // Pad decimal parts to equal length
            string thisDecimal = this->decimalPart;
            string otherDecimal = other.decimalPart;
            size_t maxDecimalLength = std::max(thisDecimal.length(), otherDecimal.length());
            
            while (thisDecimal.length() < maxDecimalLength) thisDecimal += '0';
            while (otherDecimal.length() < maxDecimalLength) otherDecimal += '0';
            
            // Subtract decimal parts from right to left
            int borrow = 0;

            for (int i = maxDecimalLength - 1; i >= 0; i--) {
                int digit1 = thisDecimal[i] - '0';
                int digit2 = otherDecimal[i] - '0';
                
                int diff = digit1 - digit2 - borrow;
                
                if (diff < 0) {
                    diff += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                
                resultDbl.decimalPart = std::to_string(diff) + resultDbl.decimalPart;
            }
            
            // Subtract integer parts from right to left
            string thisInteger = this->integerPart;
            string otherInteger = other.integerPart;
            
            // Ensure integer parts have the same length
            size_t maxIntLength = std::max(thisInteger.length(), otherInteger.length());
            
            while (thisInteger.length() < maxIntLength) thisInteger = '0' + thisInteger;
            while (otherInteger.length() < maxIntLength) otherInteger = '0' + otherInteger;
            
            for (int i = maxIntLength - 1; i >= 0; i--) {
                int digit1 = thisInteger[i] - '0';
                int digit2 = otherInteger[i] - '0';
                
                int diff = digit1 - digit2 - borrow;
                
                if (diff < 0) {
                    diff += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                
                resultDbl.integerPart = std::to_string(diff) + resultDbl.integerPart;
            }
            
            // Remove leading zeros from integer part (but keep at least one digit)
            while (resultDbl.integerPart.length() > 1 && resultDbl.integerPart[0] == '0') {
                resultDbl.integerPart.erase(0, 1);
            }
            
            // Remove trailing zeros from decimal part
            while (!resultDbl.decimalPart.empty() && resultDbl.decimalPart.back() == '0') {
                resultDbl.decimalPart.pop_back();
            }
            
            // Construct final string representation
            resultDbl.value = resultDbl.integerPart;
            
            if (!resultDbl.decimalPart.empty()) {
                resultDbl.value += "." + resultDbl.decimalPart;
            }
            
            return resultDbl;
        }

		StringDouble add(StringDouble& other) {
            StringDouble result;
            
            // Case 1: Same signs (both positive or both negative)
            if (this->isNegative == other.isNegative) {
                int carry = 0;
                
                // Pad decimal parts to equal length
                string thisDecimal = this->decimalPart;
                string otherDecimal = other.decimalPart;
                size_t maxDecimalLength = std::max(thisDecimal.length(), otherDecimal.length());
                
                while (thisDecimal.length() < maxDecimalLength) thisDecimal += '0';
                while (otherDecimal.length() < maxDecimalLength) otherDecimal += '0';
                
                // Add decimal parts from right to left
                for (int i = maxDecimalLength - 1; i >= 0; i--) {
                    int sum = (thisDecimal[i] - '0') + (otherDecimal[i] - '0') + carry;
                    carry = sum / 10;
                    result.decimalPart = std::to_string(sum % 10) + result.decimalPart;
                }
                
                // Add integer parts from right to left
                string thisInteger = this->integerPart;
                string otherInteger = other.integerPart;
                
                // Ensure integer parts have the same length
                size_t maxIntLength = std::max(thisInteger.length(), otherInteger.length());
                
                while (thisInteger.length() < maxIntLength) thisInteger = '0' + thisInteger;
                while (otherInteger.length() < maxIntLength) otherInteger = '0' + otherInteger;
                
                for (int i = maxIntLength - 1; i >= 0; i--) {
                    int sum = (thisInteger[i] - '0') + (otherInteger[i] - '0') + carry;
                    carry = sum / 10;
                    result.integerPart = std::to_string(sum % 10) + result.integerPart;
                }
                
                // Handle final carry
                if (carry > 0) {
                    result.integerPart = std::to_string(carry) + result.integerPart;
                }
                
                // The result has the same sign as the operands
                result.isNegative = this->isNegative;
            }
            // Case 2: Different signs (one positive, one negative)
            else {
                int comparison = 0;
                StringDouble* larger;
                StringDouble* smaller;
                
                // If this is negative and other is positive
                if (this->isNegative) {
                    // Compare |this| with |other|
                    comparison = this->compareAbsoluteValues(other);
                    if (comparison >= 0) {
                        // |this| >= |other|
                        larger = this;
                        smaller = &other;
                        result.isNegative = true;  // Result is negative
                    } else {
                        // |this| < |other|
                        larger = &other;
                        smaller = this;
                        result.isNegative = false;  // Result is positive
                    }
                }
                // If this is positive and other is negative
                else {
                    // Compare |this| with |other|
                    comparison = this->compareAbsoluteValues(other);
                    if (comparison >= 0) {
                        // |this| >= |other|
                        larger = this;
                        smaller = &other;
                        result.isNegative = false;  // Result is positive
                    } else {
                        // |this| < |other|
                        larger = &other;
                        smaller = this;
                        result.isNegative = true;  // Result is negative
                    }
                }
                
                // Perform subtraction: larger - smaller
                result = larger->subtractAbsoluteValues(*smaller);
                result.isNegative = (comparison == 0) ? false : larger->isNegative;
            }
            
            // Remove leading zeros from integer part (but keep at least one digit)
            while (result.integerPart.length() > 1 && result.integerPart[0] == '0') {
                result.integerPart.erase(0, 1);
            }
            
            // Remove trailing zeros from decimal part
            while (!result.decimalPart.empty() && result.decimalPart.back() == '0') {
                result.decimalPart.pop_back();
            }
            
            // Special case: if the result is zero, it should not be negative
            if (result.integerPart == "0" && result.decimalPart.empty()) {
                result.isNegative = false;
            }
            
            // Construct final string representation
            result.value = result.integerPart;
            
            if (!result.decimalPart.empty()) {
                result.value += "." + result.decimalPart;
            }
            
            // Add sign if negative
            if (result.isNegative) {
                result.value = "-" + result.value;
            }
            
            return result;
		}
		StringDouble operator+(StringDouble& other) {
			return this->add(other);
		}
};

int main() {
	// create a file handle for the test file
	std::ifstream file = std::ifstream(TEST_FILE_PATH);

	// make sure the file was opened successfully
	if (!file.is_open()) {
		// if not, display an error message and return with 1 indicating error
		std::cout << "Failed to open the file: " << TEST_FILE_PATH << "\n";

		// return with 1 indicating error
		return 1;
	}

	string rawDoubleInput;

	// read in the file into the string
	file >> rawDoubleInput;

	StringDouble doubleInput;

	try {
		// try creating the StringDouble
		doubleInput = StringDouble::fromStr(rawDoubleInput);
	} catch (...) {
		// log and return 1 to indicate error
		std::cout << "Failed to parse double: " << rawDoubleInput << "\n";

		return 1;
	}

	// we will always add the dynamic string double to the following
	StringDouble double2 = StringDouble::fromStr("-123.456");

	// add the two string doubles
	StringDouble sum = doubleInput + double2;

	std::cout << "Adding " << doubleInput.value << " and " << double2.value << "\n";

	// print the result
	std::cout << "Sum: " << sum.value << "\n";
}