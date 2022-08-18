#include <iostream>   // For input and output
#include <fstream>    // For file input and output
#include <cassert>    // To validate if file is open via assert statement
#include <cctype>     // For tolower()
#include <algorithm>  // For find(), which searches within a vector
#include <string>    
#include <vector>
using namespace std;  // To avoid prefacing cin/cout with std::


class Record {
    public:
        string date, song, artist;
        int rank, lastWeek, peakRank, weeksOnBoard;
};


void readFile(string fileName, vector <Record>& billboardRecords) 
{
    string recordLine;
  
    ifstream inStream;                  // Declare input stream for reading
    inStream.open(fileName);            // Open file
    assert(inStream.fail() == false);   // Ensure file opened

    // read and ignore first line containing header with column names
    getline(inStream, recordLine);
    
    // read in each line of the file
    while (getline(inStream, recordLine)) {
        Record record;
        
        // separate the line by commas
        int posComma1 = recordLine.find(',', 0);                           // find(sub-string to find, initial position to begin search)
        record.date = recordLine.substr(0, posComma1);                     // substr(position of the first character to be copied, length of sub-string)
        
        int posComma2 = recordLine.find(',', posComma1+1);
        record.rank = stoi(recordLine.substr(posComma1+1, posComma2-posComma1-1));      // stoi() casts as int
        
        int posComma3 = recordLine.find(',', posComma2+1);
        record.song = recordLine.substr(posComma2+1, posComma3-posComma2-1);
          
        int posComma4 = recordLine.find(',', posComma3+1);
        record.artist = recordLine.substr(posComma3+1, posComma4-posComma3-1);

        int posComma5 = recordLine.find(',', posComma4+1);
        string columnVal = recordLine.substr(posComma4+1, posComma5-posComma4-1);
        record.lastWeek = (columnVal != "") ? stoi(columnVal) : 0;                     // some rows don't have values for lastWeek column; if so, lastWeek = 0

        int posComma6 = recordLine.find(',', posComma5+1);
        record.peakRank = stoi(recordLine.substr(posComma5+1, posComma6-posComma5-1));

        record.weeksOnBoard = stoi(recordLine.substr(posComma6+1));       // default length is all characters until end of string

        billboardRecords.push_back(record);
    }
}


void displayMenuOptions(const vector <Record>& billboardRecords, int fileOption);


void menuOption1(const vector <Record>& billboardRecords) {
    
    // find the number of unique songs represented in the dataset
    vector <string> uniqueSongs;
    for (int i = 0; i < billboardRecords.size(); i++) {
        string song = billboardRecords.at(i).song;
        // Add the song name to the vector if it is not found in the vector already
        if (find(uniqueSongs.begin(), uniqueSongs.end(), song) == uniqueSongs.end()) {
            uniqueSongs.push_back(song);
        }
    }
    
    cout << "Total number of records: "
         << billboardRecords.size() << endl
         << "Number of unique songs represented in the dataset: "
         << uniqueSongs.size() << endl;
}


