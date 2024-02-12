#include <iostream>
#include "website.h"
using namespace std;

website::website(int wn){
  websiteNum = wn;
  name="www.test"+to_string(wn)+".com";
  keywords.clear();
  clicks=0;
  impressions=1;
}

website::website(){
  websiteNum = 0;
  name="www.test0.com";
  keywords.clear();
  clicks=0;
  impressions=1;
}
void website::setImpressions(int imp){
  impressions=imp;
}

 int website:: getImpressions(){
   return impressions;
 }

keyword::keyword(string w){
  word=w;
  associatedWeb.clear();
}
void keyword::setWebsite(website website){
  associatedWeb.push_back(website);
  
}