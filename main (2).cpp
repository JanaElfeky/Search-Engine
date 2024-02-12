#include "website.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

website websites[30] = {
    website(1),  website(2),  website(3),  website(4),  website(5),
    website(6),  website(7),  website(8),  website(9),  website(10),
    website(11), website(12), website(13), website(14), website(15),
    website(16), website(17), website(18), website(19), website(20),
    website(21), website(22), website(23), website(24), website(25),
    website(26), website(27), website(28), website(29), website(30)};

keyword keywords1[50] = {keyword("machine learning"),
                         keyword("deep"),
                         keyword("learning"),
                         keyword("artificial"),
                         keyword("intelligence"),
                         keyword("programming languages"),
                         keyword("computer"),
                         keyword("science"),
                         keyword("math"),
                         keyword("algorithm"),
                         keyword("software"),
                         keyword("network systems"),
                         keyword("security"),
                         keyword("cryptography"),
                         keyword("computer architecture"),
                         keyword("database"),
                         keyword("sun"),
                         keyword("photosynthesis"),
                         keyword("webpage"),
                         keyword("spider"),
                         keyword("bee"),
                         keyword("fire ant"),
                         keyword("flower"),
                         keyword("plants"),
                         keyword("trees"),
                         keyword("water drop"),
                         keyword("gum"),
                         keyword("soil"),
                         keyword("earth"),
                         keyword("fire"),
                         keyword("air"),
                         keyword("moon"),
                         keyword("stars"),
                         keyword("galaxy"),
                         keyword("digital"),
                         keyword("design"),
                         keyword("analysis of algorithms"),
                         keyword("algebra"),
                         keyword("calculus"),
                         keyword("geometry"),
                         keyword("physics"),
                         keyword("chemistry"),
                         keyword("biology"),
                         keyword("medicine"),
                         keyword("medical"),
                         keyword("knight"),
                         keyword("tour"),
                         keyword("sword"),
                         keyword("shield"),
                         keyword("spear")};

int outdegree(vector<pair<string, string>> edges, string website) {
  int degree = 0;
  for (int i = 0; i < edges.size(); i++) {
    if (edges[i].first == website)
      degree++;
  }
  return degree;
}

vector<int> pageRank() {
  vector<pair<string, string>> edges;
  string line, word, temp;
  fstream webgraph;
  webgraph.open("webgraph.csv", ios::in);
  while (getline(webgraph, line)) {
    string first, second;
    stringstream ss(line);
    getline(ss, first, ',');
    getline(ss, second);
    edges.push_back(
        make_pair(first, second)); // translates lines in csv file to pairs in a
                                   // vector, first points to second
  }
  webgraph.close();

  vector<vector<double>> page_rank(30, vector<double>(4, 0.0));
  for (int i = 0; i < 30; i++) {
    page_rank[i][0] = 0.25;
  }
  for (int j = 1; j < 3; j++) {
    for (int i = 0; i < 30; i++) {
      for (int k = 0; k < 30; k++) {
        if (edges[k].second == websites[i].name) {
          page_rank[i][j] +=
              page_rank[i][j - 1] / outdegree(edges, edges[k].first);
        }
      }
    }
  }
  double max;
  int maxIndex;
  int rank = 30;
  while (rank > 0) {
    max = 0;
    for (int i = 0; i < 30; i++) {
      if (page_rank[i][2] >= max) {
        max = page_rank[i][2];
        maxIndex = i;
      }
    }
    page_rank[maxIndex][3] = rank;
    page_rank[maxIndex][2] = -1;
    rank--;
  }
  vector<int> finalPR(30);
  for (int i = 0; i < 30; i++) {
    finalPR[i] = page_rank[i][3];
  }
  return finalPR;
}

int CTR(website website) { return (website.clicks / website.getImpressions()); }