// song with the most #1 occurrences for a given decade or year
void menuOption2(const vector <Record>& filteredRecords) {
    vector <string> songsVector, artistsVector, maxNumOneSongs, maxNumOneArtists;
    vector <int> totalNumOneOccurencesVector;
    int maxNumOneOccurences = 0;

    // for each record in given year or decade...
    for (int i = 0; i < filteredRecords.size(); i++) {

        // Parallel vectors
        songsVector.push_back(filteredRecords.at(i).song);
        artistsVector.push_back(filteredRecords.at(i).artist);

        // check how many times each song appears at #1
        int totalNumOneOccurences = 0;    // reset for each song
        for (int j = 0; j < filteredRecords.size(); j++) {
            if (filteredRecords.at(i).song == filteredRecords.at(j).song) {
                totalNumOneOccurences += 1;
            }
        }
        totalNumOneOccurencesVector.push_back(totalNumOneOccurences);

        // compare to find the record with the greatest climb
        if (totalNumOneOccurences >= maxNumOneOccurences) {
            maxNumOneOccurences = totalNumOneOccurences;
        }
    }

    // find songs/artists with matching max total of occurences
    for (int i = 0; i < songsVector.size(); i++) {
        if (totalNumOneOccurencesVector.at(i) == maxNumOneOccurences) {
            // if this song is in maxNumOneSongs vector, it's already been counted for tie, otherwise..
            if (find(maxNumOneSongs.begin(), maxNumOneSongs.end(), songsVector.at(i)) == maxNumOneSongs.end()) {
                // add to vector with songs/artists that tied for highest #1 occurence
                maxNumOneSongs.push_back(songsVector.at(i));
                maxNumOneArtists.push_back(artistsVector.at(i));
            }
        }
    }
  
    cout << "\n\nThe song(s) with the most #1 occurrences for selected time period is:  \n";
    for (int i = 0; i < maxNumOneSongs.size(); i++) {
        cout << "\n        Song Title: " << maxNumOneSongs.at(i) << "\n"
             << "        Artist: " << maxNumOneArtists.at(i) << "\n"
             << "        # of occurrences: " << maxNumOneOccurences << "\n";
    }
}


// song with the highest number of weeks on the charts for a given decade or year
void menuOption3(const vector <Record>& filteredRecords) {
    int recordWeeksOnBoard;
    string song, artist;
    vector <int> weeksOnBoardVector;
  
    // check for the highest number of weeks on board
    int maxWeeksOnBoard = 0;
    for (int i = 0; i < filteredRecords.size(); i++) {
        recordWeeksOnBoard = filteredRecords.at(i).weeksOnBoard;
        if (recordWeeksOnBoard > maxWeeksOnBoard) {
            maxWeeksOnBoard = recordWeeksOnBoard;
            song = filteredRecords.at(i).song;
            artist = filteredRecords.at(i).artist;
        }
    }
  
    cout << "\nThe song with the highest number of weeks on the charts is: \n"
         << "        " << song << " by " << artist << " with " << maxWeeksOnBoard << " weeks on the chart.\n";
}


// song with the greatest weekly climb on the charts given a decade or year
void menuOption4(const vector <Record>& filteredRecords) {
    vector <string> songsVector, artistsVector, datesVector;
    vector <int> rankVector, lastWeekVector, climbVector;
    int greatestClimb = 0;
  
    // for each record in given year or decade...
    for (int i = 0; i < filteredRecords.size(); i++) {

        // organize required columns into parallel vectors
        datesVector.push_back(filteredRecords.at(i).date);
        rankVector.push_back(filteredRecords.at(i).rank);
        songsVector.push_back(filteredRecords.at(i).song);
        artistsVector.push_back(filteredRecords.at(i).artist);
        lastWeekVector.push_back(filteredRecords.at(i).lastWeek);
        
        // calculate climb from previous week rank to current week rank
        int thisClimb = 0;      // reset
        thisClimb = filteredRecords.at(i).lastWeek - filteredRecords.at(i).rank;
        climbVector.push_back(thisClimb);

        // compare to find the record with the greatest climb
        if (thisClimb >= 0 && thisClimb >= greatestClimb) {     // >= 0 excludes negative climbs (generated due to blank lastWeek values) from comparison
            greatestClimb = thisClimb;
        }
    }
  
    cout << "\n\nThe song(s) with the greatest climb from previous week to current week position: \n";
    for (int i = 0; i < songsVector.size(); i++) {
        // find records with tie for greatest weekly climb, display all
        if (climbVector.at(i) == greatestClimb) {
            cout << "\n"
                 << "        Song Title: " << songsVector.at(i) << "\n"
                 << "        Artist: " << artistsVector.at(i) << "\n"
                 << "        Week of: " << datesVector.at(i) << "\n"
                 << "        Previous Week Position: " << lastWeekVector.at(i) << "\n"
                 << "        Current Week Position: " << rankVector.at(i) << "\n"
                 << "        Difference of " << greatestClimb << " between previous week and current week position\n";
        }
    }
}


