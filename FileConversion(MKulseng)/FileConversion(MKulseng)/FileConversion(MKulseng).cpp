// CIT 237-03 Spring 2019 
// Project 2
// File Conversion Application
// 
// This program is designed to take input from a file with data deliminated by pipes (|) and change it
// so it is separated by commas (,). If there is a comma in the data, surround the whole data piece 
// with quotes. If there are quotes in the data, surround those quotes with quotes, and surround the 
// whole piece of data with quotes as well.
// 
//
// by Marisha Kulseng
// last modified 3/28/2019 at 8:00 PM

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

char getValidChar(); // input validation. gets a CHARACTER value & converts it to lower case. will not allow characters not used in switch statement.
void printHelp();
// ]
//void menuSelectionTool(char, ifstream &inFile, ofstream &outFile); // switch statement that branches the user input to the correct action
string connectFileStream(ifstream &inFile);  // overloaded functions to connect input/output files to file stream objects
string connectFileStream(ofstream &outFile);
void processFile(ifstream &inFile, ostream &os = cout);  // primary function for file processing
void splitLine(string, string[], int); // function to split the input line into segments
string processQuotes(int, string, int); // function that does the work to process the segments that contain quotes
string printProcessing(bool, bool, string, int, int); // function that prepares the string for the final printing
int getNumQuotes(bool&, bool&, string, int); // function that finds how many quotes are in a given string, and updates bool variables 
											 // keeping track of whether commas or quotes are in the segment


int main() {
	system("title   Converting Files to CSV Format - M. Kulseng"); // console window is titled with the name of the project
	system("color 57"); // a color is assigned to make it easier to read for an extended period

	char validatedCommand; // variable to hold user's converted command 	
	ifstream inFile;
	ofstream outFile;
	bool outputOpen = false, inputOpen = false; // helps to provide intelligent feedback about file open status

	while (true) {
		cout << "Command: ";
		validatedCommand = getValidChar();

		//menuSelectionTool(validatedCommand, inFile, outFile);

		switch (validatedCommand) {
		case 'm': // CI command. Close input file
			inFile.close();  // if no file is open, nothing changes by "closing" it
			if (inputOpen)
				cout << "Input file closed.\n";
			else
				cout << "No input file is open!\n";
			inputOpen = false;
			break;
		case 'n': // CO command. Close output file.
			outFile.close(); // if no file is open, nothing changes by "closing" it
			if (outputOpen)
				cout << "Output file closed.\n";
			else
				cout << "No output file is open!\n";
			outputOpen = false;
			break;
		case 'h': printHelp(); // help
			break;
		case 'i': // specify input file name
			cout << "Enter name of input file: ";
			connectFileStream(inFile);
			inputOpen = true;
			break;
		case 'o': // specify the output file name
			cout << "Enter name of ouptut file: ";
			connectFileStream(outFile);
			outputOpen = true;
			break;
		case 'p': // process data in input file. only runs AFTER I and O. boolean flags?
			processFile(inFile, outFile);
			break;
		case 'q': cout << "Exiting program with status = 0\n";
			system("pause"); exit(0);  // pauses the system so you can review your work if needed before the program quits and closes
			break;
			//case 'r': // reverse it to pipe format!! Not Implemented
				//break;
		default: cout << "\n\tUnknown error! You can only select from this menu: \n";
			printHelp(); // If the user got here, they definitely need the help command! It should not be possible due to the input validation loop!
			break;
		}

	}
	system("pause");
	return 0;
}

char getValidChar() {
	char ch = '\0', ch2 = '\0'; // initialize ch to the null character
	string userInput;
	cin >> ws >> userInput; // allows program to take a string as input so it can determine whether the user entered a single character
	cin.ignore(120, '\n'); // ignores anything after the first "string" is stored in userInput (i.e., if we type "five balloons," it only takes "five")
	ch = userInput[0];     // sets character ch to be the first character in the string we just got from the user

	if (ch >= 'A' && ch <= 'Z')
		ch = 'a' + (ch - 'A'); // converts any uppercase input to lowercase to be compatible with the conditional (and later switch) statement

	if (ch == 'c') { // converts user input with more than one character
		ch2 = userInput[1];
		if (ch2 == 'I' || ch2 == 'i')
			ch = 'm';
		else if (ch2 == 'O' || ch2 == 'o')
			ch = 'n';
		else
			ch = 'z'; // assigns a "bad" character, to trigger the validation loop
	}
	// changed line below to reflect that r is not included
	while (!((ch >= 'm' && ch <= 'q') || ch == 'h' || ch == 'i') || userInput.length() > 2) { // checks if it's a valid character
		cout << "\n\tInvalid entry. " << (userInput.length() > 2 ? "Please enter NO MORE THAN TWO CHARACTERS. " : "") // error statement includes instructions if user entered a string
			<< "Try again (type h for help): ";
		cin >> ws >> userInput;
		cin.ignore(120, '\n');
		ch = userInput[0];

		if (ch >= 'A' && ch <= 'Z')
			ch = 'a' + (ch - 'A'); // converts any uppercase input to lowercase to be compatible with the conditional (and later switch) statement

		if (ch == 'c') { // converts user input with more than one character
			ch2 = userInput[1];
			if (ch2 == 'I' || ch2 == 'i')
				ch = 'm';
			else if (ch2 == 'O' || ch2 == 'o')
				ch = 'n';
			else
				ch = 'z'; // assigns a "bad" character, to trigger the validation loop
		}
	}
	return ch;
} // M N H I O P Q  (HI, MNOPQ) are the commands that can come out of this input. R is not included

