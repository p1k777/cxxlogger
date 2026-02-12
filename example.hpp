#include <ostream>

namespace myns {
    
    struct MyPoint {
        float x, y;
    };

    void LogType(std::ostream& os, const MyPoint& mp);

    struct GeomVector {
        float x, y;
    };

} // namespace myns


