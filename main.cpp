#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    string query;
    is >> query;
	q.stops.clear();
	if( query == "ALL_BUSES"){
		q.type = QueryType::AllBuses;
	}
	else if (query == "BUSES_FOR_STOP"){
		q.type = QueryType::BusesForStop;
		is>>q.stop;
	}
	else if (query == "STOPS_FOR_BUS"){
		q.type = QueryType::StopsForBus;
		is>>q.bus;
	}
	else if (query == "NEW_BUS"){
		q.type = QueryType::NewBus;
		is>>q.bus;
		int n;
		is>>n;
		for(int i = 0; i<n; i++){
			string s;
			is>>s;
			q.stops.push_back(s);
		}
	}  
    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
	string stop;
	vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
	if(r.buses.empty()){
		os<<"No stop";
	}
	else{
		/*for(string s : r.buses){
			os<<s<<" ";
		}*/
		for(int j = 0 ; j< r.buses.size() -1; j++){
			os<<r.buses[j]<<" ";
		}
		os<<r.buses[r.buses.size()-1];
	}
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
	//map<string, vector<string>> stopsToBuses;
	vector<string> stops;
	vector<vector<string>> buses;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
	if(r.stops.empty()){
		os<<"No bus";
	}
	else{
		int i = 0;
		for(const string &s : r.stops){
			os<<"Stop "<<s<<": ";
			/*for(const string &bus : r.buses[i]){
				os<<bus<<" ";
			}*/
			for(int j = 0; j< r.buses[i].size()-1;j++){
				os<<r.buses[i][j]<<" ";
			}
			os<<r.buses[i][r.buses[i].size()-1];

            i++;
            if(i<r.stops.size()){
                os<<endl;
            }			
		}
	}
    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
	map<string,vector<string>> all_buses;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
	if(r.all_buses.empty()){
		os<<"No buses";
	}
	else{
        int i = 0;
		for(const auto &[k,v] : r.all_buses){
			os<<"Bus "<<k<<": ";
			for(int j = 0; j< v.size()-1;j++){
				os<<v[j]<<" ";
			}
			os<<v[v.size()-1];
			/*for(const string s: v){
				os<<s<<" ";
			}*/
            ++i;
            if(i<r.all_buses.size()){
                os<<endl;
            }
		}
	}
    return os;
}

class BusManager {
public:
	
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
		for(string s : stops){
			_buses_to_stops[bus].push_back(s);
			_stops_to_buses[s].push_back(bus);
		}
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
		BusesForStopResponse q;
		q.stop = stop;
		if(_stops_to_buses.count(stop)){
			q.buses = _stops_to_buses.at(stop);
		}
		return q;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) {
        // Реализуйте этот метод
		StopsForBusResponse q;
		vector<string> buses;	
        if (_buses_to_stops.count(bus)) {
            for (const string& stop : _buses_to_stops[bus]) {
				q.stops.push_back(stop);
				buses.clear();
                if (_stops_to_buses[stop].size() == 1) {
                    buses.push_back("no interchange");
                } 
				else {
                    for (string& other_bus : _stops_to_buses[stop]) {
                        if (bus != other_bus) {
                            buses.push_back(other_bus);
                        }
                    }
                }
				q.buses.push_back(buses);
        	}
		}
		return q;
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
		AllBusesResponse _all_buses;
		_all_buses.all_buses = _buses_to_stops;
		return _all_buses;
    }
private:
	map<string,vector<string>> _buses_to_stops;
	map<string,vector<string>> _stops_to_buses;
};

// Не меняя тела функции main, реализуйте функции и классы выше
void Test(){
	Query q1, q2, q3;
	BusManager bm;
	q1.bus = "32";
	q1.stops.push_back("Stop10");
	q1.stops.push_back("Marushkino");
	q1.stops.push_back("Vnukovo");
	bm.AddBus(q1.bus, q1.stops);

	q2.bus = "32K";
	q2.stops.push_back("Tolstopaltsevo");
	q2.stops.push_back("Marushkino");
	q2.stops.push_back("Vnukovo");
	q2.stops.push_back("Peredelkino");
	q2.stops.push_back("Solntsevo");
	q2.stops.push_back("Skolkovo");
	bm.AddBus(q2.bus, q2.stops);

	q3.bus = "3";
	q3.stops.push_back("Stop10");
	q3.stops.push_back("Stop2");
	q3.stops.push_back("Stop3");
	q3.stops.push_back("Stop40");
	bm.AddBus(q3.bus, q3.stops);

	cout << bm.GetAllBuses() << endl;
	cout << bm.GetBusesForStop("Vnukovo")<<endl;
	cout << bm.GetBusesForStop("Stop4")<<endl;
	cout << bm.GetStopsForBus("3")<<endl;
}

int main() {
	//Test();
    int query_count;
    Query q;
    cin >> query_count;
    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop)<<endl;;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus)<<endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses()<<endl;
                break;
        }
    }
}