double calculateScore(vector<int> PR, website website) {
  double PRnorm = (PR[website.websiteNum - 1] - 1) / 29.0;
  int impression = website.getImpressions();
  
  for (int i=0;i<30;i++){
    if (websites[i].name==website.name){
      website.clicks=websites[i].clicks;
    }
  }
  
  return (
      (0.4 * PRnorm) +
      (0.4 * (((1 - ((0.1 * impression) / (1 + 0.1 * impression))) * PRnorm) +
              ((0.1 * impression) / (1 + 0.1 * impression)) *
                  CTR(website))));
}

unordered_map<string, vector<website>> fillMap() {
  unordered_map<string, vector<website>> hashMap;
  for (int i = 0; i < 50; i++) {
    hashMap[keywords1[i].word] = keywords1[i].associatedWeb;
  }
  for (int j = 0; j < 50; j++) {
    for (int i = 0; i < hashMap[keywords1[j].word].size(); i++) {
      for (int k = 0; k < 30; k++) {
        if (hashMap[keywords1[j].word][i].name == websites[k].name) {
          hashMap[keywords1[j].word][i].setImpressions(
              websites[k].getImpressions());
          hashMap[keywords1[j].word][i].clicks = websites[k].clicks;
        }
      }
    }
  }
  return hashMap;
}

vector<website> ranks(string search,
                      unordered_map<string, vector<website>> hashMap) {
  vector<website> associatedWebsites;
  associatedWebsites.clear();

  if (hashMap.find(search) != hashMap.end()) {
    associatedWebsites = hashMap.find(search)->second;
    for (int i = 0; i < associatedWebsites.size(); i++) {
      associatedWebsites[i].setImpressions(
          hashMap.find(search)->second[i].getImpressions());
      associatedWebsites[i].clicks = hashMap.find(search)->second[i].clicks;
    }
  }

  return associatedWebsites;
}

vector<website> sort(vector<website> associatedWebsites, vector<int> PR) {
  int n = associatedWebsites.size();
  int i, j;
  bool swapped;
  for (i = 0; i < n - 1; i++) {
    swapped = false;
    for (j = 0; j < n - i - 1; j++) {
      double webScore = calculateScore(PR, associatedWebsites[j]);
      double webScore2 = calculateScore(PR, associatedWebsites[j + 1]);
      if (webScore < webScore2) {
        swap(associatedWebsites[j], associatedWebsites[j + 1]);
        swapped = true;
      }
    }
    // If no two elements were swapped
    // by inner loop, then break
    if (swapped == false)
      break;
  }
  return associatedWebsites;
}

void updateInfo(vector<website> results3) {
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < results3.size(); j++) {
      if (websites[i].name == results3[j].name) {
        websites[i].setImpressions(results3[j].getImpressions());
        websites[i].clicks = results3[i].clicks;
      }
    }
  }
  ofstream impressionsFile;
  impressionsFile.open("impressions.csv", ios::out | ios::trunc);
  for (int i = 0; i < 30; i++) {
    impressionsFile << websites[i].name << "," << websites[i].getImpressions()
                    << "\n";
  }
  impressionsFile.close();
}

