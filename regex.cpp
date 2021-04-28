// Copyright [2021] <Gabriel Vega>

#include <boost/regex.hpp>
#include <iostream>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::stoi;
using boost::regex;
using boost::smatch;
using boost::regex_match;
using boost::gregorian::from_string;
using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;
int main(int argc, char* argv[]) {
    // set up input stream
    std::ifstream iF(argv[1]);
    if (iF.fail()) {
        cout << "incorrect arguments passed" << endl;
        exit(1);
    }
    // set up output stream
    string outputName(argv[1]);
    outputName += ".rpt";
    std::ofstream oF(outputName);
    // sets up regular expressions to find the startup and success message
    regex startup(
      "([0-9]+)-([0-9]+)-([0-9]+) "
      "([0-9]+):([0-9]+):([0-9]+): "
      "\\(log.c.166\\) server started.*");
    regex success(
      "([0-9]+)-([0-9]+)-([0-9]+) "
      "([0-9]+):([0-9]+):([0-9]+).([0-9]+):INFO:"
      "oejs.AbstractConnector:Started SelectChannelConnector@0.0.0.0:9080.*");
    string line;
    int lineNum = 1;
    ptime t1;
    date d;
    bool repeat = false;
    while (getline(iF, line)) {
        smatch sm;
        if (regex_match(line, sm, startup)) {
            if (repeat) {
                oF << "**** Incomplete boot ****" << endl << endl;
            }
            oF << "=== Device boot ===" << endl;
            oF << lineNum << "(device5_intouch.log): " << stoi(sm[1]) << '-'
            << stoi(sm[2]) << '-' << stoi(sm[3]) << ' ' << stoi(sm[4]) << ':'
            << stoi(sm[5]) << ':' << stoi(sm[6]) << ' ' << "Boot Start" << endl;
            date tempD(stoi(sm[1]), stoi(sm[2]), stoi(sm[3]));
            d = tempD;
            ptime temp = ptime(d, time_duration(stoi(sm[4]),
            stoi(sm[5]), stoi(sm[6])));
            t1 = temp;
            repeat = true;
        } else if (regex_match(line, sm, success)) {
            oF << lineNum << "(device5_intouch.log): " << stoi(sm[1]) << '-'
            << stoi(sm[2]) << '-' << stoi(sm[3]) << ' ' << stoi(sm[4]) << ':'
            << stoi(sm[5]) << ':' << stoi(sm[6]) << ' ' << "Boot Completed"
            << endl;
            ptime t2(date(stoi(sm[1]), stoi(sm[2]), stoi(sm[3])),
            time_duration(stoi(sm[4]), stoi(sm[5]), stoi(sm[6])));
            time_duration td = t2 - t1;
            oF << "\t" << "Boot Time: " << td.total_milliseconds()
            << "ms" << endl << endl;
            repeat = false;
        }
        ++lineNum;
    }
}
