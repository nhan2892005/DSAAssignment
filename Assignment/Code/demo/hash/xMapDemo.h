/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xmapDemo.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 10:56
 */

#ifndef XHASHMAPDEMO_H
#define XHASHMAPDEMO_H
#include "hash/xMap.h"
#include "util/Point.h"
#include "util/ArrayLib.h"
#include "util/sampleFunc.h"
#include "util/FuncLib.h"


int hashFunc(int& key, int tablesize) {
    return key % tablesize;
}

void simpleMap() {
    int keys[] = {2, 12, 42, 72, 3, 45, 76, 30};
    int values[] = {35, 67, 100, 23, 68, 68, 72, 45};

    xmap<int, int> map(&hashFunc);
    for (int idx = 0; idx < 6; idx++) {
        map.put(keys[idx], values[idx]);
    }
    map.println();
    DLinkedList<int> list = map.clashes();
    list.println();

}

void hashDemo1() {
    int keys[] = {2, 12, 42, 72, 3, 45, 76, 30};
    int values[] = {35, 67, 100, 23, 68, 68, 72, 45};
    xmap<int, int> hash(&xmap<int, int>::simpleHash);
    for (int idx = 0; idx < 8; idx++) {
        hash.put(keys[idx], values[idx]);
    }
    hash.println();
}

int hashFunc(int*& item, int size) {
    return *item % size;
}

bool keyEQ(int*& lhs, int*& rhs) {
    return *lhs == *rhs;
}

void deleteKey(int *key) {
    delete key;
}

string key2str(int*& item) {
    stringstream os;
    os << *item;
    return os.str();
}

bool valueEQ(int*& lhs, int*& rhs) {
    return *lhs == *rhs;
}

string value2str(int*& item) {
    stringstream os;
    os << *item;
    return os.str();
}

void hashDemo2() {
    int keys[] = {2, 12, 42, 72, 3, 45, 76, 30};
    int values[] = {35, 67, 100, 23, 68, 68, 72, 45};

    xmap<int, int*> hashMap(
            &xmap<int, int*>::simpleHash,
            0.75,
            &valueEQ,
            &xmap<int, int*>::freeValue,
            0, //keyEQ
            0 //deleteKeys
            );
    for (int idx = 0; idx < 8; idx++) {
        hashMap.put(keys[idx], new int(values[idx]));
    }
    hashMap.println(0, &value2str);
}

void hashDemo3() {
    int keys[] = {2, 12, 42, 72, 3, 45, 76, 30};
    int values[] = {35, 67, 100, 23, 68, 68, 72, 45};

    xmap<int*, int*> hashMap(
            &hashFunc,
            0.75,
            &valueEQ,
            &xmap<int*, int*>::freeValue,
            &keyEQ,
            &xmap<int*, int*>::freeKey
            );
    for (int idx = 0; idx < 8; idx++) {
        hashMap.put(new int(keys[idx]), new int(values[idx]));
    }
    hashMap.println(&key2str, &value2str);
    int* pkey = new int(42);
    hashMap.remove(pkey, &deleteKey);
    delete pkey;
    hashMap.println(&key2str, &value2str);
}

void hashDemo4() {
    int keys[] = {2, 12, 42, 72, 3, 45, 76, 30};
    Point * values[] = {
        new Point(2.3, 4.5), new Point(4.2, 6.7),
        new Point(5.2, 7.2), new Point(9.1, 1.8),
        new Point(8.9, 9.1), new Point(8.1, 3.1),
        new Point(3.3, 5.4), new Point(7.3, 4.2)
    };

    xmap<int*, Point*> hashMap(
            &hashFunc,
            0.75,
            &Point::pointEQ,
            &xmap<int*, Point*>::freeValue,
            &keyEQ,
            &xmap<int*, Point*>::freeKey
            );
    for (int idx = 0; idx < 8; idx++) {
        hashMap.put(new int(keys[idx]), values[idx]);
    }
    hashMap.println(&key2str, &point2str);
}

void hashDemo5() {
    int keys[] = {2, 12, 42, 72, 3, 45, 76, 30};
    int values[] = {35, 67, 100, 23, 68, 68, 72, 45};
    xmap<int, int> hash(&xmap<int, int>::simpleHash);
    for (int idx = 0; idx < 8; idx++) {
        hash.put(keys[idx], values[idx]);
    }

    DLinkedList<int> keyset = hash.keys();
    cout << "Keys: " << keyset.toString() << endl;
    DLinkedList<int> valueset = hash.values();
    cout << "Values: " << valueset.toString() << endl;
}