void retrieval(string search, vector<int> PR,
               unordered_map<string, vector<website>> hashMap) {
  vector<website> results;
  vector<website> results2;
  vector<website> results3;
  if (search[0] == '"' && search[search.length() - 1] == '"') {
    results3 = ranks(search.substr(1, search.length() - 2), hashMap);
    results3 = sort(results3, PR);
    for (int i = 0; i < results3.size(); i++) {
      results3[i].setImpressions(sort(results3, PR)[i].getImpressions());
      results3[i].clicks = sort(results3, PR)[i].clicks;
    }
    for (int i = 0; i < results3.size(); i++) {
      results3[i].setImpressions(results3[i].getImpressions() + 1);
    }
    updateInfo(results3);
  } else if (search.find(" AND ") != string::npos) {
    results = ranks(search.substr(0, search.find(" AND ")), hashMap);
    results2 = ranks(search.substr(search.find(" AND ") + 5, search.length()),
                     hashMap);
    for (const auto &element1 : results) {
      for (const auto &element2 : results2) {
        if (element1.name == element2.name) {
          results3.push_back(element1);
          break; // Once a common element is found, skip further comparisons for
                 // the current element1
        }
      }
    }
    results3 = sort(results3, PR);
    for (int i = 0; i < results3.size(); i++) {
      results3[i].setImpressions(sort(results3, PR)[i].getImpressions());
      results3[i].clicks = sort(results3, PR)[i].clicks;
    }
    for (int i = 0; i < results3.size(); i++) {
      results3[i].setImpressions(results3[i].getImpressions() + 1);
    }
    updateInfo(results3);
  } else if (search.find(" OR ") != string::npos) {
    results = ranks(search.substr(0, search.find(" OR ")), hashMap);
    results2 =
        ranks(search.substr(search.find(" OR ") + 4, search.length()), hashMap);

    results3.insert(results3.end(), results.begin(), results.end());

    // Initialize encountered flag for all elements to false
    bool encountered[results2.size()]; // Array to track encountered elements
    for (size_t i = 0; i < results2.size(); ++i) {
      encountered[i] = false;
    }

    // Iterate through the second vector and check for duplicates
    for (size_t i = 0; i < results2.size(); ++i) {
      if (!encountered[i]) {
        encountered[i] = true;
        bool duplicateFound = false;

        for (const auto &element : results3) {
          if (element.name == results2[i].name) {
            duplicateFound = true;
            break; // No need to continue iterating if a duplicate is found
          }
        }

        if (!duplicateFound) {
          results3.push_back(results2[i]);
        }
      }
    }
    results3 = sort(results3, PR);
    for (int i = 0; i < results3.size(); i++) {
      results3[i].setImpressions(sort(results3, PR)[i].getImpressions());
      results3[i].clicks = sort(results3, PR)[i].clicks;
    }
    for (int i = 0; i < results3.size(); i++) {
      results3[i].setImpressions(results3[i].getImpressions() + 1);
    }
    updateInfo(results3);
  } else {
    istringstream ss(search);
    string word;
    vector<string> searchWords;
    while (ss >> word) {
      searchWords.push_back(word);
    }
    if (searchWords.size() == 1) {
      results3 = ranks(searchWords[0], hashMap);
      results3 = sort(results3, PR);
      for (int i = 0; i < results3.size(); i++) {
        // cout<<results[i].getImpressions()<<endl;

        results3[i].setImpressions(results3[i].getImpressions() + 1);
        // cout<<results[i].getImpressions()<<endl;
      }
      updateInfo(results3);
    } else if (searchWords.size() == 2) {
      results = ranks(searchWords[0], hashMap);
      results2 = ranks(searchWords[1], hashMap);

      results3.insert(results3.end(), results.begin(), results.end());

      // Initialize encountered flag for all elements to false
      bool encountered[results2.size()]; // Array to track encountered elements
      for (size_t i = 0; i < results2.size(); ++i) {
        encountered[i] = false;
      }

      // Iterate through the second vector and check for duplicates
      for (size_t i = 0; i < results2.size(); ++i) {
        if (!encountered[i]) {
          encountered[i] = true;
          bool duplicateFound = false;

          for (const auto &element : results3) {
            if (element.name == results2[i].name) {
              duplicateFound = true;
              break; // No need to continue iterating if a duplicate is found
            }
          }

          if (!duplicateFound) {
            results3.push_back(results2[i]);
          }
        }
      }

      results3 = sort(results3, PR);
      for (int i = 0; i < results3.size(); i++) {
        results3[i].setImpressions(sort(results3, PR)[i].getImpressions());
        results3[i].clicks = sort(results3, PR)[i].clicks;
      }
      for (int i = 0; i < results3.size(); i++) {
        results3[i].setImpressions(results3[i].getImpressions() + 1);
      }
      updateInfo(results3);
    }
  }
  for (int i = 0; i < results3.size(); i++)
    cout << i + 1 << ".  " << results3[i].name << endl;
}

