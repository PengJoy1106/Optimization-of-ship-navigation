#include <iostream>
#include <cmath>


constexpr double kEarthRadiusKm = 6371.0; // 地球半径（公里）
constexpr double kPi = 3.14159265358979323846; // π 的近似值

double degreesToRadians(double degrees) {
    return degrees * kPi / 180.0;
}

double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    // 将经纬度从角度转换为弧度
    lat1 = degreesToRadians(lat1);
    lon1 = degreesToRadians(lon1);
    lat2 = degreesToRadians(lat2);
    lon2 = degreesToRadians(lon2);

    // 计算差值
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    // 应用 Haversine 公式
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon / 2) * std::sin(dlon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    double distance = kEarthRadiusKm * c;
    return distance;
}

int main() {

    // 安徽省合肥市的经纬度
//    double latAnhui = 31.8639; // 纬度
//    double lonAnhui = 117.2808; // 经度
    // 示例坐标：纽约市和洛杉矶的经纬度
    double latNYC = 40.7128; // 纬度
    double lonNYC = -74.0060; // 经度
    // 江苏省南京市的经纬度
    double latJiangsu = 32.0603; // 纬度
    double lonJiangsu = 118.7969; // 经度

    // 计算距离
    double distance = haversineDistance(latNYC, lonNYC, latJiangsu, lonJiangsu);

    std::cout <<" 两地之间的距离为 " << distance << " 公里" << std::endl;

    return 0;
}
