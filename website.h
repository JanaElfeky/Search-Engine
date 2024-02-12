#include <iostream>
#include <vector>
using namespace std;

class website{
public:
  string name;
  vector<string> keywords;
  int websiteNum;
  int clicks;
  int impressions;
  website();
  website(int wn);
  void setImpressions(int imp);
  int getImpressions();
};

class keyword{
public:
string word;
vector<website> associatedWeb;
keyword(string w);
void setWebsite(website website);
};