void printHelp() { // prints help menu
	cout << "Supported commands: \n\tCI\t\tClose input file.\n"
		<< "\tCO\t\tClose output file."
		<< "\n\th\t\tprint this help text."
		<< "\n\ti\t\topen file for input."
		<< "\n\to\t\topen file for output."
		<< "\n\tp\t\tprocess (output data in \"CSV\" format)."
		//<< "\n\tr\t\treverse (output data in \"pipe\" format).\n" NOT IMPLEMENTED
		<< "\n\tq\t\tquit (end the program).\n";
}

/*void menuSelectionTool(char selection, ifstream &inFile, ofstream &outFile) {
	switch (selection) {

	case 'm': // CI command. Close input file
		inFile.close(); 
		cout << "Input file closed.\n";
		break;
	case 'n': // CO command. Close output file.
		outFile.close(); 
		cout << "Output file closed.\n";
		break;
	case 'h': printHelp(); // help
		break;
	case 'i': // specify input file name
		cout << "Enter name of input file: ";
		connectFileStream(inFile);
		break;
	case 'o': // specify the output file name
		cout << "Enter name of ouptut file: ";
		connectFileStream(outFile);
		break;
	case 'p': // process data in input file. only runs AFTER I and O. boolean flags?
		processFile(inFile, outFile);
		break;
	case 'q': cout << "Exiting program with status = 0\n";
		system("pause"); exit(0);  // pauses the system so you can review your work if needed before the program quits and closes
		break;
		//case 'r': // reverse it to pipe format!! Not Implemented
			//break;
	default: cout << "\n\tUnknown error! You can only select from this menu: \n";
		printHelp(); // If the user got here, they definitely need the help command! It should not be possible due to the input validation loop!
		break;
	}
}*/

string connectFileStream(ifstream &inFile)
{
	string fileName;

	getline(cin >> ws, fileName);
	inFile.open(fileName);

	while (inFile.fail()) {
		cout << "\tFailed to open \"" << fileName << "\" for READING the data.\n"
			<< "\tPlease make sure the file is located in the appropriate folder"
			<< " and\n\tit is not misspelled. "
			<< "\n\tDon't forget to include the filename extension: ";
		getline(cin >> ws, fileName);
		inFile.open(fileName);
	}
	return fileName;
}

string connectFileStream(ofstream &outFile)
{
	string fileName;
	getline(cin >> ws, fileName);
	outFile.open(fileName);
	while (outFile.fail()) {
		cout << "\n\tFailed to open \"" << fileName << "\" for WRITING the data.\n"
			<< "\tPlease make sure the file name does not contain illegal characters\n"
			<< "\tand it is not currently used by another application."
			<< "\n\tDon't forget to include the filename extension: ";

		getline(cin >> ws, fileName);
		outFile.open(fileName);
	}
	return fileName;
}

/*what do we need to do to process the file?
			1. get line
			2. check for where the pipes are in that line
			3. dynamically allocate memory for array of strings of size numPipes + 1
			4. separate line into segments by pipe & store in that array
			5. check each segment to see if there is a(,) or a(")
			6. if a segment has ("), go through char by char and add (") whenever (") appears
			7. In segments that have(") OR(, ), surround with(")
			8. delete dyamically allocated string array before going to the next line
*/

