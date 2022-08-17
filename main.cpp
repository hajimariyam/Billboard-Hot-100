#include <iostream>   // For input and output
#include <fstream>    // For file input and output
#include <cassert>    // To validate if file is open
#include <cctype>     // For tolower()
#include <algorithm>  // For find()
#include <string>    
#include <vector>
using namespace std;  // To avoid prefacing cin/cout with std::


class Row {
    public:
        string date, song, artist;
        string rank, lastWeek, peakRank, weeksOnBoard;
};


void readFile(string fileName, vector <Row>& chart) {

    string line;
  
    ifstream inStream;                  // Declare input stream for reading
    inStream.open(fileName);            // Open file
    assert(inStream.fail() == false);   // Ensure file opened
    
    while (getline(inStream, line)) {
        Row row;
        
        int num1 = line.find(',', 0);
        row.date = line.substr(0, num1);
        
        int num2 = line.find(',', num1+1);
        row.rank = line.substr(num1+1, num2-num1-1);
        
        int num3 = line.find(',', num2+1);
        row.song = line.substr(num2+1, num3-num2-1);
          
        int num4 = line.find(',', num3+1);
        row.artist = line.substr(num3+1, num4-num3-1);

        int num5 = line.find(',', num4+1);
        row.lastWeek = line.substr(num4+1, num5-num4-1);

        int num6 = line.find(',', num5+1);
        row.peakRank = line.substr(num5+1, num6-num5-1);

        int num7 = line.find(',', num6+1);
        row.weeksOnBoard = line.substr(num6+1, num7-num6-1);

        chart.push_back(row);
    }
  
    // remove first Row object containing column names
    chart.erase(chart.begin());
}


void displayMenuOptions(const vector <Row>& chart, int fileOption);


void menuOption1(const vector <Row>& chart) {
    vector <string> uniqueSongs;
  
    for (int i = 0; i < chart.size(); i++) {
        if (find(uniqueSongs.begin(), uniqueSongs.end(), chart.at(i).song) == uniqueSongs.end()) {
            uniqueSongs.push_back(chart.at(i).song);
        }
    }
    
    cout << "Total number of rows: "
         << chart.size() << endl
         << "Number of unique songs represented in the dataset: "
         << uniqueSongs.size() << endl;
}


// song with the most #1 occurrences for a given decade or year
void menuOption2(const vector <Row>& chart, int fileOption) {
    char decadeOrYear;
    int inputDecade, inputYear, rowYear, rowRank;
    int totalOccurences = 0;
    vector <Row> decadeVector;
    vector <string> songsVector, artistsVector, highestSongs, highestArtists;
    vector <int> totalOccurencesVector;

    while (true) {
        cout << "Enter D to select a decade or Y to select a year."
             << "\nYour choice --> ";
      
        cin >> decadeOrYear;
        decadeOrYear = tolower(decadeOrYear);

        if (decadeOrYear == 'd') {
            cout << "Enter the decade you would like to choose, "
                 << "as a multiple of 10 (e.g. 2010).\n";
            cin >> inputDecade;
            inputDecade = (inputDecade - (inputDecade % 10));      // adjust input that's not a multiple of 10

            for (int i = 0; i < chart.size(); i++) {
                // MM/DD/YYYY --> YYYY, convert to integer with stoi()
                rowYear = stoi(chart.at(i).date.substr(chart.at(i).date.size()-4, 4));
                rowRank = stoi(chart.at(i).rank);
                // compare using first 3 integer of YYYY, filter songs at rank #1
                if ((rowYear / 10 == inputDecade / 10) && rowRank == 1) {
                    decadeVector.push_back(chart.at(i));  }
            }

            // No matches found in data for inputted year
            if (decadeVector.size() == 0) {
                cout << "No songs found in that timeframe. Be sure that the timeframe "
                     << "you enter is within the one covered by the dataset chosen.";
                displayMenuOptions(chart, fileOption);
            }
            break;
        }
          
        else if (decadeOrYear == 'y') {
            cout << "Enter the year you would like to choose (e.g. 2001). \n";
            cin >> inputYear;

            for (int i = 0; i < chart.size(); i++) {
                rowYear = stoi(chart.at(i).date.substr(chart.at(i).date.size()-4, 4));
                rowRank = stoi(chart.at(i).rank);
                if ((rowYear == inputYear) && rowRank == 1) {
                    decadeVector.push_back(chart.at(i)); }
            }
            // No matches found in data for inputted year
            if (decadeVector.size() == 0) {
                cout << "No songs found in that timeframe. Be sure that the timeframe "
                     << "you enter is within the one covered by the dataset chosen.";
                displayMenuOptions(chart, fileOption);
            }
            break;
        }
          
        else {
            cout << "\nInvalid entry. Try again.\n";
            continue;
        }
    }

    for (int i = 0; i < decadeVector.size(); i++) {
        songsVector.push_back(decadeVector.at(i).song);
        artistsVector.push_back(decadeVector.at(i).artist);
        // check how many times given song appears at #1
        totalOccurences = 0;
        for (int j = 0; j < decadeVector.size(); j++) {
            if (decadeVector.at(i).song == decadeVector.at(j).song) {
                totalOccurences += 1;
            }
        }
        totalOccurencesVector.push_back(totalOccurences);
    }

    totalOccurences = 0;
  
    for (int i = 0; i < totalOccurencesVector.size(); i++) {
        // find max total of occurences
        if (totalOccurencesVector.at(i) >= totalOccurences) {
            totalOccurences = totalOccurencesVector.at(i);
        }
    }

    for (int i = 0; i < songsVector.size(); i++) {
        // find songs/artists with matching max total of occurences
        if (totalOccurencesVector.at(i) == totalOccurences) {
            if (find(highestSongs.begin(), highestSongs.end(), songsVector.at(i)) == highestSongs.end()) {
                highestSongs.push_back(songsVector.at(i));
                highestArtists.push_back(artistsVector.at(i));
            }
        }
    }
  
    cout << "\n\nThe song(s) with the most #1 occurrences for selected time period is:  \n";
    for (int i = 0; i < highestSongs.size(); i++) {
        cout << "        Song Title: " << highestSongs.at(i) << "\n"
             << "        Artist: " << highestArtists.at(i) << "\n"
             << "        # of occurrences: " << totalOccurences << "\n";
    }
}