void hashDemo6() {
    int count = 10000000;
    //int count = 100;
    int *keys = genIntArray(count, 0, 1999999999);
    xmap<int, int*> hash(&xmap<int, int*>::simpleHash);
    for (int idx = 0; idx < count; idx++) {
        hash.put(keys[idx], 0);
    }
    //hash.println();

    DLinkedList<int> clashes = hash.clashes();
    int max = -1;
    for (DLinkedList<int>::Iterator it = clashes.begin(); it != clashes.end(); it++) {
        int item = *it;
        if (item > max) max = item;
    }

    cout << "table size: " << hash.getCapacity() << endl;
    cout << "current count: " << hash.size() << endl;
    cout << "real load factor: " << (float) hash.size() / hash.getCapacity() << endl;
    cout << "max #collisions: " << max << endl;
    delete []keys;
}

int stringHash(string& str, int size) {
    long long int sum = 0;
    for (int idx = 0; idx < str.length(); idx++) sum += str[idx];
    return sum % size;
}

//https://raw.githubusercontent.com/icyrockcom/country-capitals/master/data/country-list.csv

string countries[] = {
    "country", "capital", "type",
    "Abkhazia", "Sukhumi", "countryCapital",
    "Afghanistan", "Kabul", "countryCapital",
    "Akrotiri and Dhekelia", "Episkopi Cantonment", "countryCapital",
    "Albania", "Tirana", "countryCapital",
    "Algeria", "Algiers", "countryCapital",
    "American Samoa", "Pago Pago", "countryCapital",
    "Andorra", "Andorra la Vella", "countryCapital",
    "Angola", "Luanda", "countryCapital",
    "Anguilla", "The Valley", "countryCapital",
    "Antigua and Barbuda", "St. John's", "countryCapital",
    "Argentina", "Buenos Aires", "countryCapital",
    "Armenia", "Yerevan", "countryCapital",
    "Aruba", "Oranjestad", "countryCapital",
    "Ascension Island", "Georgetown", "countryCapital",
    "Australia", "Canberra", "countryCapital",
    "Austria", "Vienna", "countryCapital",
    "Azerbaijan", "Baku", "countryCapital",
    "Bahamas", "Nassau", "countryCapital",
    "Bahrain", "Manama", "countryCapital",
    "Bangladesh", "Dhaka", "countryCapital",
    "Barbados", "Bridgetown", "countryCapital",
    "Belarus", "Minsk", "countryCapital",
    "Belgium", "Brussels", "countryCapital",
    "Belize", "Belmopan", "countryCapital",
    "Benin", "Porto-Novo", "countryCapital",
    "Bermuda", "Hamilton", "countryCapital",
    "Bhutan", "Thimphu", "countryCapital",
    "Bolivia", "Sucre", "countryCapital",
    "Bolivia", "La Paz", "countryCapital",
    "Bosnia and Herzegovina", "Sarajevo", "countryCapital",
    "Botswana", "Gaborone", "countryCapital",
    "Brazil", "Brasília", "countryCapital",
    "British Virgin Islands", "Road Town", "countryCapital",
    "Brunei", "Bandar Seri Begawan", "countryCapital",
    "Bulgaria", "Sofia", "countryCapital",
    "Burkina Faso", "Ouagadougou", "countryCapital",
    "Burundi", "Bujumbura", "countryCapital",
    "Cambodia", "Phnom Penh", "countryCapital",
    "Cameroon", "Yaoundé", "countryCapital",
    "Canada", "Ottawa", "countryCapital",
    "Cape Verde", "Praia", "countryCapital",
    "Cayman Islands", "George Town", "countryCapital",
    "Central African Republic", "Bangui", "countryCapital",
    "Chad", "N'Djamena", "countryCapital",
    "Chile", "Santiago", "countryCapital",
    "China", "Beijing", "countryCapital",
    "Christmas Island", "Flying Fish Cove", "countryCapital",
    "Cocos (Keeling) Islands", "West Island", "countryCapital",
    "Colombia", "Bogotá", "countryCapital",
    "Comoros", "Moroni", "countryCapital",
    "Cook Islands", "Avarua", "countryCapital",
    "Costa Rica", "San José", "countryCapital",
    "Croatia", "Zagreb", "countryCapital",
    "Cuba", "Havana", "countryCapital",
    "Curaçao", "Willemstad", "countryCapital",
    "Cyprus", "Nicosia", "countryCapital",
    "Czech Republic", "Prague", "countryCapital",
    "Côte d'Ivoire", "Yamoussoukro", "countryCapital",
    "Democratic Republic of the Congo", "Kinshasa", "countryCapital",
    "Denmark", "Copenhagen", "countryCapital",
    "Djibouti", "Djibouti", "countryCapital",
    "Dominica", "Roseau", "countryCapital",
    "Dominican Republic", "Santo Domingo", "countryCapital",
    "East Timor (Timor-Leste)", "Dili", "countryCapital",
    "Easter Island", "Hanga Roa", "countryCapital",
    "Ecuador", "Quito", "countryCapital",
    "Egypt", "Cairo", "countryCapital",
    "El Salvador", "San Salvador", "countryCapital",
    "Equatorial Guinea", "Malabo", "countryCapital",
    "Eritrea", "Asmara", "countryCapital",
    "Estonia", "Tallinn", "countryCapital",
    "Ethiopia", "Addis Ababa", "countryCapital",
    "Falkland Islands", "Stanley", "countryCapital",
    "Faroe Islands", "Tórshavn", "countryCapital",
    "Federated States of Micronesia", "Palikir", "countryCapital",
    "Fiji", "Suva", "countryCapital",
    "Finland", "Helsinki", "countryCapital",
    "France", "Paris", "countryCapital",
    "French Guiana", "Cayenne", "countryCapital",
    "French Polynesia", "Papeete", "countryCapital",
    "Gabon", "Libreville", "countryCapital",
    "Gambia", "Banjul", "countryCapital",
    "Georgia", "Tbilisi", "countryCapital",
    "Germany", "Berlin", "countryCapital",
    "Ghana", "Accra", "countryCapital",
    "Gibraltar", "Gibraltar", "countryCapital",
    "Greece", "Athens", "countryCapital",
    "Greenland", "Nuuk", "countryCapital",
    "Grenada", "St. George's", "countryCapital",
    "Guam", "Hagåtña", "countryCapital",
    "Guatemala", "Guatemala City", "countryCapital",
    "Guernsey", "St. Peter Port", "countryCapital",
    "Guinea", "Conakry", "countryCapital",
    "Guinea-Bissau", "Bissau", "countryCapital",
    "Guyana", "Georgetown", "countryCapital",
    "Haiti", "Port-au-Prince", "countryCapital",
    "Honduras", "Tegucigalpa", "countryCapital",
    "Hungary", "Budapest", "countryCapital",
    "Iceland", "Reykjavík", "countryCapital",
    "India", "New Delhi", "countryCapital",
    "Indonesia", "Jakarta", "countryCapital",
    "Iran", "Tehran", "countryCapital",
    "Iraq", "Baghdad", "countryCapital",
    "Ireland", "Dublin", "countryCapital",
    "Isle of Man", "Douglas", "countryCapital",
    "Israel", "Jerusalem", "countryCapital",
    "Italy", "Rome", "countryCapital",
    "Jamaica", "Kingston", "countryCapital",
    "Japan", "Tokyo", "countryCapital",
    "Jersey", "St. Helier", "countryCapital",
    "Jordan", "Amman", "countryCapital",
    "Kazakhstan", "Astana", "countryCapital",
    "Kenya", "Nairobi", "countryCapital",
    "Kiribati", "Tarawa", "countryCapital",
    "Kosovo", "Pristina", "countryCapital",
    "Kuwait", "Kuwait City", "countryCapital",
    "Kyrgyzstan", "Bishkek", "countryCapital",
    "Laos", "Vientiane", "countryCapital",
    "Latvia", "Riga", "countryCapital",
    "Lebanon", "Beirut", "countryCapital",
    "Lesotho", "Maseru", "countryCapital",
    "Liberia", "Monrovia", "countryCapital",
    "Libya", "Tripoli", "countryCapital",
    "Liechtenstein", "Vaduz", "countryCapital",
    "Lithuania", "Vilnius", "countryCapital",
    "Luxembourg", "Luxembourg", "countryCapital",
    "Macedonia", "Skopje", "countryCapital",
    "Madagascar", "Antananarivo", "countryCapital",
    "Malawi", "Lilongwe", "countryCapital",
    "Malaysia", "Kuala Lumpur", "countryCapital",
    "Maldives", "Malé", "countryCapital",
    "Mali", "Bamako", "countryCapital",
    "Malta", "Valletta", "countryCapital",
    "Marshall Islands", "Majuro", "countryCapital",
    "Mauritania", "Nouakchott", "countryCapital",
    "Mauritius", "Port Louis", "countryCapital",
    "Mexico", "Mexico City", "countryCapital",
    "Moldova", "Chisinau", "countryCapital",
    "Monaco", "Monaco", "countryCapital",
    "Mongolia", "Ulaanbaatar", "countryCapital",
    "Montenegro", "Podgorica", "countryCapital",
    "Montserrat", "Plymouth", "countryCapital",
    "Morocco", "Rabat", "countryCapital",
    "Mozambique", "Maputo", "countryCapital",
    "Myanmar", "Naypyidaw", "countryCapital",
    "Nagorno-Karabakh Republic", "Stepanakert", "countryCapital",
    "Namibia", "Windhoek", "countryCapital",
    "Nauru", "Yaren", "countryCapital",
    "Nepal", "Kathmandu", "countryCapital",
    "Netherlands", "Amsterdam", "countryCapital",
    "New Caledonia", "Nouméa", "countryCapital",
    "New Zealand", "Wellington", "countryCapital",
    "Nicaragua", "Managua", "countryCapital",
    "Niger", "Niamey", "countryCapital",
    "Nigeria", "Abuja", "countryCapital",
    "Niue", "Alofi", "countryCapital",
    "Norfolk Island", "Kingston", "countryCapital",
    "North Korea", "Pyongyang", "countryCapital",
    "Northern Cyprus", "Nicosia", "countryCapital",
    "United Kingdom Northern Ireland", "Belfast", "countryCapital",
    "Northern Mariana Islands", "Saipan", "countryCapital",
    "Norway", "Oslo", "countryCapital",
    "Oman", "Muscat", "countryCapital",
    "Pakistan", "Islamabad", "countryCapital",
    "Palau", "Ngerulmud", "countryCapital",
    "Palestine", "Jerusalem", "countryCapital",
    "Panama", "Panama City", "countryCapital",
    "Papua New Guinea", "Port Moresby", "countryCapital",
    "Paraguay", "Asunción", "countryCapital",
    "Peru", "Lima", "countryCapital",
    "Philippines", "Manila", "countryCapital",
    "Pitcairn Islands", "Adamstown", "countryCapital",
    "Poland", "Warsaw", "countryCapital",
    "Portugal", "Lisbon", "countryCapital",
    "Puerto Rico", "San Juan", "countryCapital",
    "Qatar", "Doha", "countryCapital",
    "Republic of China (Taiwan)", "Taipei", "countryCapital",
    "Republic of the Congo", "Brazzaville", "countryCapital",
    "Romania", "Bucharest", "countryCapital",
    "Russia", "Moscow", "countryCapital",
    "Rwanda", "Kigali", "countryCapital",
    "Saint Barthélemy", "Gustavia", "countryCapital",
    "Saint Helena", "Jamestown", "countryCapital",
    "Saint Kitts and Nevis", "Basseterre", "countryCapital",
    "Saint Lucia", "Castries", "countryCapital",
    "Saint Martin", "Marigot", "countryCapital",
    "Saint Pierre and Miquelon", "St. Pierre", "countryCapital",
    "Saint Vincent and the Grenadines", "Kingstown", "countryCapital",
    "Samoa", "Apia", "countryCapital",
    "San Marino", "San Marino", "countryCapital",
    "Saudi Arabia", "Riyadh", "countryCapital",
    "Scotland", "Edinburgh", "countryCapital",
    "Senegal", "Dakar", "countryCapital",
    "Serbia", "Belgrade", "countryCapital",
    "Seychelles", "Victoria", "countryCapital",
    "Sierra Leone", "Freetown", "countryCapital",
    "Singapore", "Singapore", "countryCapital",
    "Sint Maarten", "Philipsburg", "countryCapital",
    "Slovakia", "Bratislava", "countryCapital",
    "Slovenia", "Ljubljana", "countryCapital",
    "Solomon Islands", "Honiara", "countryCapital",
    "Somalia", "Mogadishu", "countryCapital",
    "Somaliland", "Hargeisa", "countryCapital",
    "South Africa", "Pretoria", "countryCapital",
    "South Georgia and the South Sandwich Islands", "Grytviken", "countryCapital",
    "South Korea", "Seoul", "countryCapital",
    "South Ossetia", "Tskhinvali", "countryCapital",
    "South Sudan South Sudan", "Juba", "countryCapital",
    "Spain", "Madrid", "countryCapital",
    "Sri Lanka", "Sri Jayawardenapura Kotte", "countryCapital",
    "Sudan", "Khartoum", "countryCapital",
    "Suriname", "Paramaribo", "countryCapital",
    "Swaziland", "Mbabane", "countryCapital",
    "Sweden", "Stockholm", "countryCapital",
    "Switzerland", "Bern", "countryCapital",
    "Syria", "Damascus", "countryCapital",
    "São Tomé and Príncipe", "São Tomé", "countryCapital",
    "Tajikistan", "Dushanbe", "countryCapital",
    "Tanzania", "Dodoma", "countryCapital",
    "Thailand", "Bangkok", "countryCapital",
    "Togo", "Lomé", "countryCapital",
    "Tonga", "Nukuʻalofa", "countryCapital",
    "Transnistria", "Tiraspol", "countryCapital",
    "Trinidad and Tobago", "Port of Spain", "countryCapital",
    "Tristan da Cunha", "Edinburgh of the Seven Seas", "countryCapital",
    "Tunisia", "Tunis", "countryCapital",
    "Turkey", "Ankara", "countryCapital",
    "Turkmenistan", "Ashgabat", "countryCapital",
    "Turks and Caicos Islands", "Cockburn Town", "countryCapital",
    "Tuvalu", "Funafuti", "countryCapital",
    "Uganda", "Kampala", "countryCapital",
    "Ukraine", "Kiev", "countryCapital",
    "United Arab Emirates", "Abu Dhabi", "countryCapital",
    "United Kingdom; England", "London", "countryCapital",
    "United States", "Washington, D.C.", "countryCapital",
    "United States Virgin Islands", "Charlotte Amalie", "countryCapital",
    "Uruguay", "Montevideo", "countryCapital",
    "Uzbekistan", "Tashkent", "countryCapital",
    "Vanuatu", "Port Vila", "countryCapital",
    "Vatican City", "Vatican City", "countryCapital",
    "Venezuela", "Caracas", "countryCapital",
    "Vietnam", "Hanoi", "countryCapital",
    "Wales", "Cardiff", "countryCapital",
    "Wallis and Futuna", "Mata-Utu", "countryCapital",
    "Western Sahara", "El Aaiún", "countryCapital",
    "Yemen", "Sanaá", "countryCapital",
    "Zambia", "Lusaka", "countryCapital",
    "Zimbabwe", "Harare", "countryCapital"
};
int ncountry = 248;