int main_menu() {
  cout << "\nWelcome!" << endl;
  cout << "What would you like to do?" << endl;
  cout << "1. New search" << endl;
  cout << "2. Exit" << endl;
  int choice;
  cin >> choice;
  return choice;
}

int search_menu(string &search, vector<int> PR,
                unordered_map<string, vector<website>> hashMap) {
  cout << "Enter your search :\n";
  cin.ignore();
  getline(cin, search);
  cout << "\nSearch results:" << endl;
  retrieval(search, PR, hashMap);
  cout << "\nWould you like to" << endl;
  cout << "1. Choose a webpage to open" << endl;
  cout << "2. New search" << endl;
  cout << "3. Exit to main menu" << endl;
  int choice;
  cin >> choice;
  return choice;
}

int open_webpage_menu() {
  string web;
  cout << "Which website would you like to view?\nType in your choice: _\n";
  cin >> web;
  for (int i = 0; i < 30; i++) {
    if (web == websites[i].name) {
      websites[i].clicks = websites[i].clicks + 1;
    }
  }
  cout << "\nYou're now viewing " << web << "." << endl;
  cout << "\nWould you like to" << endl;
  cout << "1. Back to search results" << endl;
  cout << "2. New search" << endl;
  cout << "3. Exit to main menu" << endl;
  int choice;
  cin >> choice;
  return choice;
}

void run_program(string &search, vector<int> PR,
                 unordered_map<string, vector<website>> hashMap) {
  while (true) {
    int choice = main_menu();
    if (choice == 1) {
      choice = search_menu(search, PR, hashMap);
      while (choice != 3) {
        if (choice == 1) {
          choice = open_webpage_menu();
        } else {
          choice = search_menu(search, PR, hashMap);
        }
      }
    } else if (choice == 2) {
      cout << "\nExiting the program." << endl;
      break;
    } else {
      cout << "\nInvalid choice. Please try again." << endl;
    }
  }
}

int main() {
  fstream webgraph;
  string temp, line;
  webgraph.open("webgraph.csv", ios::out);

  for (int i = 0; i < 100; i++) {
    int random1 = rand() % 30;
    int random2 = rand() % 30;
    if (random1 != random2)
      webgraph << websites[random1].name << "," << websites[random2].name
               << "\n";
    else
      webgraph << websites[random1].name << ","
               << websites[(random2 + random1) % 30].name << "\n";
  }
  webgraph.close();

  fstream keywords;
  keywords.open("keywords.csv", ios::out);
  for (int i = 0; i < 30; i++) {
    keywords << websites[i].name;
    int n = rand() % 4;
    for (int j = 1; j <= n + 1; j++) {
      int random = rand() % 50;
      for (int k = 0; k < websites[i].keywords.size(); k++) {
        if (websites[i].keywords[k] == keywords1[random].word) {
          int random1 = rand() % 50;
          random = (random + random1) % 50;
        }
      }
      websites[i].keywords.push_back(keywords1[random].word);
      keywords1[random].setWebsite(websites[i]);
      keywords << "," << keywords1[random].word;
    }
    keywords << "\n";
  }
  keywords.close();

  fstream impressions;
  impressions.open("impressions.csv", ios::out);
  for (int i = 0; i < 30; i++) {
    int random = rand() % 20 + 1;
    websites[i].setImpressions(random);
    impressions << websites[i].name << "," << random << "\n";
  }
  impressions.close();

  vector<int> PR = pageRank();
  unordered_map<string, vector<website>> hashMap = fillMap();
  string search;
  run_program(search, PR, hashMap);

  return 0;
}