// most long-lasting song on the charts for a given decade or year
void menuOption3(const vector <Row>& chart, int fileOption) {
    char decadeOrYear;
    int inputDecade, inputYear, rowYear, rowWeeksOnBoard;
    int highestWeeksOnBoard = 0;
    string song, artist;
    vector <Row> decadeVector;
    vector <int> weeksOnBoardVector;

    while (true) {
        cout << "Enter D to select a decade or Y to select a year."
             << "\nYour choice --> ";
      
        cin >> decadeOrYear;
        decadeOrYear = tolower(decadeOrYear);

        if (decadeOrYear == 'd') {
            cout << "Enter the decade you would like to choose, "
                 << "as a multiple of 10 (e.g. 2010).\n";
            cin >> inputDecade;
            inputDecade = (inputDecade - (inputDecade % 10));      // adjust input that's not a multiple of 10

            for (int i = 0; i < chart.size(); i++) {
                // MM/DD/YYYY --> YYYY, convert to integer with stoi()
                rowYear = stoi(chart.at(i).date.substr(chart.at(i).date.size()-4, 4));
                // compare using first 3 integer of YYYY
                if (rowYear / 10 == inputDecade / 10) {
                    decadeVector.push_back(chart.at(i));  }
            }
            // No matches found in data for inputted year
            if (decadeVector.size() == 0) {
                cout << "No songs found in that timeframe. Be sure that the timeframe "
                     << "you enter is within the one covered by the dataset chosen.";
                displayMenuOptions(chart, fileOption);
            }
            break;
        }
          
        else if (decadeOrYear == 'y') {
            cout << "Enter the year you would like to choose (e.g. 2001). \n";
            cin >> inputYear;

            for (int i = 0; i < chart.size(); i++) {
                rowYear = stoi(chart.at(i).date.substr(chart.at(i).date.size()-4, 4));
                if (rowYear == inputYear) {
                    decadeVector.push_back(chart.at(i)); }
            }

            // No matches found in data for inputted year
            if (decadeVector.size() == 0) {
                cout << "No songs found in that timeframe. Be sure that the timeframe "
                     << "you enter is within the one covered by the dataset chosen.";
                displayMenuOptions(chart, fileOption);
            }
            break;
        }
          
        else {
            cout << "\nInvalid entry. Try again.\n";
            continue;
        }
    }
  
    for (int i = 0; i < decadeVector.size(); i++) {
        // check for the highest number of weeks on board
        rowWeeksOnBoard = stoi(decadeVector.at(i).weeksOnBoard);
        if (rowWeeksOnBoard > highestWeeksOnBoard) {
            highestWeeksOnBoard = rowWeeksOnBoard;
            song = decadeVector.at(i).song;
            artist = decadeVector.at(i).artist;
        }
    }
  
    cout << "\nThe song with the highest number of weeks on the charts is: \n"
         << "        " << song << " by " << artist << " with " << highestWeeksOnBoard << " weeks on the chart.\n";
}