void processFile(ifstream &inFile, ostream &os) {
	string *segmentArr = nullptr;
	string currentLine;
	int numPipes, pipeIndex, lineLength;

	while (getline(inFile >> ws, currentLine)) { // step 1: get the line
		// set accumulators to 0  & initialize lineLength
		numPipes = 0;
		pipeIndex = 0;
		lineLength = currentLine.length();
		cout << "\nInput data = " <<currentLine;

		// step 2: count how many pipes are in the line
		for (int i = 0; i < lineLength + 1; i++) { // sets up dynamically allocated array sizes
			if (currentLine[i] == '|')
				numPipes++;
		}

		// step 3: dynamically allocate memory for array of strings of size numPipes + 1
		segmentArr = new string[numPipes + 1];

		// step 4: separate line into segments by pipe & store in that array
		splitLine(currentLine, segmentArr, numPipes + 1);

		// step 5: check each segment to see if there is a (,) or a (")
		for (int y = 0; y <= numPipes; y++) { // does this for each segment on this line one at a time
			bool hasQuotes = false, hasCommas = false;
			string toPrint = ""; // string that gets printed in the final version, initialized as empty string
			int numQuotes = 0, segLength = segmentArr[y].length();

			// this function will check each segment to see if there is a(,) or a("), and returns the number of quotes found
			numQuotes = getNumQuotes(hasCommas, hasQuotes, segmentArr[y], segLength);

			// this function consolidates the components of the string and changes them as needed
			toPrint = printProcessing(hasQuotes, hasCommas, segmentArr[y], segLength, numQuotes);

			os << toPrint << ",";	// write the final, altered segment to the file
		}

		os << endl; // end the current line in the file and start anew

		delete[] segmentArr; // delete dynamically allocated memory (Step 8 above)
		segmentArr = nullptr;
	}
	cout << "\nEnd of file encountered.\n";
}

void splitLine(string line, string segmentArr[], int size) { // size is numPipes + 1
	int pipeIndex = 0;
	for (int x = 0; x < size; x++) {
		segmentArr[x] = line.substr(pipeIndex, line.find('|', pipeIndex) - pipeIndex); // store each segment in array
		if (x != size - 1)
			pipeIndex = line.find('|', pipeIndex) + 1; // as long as we haven't reached the last pipe, update pipeIndex
	}
}

string processQuotes(int segLength, string segment, int numQuotes) {  // edit strings that have quotes
		// dynamically allocate memory for an array of quote index positions
	int *quoteIndex = new int[numQuotes];
	int quoteCounter = 0; // keeps track of how many quotes have been accounted for
	string toPrint = "";
	int start = 0;

	for (int z = 0; z < segLength; z++) {
		if (segment[z] == '"' && quoteCounter < numQuotes) { // checks to see if there are quotes in the segment & saves the index where they occur
			quoteIndex[quoteCounter] = z;
			quoteCounter++;
		}
	}

	// Next prepare lines with quotes for printing.
	// Step 6: if a segment has ("), go through char by char and add (") whenever (") appears
	for (int k = 0; k < numQuotes; k++) {
		toPrint += segment.substr(start, quoteIndex[k] - start) + "\""; // adds quotes to the end of each substring of the segment
		if (k < numQuotes /*- 1*/)
			start = quoteIndex[k]; // resets start so it's at the index this quote appears

		if (k == numQuotes - 1) // if it's the last iteration of this loop, add the last segment that had no quote to the end
			toPrint += segment.substr(start, segment.length() - start);
	}

	delete[] quoteIndex;   // delete dynamically allocated memory
	quoteIndex = nullptr;
	return toPrint;
}

string printProcessing(bool hasQuotes, bool hasCommas, string segment, int segLength, int numQuotes) {
	string toPrint = "";
	if (hasQuotes)
		toPrint = processQuotes(segLength, segment, numQuotes);

	if (!hasCommas && !hasQuotes) // if there are no commas or quotes, just print the segment as it is
		toPrint = segment;

	// step 7: in segments that have(") OR(, ), surround with(")
	else if (hasCommas || hasQuotes) { // if there are any commas or quotes, as found above, adjust the string as needed before printing
		if (hasCommas && !hasQuotes)
			toPrint = segment; //if we haven't done any quote processing, assign the segment to the toPrint variable
		toPrint = "\"" + toPrint + "\""; // add quotes around the whole segment if quotes or commas appeared in the segment
	}
	else
		toPrint = "***ERROR***"; // should never print

	return toPrint;
}

int getNumQuotes(bool &hasCommas, bool &hasQuotes, string segment, int segLength) {
	int numQuotes = 0;
	for (int z = 0; z < segLength; z++) { //check each segment to see if there is a(, ) or a("")
		if (segment[z] == ',') // checks to see if there's a comma in the segment
			hasCommas = true;
		if (segment[z] == '"') { // checks to see if there are quotes in the segment
			hasQuotes = true;
			numQuotes++;
		}
	}
	return numQuotes;
}