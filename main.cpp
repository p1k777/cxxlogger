#include <iostream>
#include <fstream>

#include "logger.hpp"
#include "example.hpp"

void myns::LogType(std::ostream& os, const MyPoint& mp) {
    os << '(' << mp.x << ", " << mp.y << ')';
}

namespace logger {
    
    void LogType(std::ostream& os, const myns::GeomVector& gv) {
        os << '{' << gv.x << ", " << gv.y << '}';
    }

} // namespace logger


int main() {
    logger::ConsoleLogger console_log(&std::cout);

    console_log.Debug(myns::MyPoint{2, 3});

    // если вернуть строчку, получим CE
    // console_log.Debug(myns::GeomVector{2, 3});

    return 0;
}