// song with the greatest weekly climb on the charts given a decade or year
void menuOption4(const vector <Row>& chart, int fileOption) {
    char decadeOrYear;
    int inputDecade, inputYear, rowYear, rowRank, rowLastWeek;
    int climb = 0;
    int highestClimb = 0;
    vector <Row> decadeVector;
    vector <string> songsVector, artistsVector, datesVector;
    vector <int> rankVector, lastWeekVector, climbVector;

    while (true) {
        cout << "Enter D to select a decade or Y to select a year."
             << "\nYour choice --> ";
      
        cin >> decadeOrYear;
        decadeOrYear = tolower(decadeOrYear);

        if (decadeOrYear == 'd') {
            cout << "Enter the decade you would like to choose, "
                 << "as a multiple of 10 (e.g. 2010).\n";
            cin >> inputDecade;
            inputDecade = (inputDecade - (inputDecade % 10));      // adjust input that's not a multiple of 10

            for (int i = 0; i < chart.size(); i++) {
                // MM/DD/YYYY --> YYYY, convert to integer with stoi()
                rowYear = stoi(chart.at(i).date.substr(chart.at(i).date.size()-4, 4));
                // compare using first 3 integer of YYYY, filter songs at rank #1
                if (rowYear / 10 == inputDecade / 10) {
                    decadeVector.push_back(chart.at(i));  }
            }

            // No matches found in data for inputted year
            if (decadeVector.size() == 0) {
                cout << "No songs found in that timeframe. Be sure that the timeframe "
                     << "you enter is within the one covered by the dataset chosen.";
                displayMenuOptions(chart, fileOption);
            }
            break;
        }
          
        else if (decadeOrYear == 'y') {
            cout << "Enter the year you would like to choose (e.g. 2001). \n";
            cin >> inputYear;

            for (int i = 0; i < chart.size(); i++) {
                rowYear = stoi(chart.at(i).date.substr(chart.at(i).date.size()-4, 4));
                if (rowYear == inputYear) {
                    decadeVector.push_back(chart.at(i)); }
            }
            // No matches found in data for inputted year
            if (decadeVector.size() == 0) {
                cout << "No songs found in that timeframe. Be sure that the timeframe "
                     << "you enter is within the one covered by the dataset chosen.";
                displayMenuOptions(chart, fileOption);
            }
            break;
        }
          
        else {
            cout << "\nInvalid entry. Try again.\n";
            continue;
        }
    }
  
    for (int i = 0; i < decadeVector.size(); i++) {
        if (decadeVector.at(i).rank != "") {
            rowRank = stoi(decadeVector.at(i).rank);
        }
        if (decadeVector.at(i).lastWeek != "") {
            rowLastWeek = stoi(decadeVector.at(i).lastWeek);
        }

        songsVector.push_back(decadeVector.at(i).song);
        artistsVector.push_back(decadeVector.at(i).artist);
        rankVector.push_back(rowRank);
        lastWeekVector.push_back(rowLastWeek);
        datesVector.push_back(decadeVector.at(i).date);
        climb = rowLastWeek - rowRank;
        climbVector.push_back(climb);
    }
  
    for (int i = 0; i < climbVector.size(); i++) {
        // find highest climb
        if (climbVector.at(i) >= highestClimb) {
            highestClimb = climbVector.at(i);
        }
    }

    cout << "\n\nThe song(s) with the greatest climb from previous week to current week position: \n";
    for (int i = 0; i < songsVector.size(); i++) {
        // find rows with matching climb
        if (climbVector.at(i) == highestClimb) {
            cout << "        Song Title: " << songsVector.at(i) << "\n"
                 << "        Artist: " << artistsVector.at(i) << "\n"
                 << "        Week of: " << datesVector.at(i) << "\n"
                 << "        Previous Week Position: " << lastWeekVector.at(i) << "\n"
                 << "        Current Week Position: " << rankVector.at(i) << "\n"
                 << "        Difference of " << highestClimb << " between previous week and current week position\n";
        }
    }
}