void hashDemo7() {
    xmap<string, string> map(&stringHash);
    for (int c = 0; c < ncountry * 3; c += 3) {
        string name = countries[c];
        string capital = countries[c + 1];
        map.put(name, capital);
    }
    DLinkedList<int> clashes = map.clashes();
    int max = -1;
    for (DLinkedList<int>::Iterator it = clashes.begin(); it != clashes.end(); it++) {
        int item = *it;
        if (item > max) max = item;
    }
    cout << "table size: " << map.getCapacity() << endl;
    cout << "current count: " << map.size() << endl;
    cout << "real load factor: " << (float) map.size() / map.getCapacity() << endl;
    cout << "max #collisions: " << max << endl;

    string country = "Vietnam";
    cout << "Capital of " << country << " is " << map.get("Vietnam") << endl;
}

int countryHash(string& country, int size){
    int sum =0;
    for(int idx=0; idx < country.length(); idx++)
        sum += country[idx];
    return sum%size;
}

void countryDemo(){
    xmap<string, string> map(&countryHash);
    for(int idx=0; idx < ncountry*3; idx+=3){
        string name = countries[idx];
        string capital = countries[idx+1];
        map.put(name, capital);
    }
    map.println();
    
    DLinkedList<int> list = map.clashes();
    int max = -1;
    for(DLinkedList<int>::Iterator it= list.begin(); it != list.end(); it++){
        if (*it > max) max = *it;
    }
    cout << "Size: " << map.size() << endl;
    cout << "MAX COLLISION: " << max << endl;
    
    string countryname = "Thailand";
    cout << countryname << " : " << map.get(countryname) << endl;
}
#endif /* XHASHMAPDEMO_H */