// linear search on artist column to find #1 songs containing provided input text
void menuOption5(const vector<Record>& billboardRecords) {
    vector<Record> foundArtists;
    int recordRank;
    string artist, searchInput;
  
    cout << "Enter search text to retrieve first 10 records of #1 songs that match by artist name: " << endl;
    cin.ignore();
    getline(cin, searchInput);       // instead of single word, read in characters until '\n' is encountered
  
    // lowercase each character of user inputted search value
    for(int charIndex = 0; charIndex < searchInput.size(); charIndex++) {
        searchInput.at(charIndex) = tolower(searchInput.at(charIndex));
    }

    // find records where user search input is found in the artist name
    for (int recordNum = 0; recordNum < billboardRecords.size(); recordNum++) {

        // ignore records which are/were not #1 on the charts
        if (billboardRecords.at(recordNum).rank != 1 || billboardRecords.at(recordNum).peakRank != 1) {
            continue;
        }

        // lowercase each character of artist name
        artist = billboardRecords.at(recordNum).artist;
        for(int charIndex = 0; charIndex < artist.size(); charIndex++) {
            artist.at(charIndex) = tolower(artist.at(charIndex));
        }

        // Check if artist name contains the search input
        if (artist.find(searchInput) != string::npos) {
            foundArtists.push_back(billboardRecords.at(recordNum));
        }
    }

    // Check if no matching artists were found
    if (foundArtists.size() == 0) {
        cout << "No matching artists were found." << endl;
        return;
    }

    // Display the first 10 #1 records found that match the artist name
    cout << "Retrieval of first 10 #1 records found based upon search by artist name: " << endl;
    for (int num = 0; num < 10; num++) {
        if (num >= foundArtists.size()) {
            break;
        }
        cout << num+1 << ") Song: " << foundArtists.at(num).song << endl
             << "    Artist: " << foundArtists.at(num).artist << endl
             << "    Week of: " << foundArtists.at(num).date << endl
             << "    Week Position: " << foundArtists.at(num).rank << endl;
    }
}


vector <Record> timeframeSelection(const vector <Record>& billboardRecords, int fileOption, int menuOption) {
    char decadeOrYear;
    int inputDecade, inputYear, recordYear, recordRank;
    vector <Record> filteredRecords;

    while (true) {
        cout << "Enter D to select a decade or Y to select a year."
             << "\nYour choice --> ";
      
        cin >> decadeOrYear;
        decadeOrYear = tolower(decadeOrYear);

        if (decadeOrYear != 'd' && decadeOrYear != 'y') {
            cout << "\nInvalid entry. Try again.\n";
            continue;
        }

        else {
            if (decadeOrYear == 'd') {
                cout << "Enter the decade you would like to choose, as a multiple of 10 (e.g. 2010).\n";
                cin >> inputDecade;
                inputDecade = (inputDecade - (inputDecade % 10));      // adjust input that's not a multiple of 10 
            }
            else if (decadeOrYear == 'y') {
                cout << "Enter the year you would like to choose (e.g. 2001). \n";
                cin >> inputYear;
            }

            for (int i = 0; i < billboardRecords.size(); i++) {
                // MM/DD/YYYY --> YYYY, convert to integer with stoi()
                recordYear = stoi(billboardRecords.at(i).date.substr(billboardRecords.at(i).date.size()-4, 4));
                recordRank = billboardRecords.at(i).rank;

                // for menu option 2, compare using first 3 integers of YYYY and filter songs at rank #1
                if (decadeOrYear == 'd' && menuOption == 2 && (recordYear / 10 == inputDecade / 10) && recordRank == 1) {
                    filteredRecords.push_back(billboardRecords.at(i));
                }
                else if (decadeOrYear == 'y' && menuOption == 2 && (recordYear == inputYear) && recordRank == 1) {
                    filteredRecords.push_back(billboardRecords.at(i));
                }
                
                // for menu options 3 and 4, only compare using first 3 integers of YYYY
                else if (decadeOrYear == 'd' && (menuOption == 3 || menuOption == 4) && (recordYear / 10 == inputDecade / 10)) {
                    filteredRecords.push_back(billboardRecords.at(i));
                }
                else if (decadeOrYear == 'y' && (menuOption == 3 || menuOption == 4) && (recordYear == inputYear)) {
                    filteredRecords.push_back(billboardRecords.at(i));
                }
            }

            // Check if there are songs found in the timeframe, e.g. searching for songs from 2001 in the 1960-1980 datafile will not yield results
            if (filteredRecords.size() == 0) {
                cout << "No songs found in that timeframe. Be sure that the timeframe you enter is within the one covered by the dataset chosen.";
                displayMenuOptions(billboardRecords, fileOption);
            }
            else {
                return filteredRecords;
            };
        }
    }
}