void menuOption5(const vector<Row>& chart) {

    vector<Row> foundArtists;
    int rowRank;
    string artist, searchInput;
  
    cout << "Enter search text to retrieve first 10 records of #1 songs that match by artist name: " << endl;
    cin.ignore();
    getline(cin, searchInput);
  
    // make search value lowercase
    for(int i = 0; i < searchInput.size(); i++) {
        searchInput.at(i) = tolower(searchInput.at(i));
    }

    // find artist names that match search input
    for (int i = 0; i < chart.size(); i++) {
        
        rowRank = stoi(chart.at(i).rank);
        if (rowRank != 1) {
            continue;
        }

        artist = chart.at(i).artist;
        // make artist name lowercase
        for(int j = 0; j <  artist.size(); j++) {
            artist.at(j) = tolower(artist.at(j));
        }

        // Check if artist name contains the search input
        if (artist.find(searchInput) != string::npos) {
            foundArtists.push_back(chart.at(i));
        }
    }

    // Check if no matching artists were found
    if(foundArtists.size() == 0) {
        cout << "No matching artists were found." << endl;
        return;
    }

    // Display the first 10 #1 records found that match the artist name
    cout << "Retrieval of first 10 #1 records found based upon search by artist name: " << endl;
    for (int x = 0; x < 10; x++) {
        if(x >= foundArtists.size()) {
            break;
        }
        cout << x+1 << ") Song: " << foundArtists.at(x).song << endl
             << "    Artist: " << foundArtists.at(x).artist << endl
             << "    Week of: " << foundArtists.at(x).date << endl
             << "    Week Position: " << foundArtists.at(x).rank << endl;
    }
   
}


void displayMenuOptions(const vector <Row>& chart, int fileOption) {

    while (true) {
        int menuOption;
    
        cout << "\n\nSelect a menu option: \n"
             << "   1. Display overall information about the data\n"
             << "   2. Display the Billboard info of the song with the "
             <<     "most #1 occurrences for a given decade or year\n"
             << "   3. Display the Billboard info for the most long-lasting "
             <<     "song on the charts for a given decade or year\n"
             << "   4. Display the Billboard info for the song with the greatest "
             <<     "weekly climb on the charts given a decade or year\n"
             << "   5. Artist Search - Return the Billboard info for records "
                    "that matches user input for artist search\n"
             << "   6. Exit\n"
             << "Your choice --> ";
    
        cin >> menuOption;
    
        switch(menuOption) {
            case(1): {
                menuOption1(chart);
                break;
            }
            case(2): {
                menuOption2(chart, fileOption);
                break;
            }
            case(3): {
                menuOption3(chart, fileOption);
                break;
            }
            case(4): {
                menuOption4(chart, fileOption);
                break;
            }
            case(5): {
                menuOption5(chart);
                break;
            }
            case(6): {
                exit(0);
                break;
            }
            default: {
                cout << "\nInvalid value.  "
                     << "Please re-enter a value from the menu options below.";
                continue;
            }
        }
    }
}


void displayFileOptions(vector <Row>& chart) 
{
    int fileOption;
    
    cout << "Billboard Hot 100: Analysis \n"
         << " \n"
         << "This program will analyze weekly Billboard data spanning "
         <<   "from 1960 - 2020.\n\n"
         << "Select file option:\n"
         << "   1. To analyze charts top 50: 1960 - 1980 data file\n"
         << "   2. To analyze charts top 50: 1981 - 2000 data file\n"
         << "   3. To analyze charts top 50: 2001 - 2020 data file\n"
         << "Your choice --> ";
  
    cin >> fileOption;
  
    // read in relevant file
    switch(fileOption) {
        case(1): {
            readFile("charts_top50_1960_1980.csv", chart);
            break;
        }
        case(2): {
            readFile("charts_top50_1981_2000.csv", chart);
            break;
        }
        case(3): {
            readFile("charts_top50_2001_2020.csv", chart);
            break;
        }
        default: {
            cout << "\nInvalid value. Exiting Program.";
            exit(0);
            break;
        }
    }
  
    displayMenuOptions(chart, fileOption);
}


int main() {

    vector <Row> chart;
  
    displayFileOptions(chart);
  
    return 0;
}