void displayMenuOptions(const vector <Record>& billboardRecords, int fileOption) {
    int menuOption = 0;

    while (true) {
        cout << "\n\nSelect a menu option: \n"
             << "   1. Display general information about the dataset\n"
             << "   2. Display the Billboard info of the song with the most #1 occurrences for a given decade or year\n"
             << "   3. Display the Billboard info for the most long-lasting song on the charts for a given decade or year\n"
             << "   4. Display the Billboard info for the song with the greatest weekly climb on the charts given a decade or year\n"
             << "   5. Display the Billboard info for records that match the user input for artist search\n"
             << "   6. Exit\n"
             << "Your choice --> ";
        cin >> menuOption;
    
        switch(menuOption) {
            case(1): {
                menuOption1(billboardRecords);
                break;
            }
            case(2): {
                vector <Record> filteredRecords = timeframeSelection(billboardRecords, fileOption, 2);
                menuOption2(filteredRecords);
                break;
            }
            case(3): {
                vector <Record> filteredRecords = timeframeSelection(billboardRecords, fileOption, 3);
                menuOption3(filteredRecords);
                break;
            }
            case(4): {
                vector <Record> filteredRecords = timeframeSelection(billboardRecords, fileOption, 4);
                menuOption4(filteredRecords);
                break;
            }
            case(5): {
                menuOption5(billboardRecords);
                break;
            }
            case(6): {
                exit(0);
                break;
            }
            default: {
                cout << "\nInvalid value.  "
                     << "Please re-enter a value from the menu options below.";
                break;
            }
        }
    }
}


void displayFileOptions() 
{
    int fileOption;
    string fileName;
    
    cout << "Billboard Hot 100: Analysis \n"
         << " \n"
         << "This program will analyze data from weekly Billboard Hot 100 charts spanning 1960–2020.\n\n"
         << "Select file option:\n"
         << "   1. To analyze top 50 songs from weekly 1960 - 1980 charts\n"
         << "   2. To analyze top 50 songs from weekly 1981 - 2000 charts\n"
         << "   3. To analyze top 50 songs from weekly 2001 - 2020 charts\n"
         << "Your choice --> ";
  
    cin >> fileOption;
  
    // read in relevant file
    switch(fileOption) {
        case(1): {
            fileName = "charts_top50_1960_1980.csv";
            break;
        }
        case(2): {
            fileName = "charts_top50_1981_2000.csv";
            break;
        }
        case(3): {
            fileName = "charts_top50_2001_2020.csv";
            break;
        }
        default: {
            cout << "\nInvalid value. Exiting Program.";
            exit(-1);   // error
            break;
        }
    }

    vector <Record> billboardRecords;
    readFile(fileName, billboardRecords);
  
    displayMenuOptions(billboardRecords, fileOption);
}


int main() 
{
    displayFileOptions();
    return 0;
}
